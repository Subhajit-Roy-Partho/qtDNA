#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector3D>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class OpenGLWidget; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenFile_triggered(); // Or a button click slot

private:
    Ui::MainWindow *ui; // If using .ui file
    OpenGLWidget *m_openGLWidget;

    bool parseDataFile(const QString& filePath, QVector<QVector3D>& points);
};
#endif // MAINWINDOW_H