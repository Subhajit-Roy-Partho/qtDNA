#include "mainwindow.h"
// #include "ui_mainwindow.h" // If using .ui file
#include "openglwidget.h" // Make sure to include your OpenGL widget header

#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QMenuBar> // For menu bar action

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow) // If using .ui file
{
    // ui->setupUi(this); // If using .ui file

    // Create UI programmatically if not using .ui file
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_openGLWidget = new OpenGLWidget(this);
    layout->addWidget(m_openGLWidget, 1); // OpenGL widget takes most space

    // --- Option 1: Using a QAction in a menu bar ---
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openAction = new QAction(tr("&Open Data File..."), this);
    fileMenu->addAction(openAction);
    connect(openAction, &QAction::triggered, this, &MainWindow::on_actionOpenFile_triggered);
    // --- End Option 1 ---

    // --- Option 2: Using a QPushButton ---
    // QPushButton *loadButton = new QPushButton("Load Data File", this);
    // layout->addWidget(loadButton);
    // connect(loadButton, &QPushButton::clicked, this, &MainWindow::on_actionOpenFile_triggered);
    // --- End Option 2 ---


    setCentralWidget(centralWidget);
    resize(800, 600);
    setWindowTitle("Sphere Viewer");
}

MainWindow::~MainWindow()
{
    // delete ui; // If using .ui file
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Data File"), "", tr("Data Files (*.dat *.txt);;All Files (*)"));

    if (filePath.isEmpty()) {
        return;
    }

    QVector<QVector3D> spherePoints;
    if (parseDataFile(filePath, spherePoints)) {
        m_openGLWidget->loadData(spherePoints);
    } else {
        QMessageBox::warning(this, "Error", "Could not parse the data file.");
    }
}

bool MainWindow::parseDataFile(const QString& filePath, QVector<QVector3D>& points)
{
    points.clear();
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file:" << filePath << file.errorString();
        return false;
    }

    QTextStream in(&file);
    int lineCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        lineCount++;

        if (lineCount <= 3) { // Skip first 3 lines
            continue;
        }

        if (line.isEmpty()) { // Skip empty lines
            continue;
        }

        QStringList parts = line.split(' ', Qt::SkipEmptyParts); // Use Qt::SkipEmptyParts
                                                                 // or QString::simplified() on line

        if (parts.size() >= 3) {
            bool okX, okY, okZ;
            float x = parts[0].toFloat(&okX);
            float y = parts[1].toFloat(&okY);
            float z = parts[2].toFloat(&okZ);

            if (okX && okY && okZ) {
                points.append(QVector3D(x, y, z));
            } else {
                qWarning() << "Skipping malformed line (not 3 valid floats at start):" << line;
            }
        } else {
             qWarning() << "Skipping malformed line (less than 3 parts):" << line;
        }
    }

    file.close();
    qDebug() << "Loaded" << points.size() << "sphere positions.";
    return !points.isEmpty(); // Consider it a success if at least one point was loaded
}