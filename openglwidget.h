#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector>
#include <QMouseEvent>
#include <QWheelEvent>

// Forward declaration for GLUquadric to avoid including full GL/glu.h in header
// if you prefer. Otherwise, include <GL/glu.h> or <OpenGL/glu.h>
struct GLUquadric;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() override;

    void loadData(const QVector<QVector3D>& points);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QVector<QVector3D> m_spherePositions;
    GLUquadric* m_quadric; // For drawing spheres with GLU

    float m_xRot = 0.0f;
    float m_yRot = 0.0f;
    float m_zRot = 0.0f; // Could add z-rotation if needed
    float m_zoom = -10.0f; // Initial distance from origin
    QPoint m_lastMousePos;

    float m_sphereRadius = 0.1f; // Default radius for spheres
};

#endif // OPENGLWIDGET_H