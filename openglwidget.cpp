#include "openglwidget.h"
#include <QDebug>

// Make sure to include the correct GLU header
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_quadric(nullptr)
{
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent(); // Important for cleanup of OpenGL resources
    if (m_quadric) {
        gluDeleteQuadric(m_quadric);
    }
    doneCurrent();
}

void OpenGLWidget::loadData(const QVector<QVector3D>& points)
{
    m_spherePositions = points;
    // Adjust zoom based on data if needed, or find bounding box
    // For now, a fixed zoom and letting user adjust is fine.
    if (!m_spherePositions.isEmpty()) {
        // Basic auto-zoom (very simple, might need refinement)
        float maxCoord = 0.0f;
        for(const auto& p : m_spherePositions) {
            if (qAbs(p.x()) > maxCoord) maxCoord = qAbs(p.x());
            if (qAbs(p.y()) > maxCoord) maxCoord = qAbs(p.y());
            if (qAbs(p.z()) > maxCoord) maxCoord = qAbs(p.z());
        }
        m_zoom = -maxCoord * 2.5f; // Heuristic for initial zoom
        if (m_zoom > -2.0f) m_zoom = -2.0f; // Min zoom distance
        if (m_zoom == 0.0f) m_zoom = -10.0f; // Default if all points are at origin
    }
    update(); // Trigger repaint
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // Initialize QOpenGLFunctions

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Dark blue background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // Use glColor for material color

    // Setup light 0
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_position[] = { 5.0f, 5.0f, 5.0f, 1.0f }; // Positional light

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    m_quadric = gluNewQuadric();
    if (!m_quadric) {
        qWarning("Failed to create GLU quadric object.");
        return;
    }
    gluQuadricNormals(m_quadric, GLU_SMOOTH); // Generate smooth normals for lighting
    //gluQuadricDrawStyle(m_quadric, GLU_FILL); // Default is fill
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (h == 0) h = 1;
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 0.1, 100.0); // Field of view, aspect ratio, near clip, far clip
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply camera transformations
    glTranslatef(0.0f, 0.0f, m_zoom);
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f); // Rotate around X axis
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f); // Rotate around Y axis
    // glRotatef(m_zRot, 0.0f, 0.0f, 1.0f); // Rotate around Z axis

    if (!m_quadric) return;

    glColor3f(0.8f, 0.2f, 0.2f); // Reddish color for spheres

    for (const QVector3D& pos : m_spherePositions)
    {
        glPushMatrix();
        glTranslatef(pos.x(), pos.y(), pos.z());
        gluSphere(m_quadric, m_sphereRadius, 32, 32); // Quadric, radius, slices, stacks
        glPopMatrix();
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastMousePos.x();
    int dy = event->y() - m_lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_xRot += dy * 0.25f;
        m_yRot += dx * 0.25f;
        update();
    } else if (event->buttons() & Qt::RightButton) { // Example: Pan (less intuitive with current setup)
        // Could implement panning here, or Z-rotation
        // m_xRot += dy * 0.25f;
        // m_zRot += dx * 0.25f;
        // update();
    }
    m_lastMousePos = event->pos();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numDegrees.isNull()) {
        m_zoom += numDegrees.y() / 5.0f; // Zoom step
        if (m_zoom > -0.5f) m_zoom = -0.5f; // Prevent zooming too close/through
        update();
    }
    event->accept();
}