#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

class QSurface;
class QTextEdit;
class RenderWindow;
class CoreEngine;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void init();
    void renderWindowReady();
    void renderWindowError(const QString &msg);

    void on_actionWireframe_toggled(bool arg1);

    void on_lightIntensitySlider_sliderMoved(int position);

    void on_lightAmbientRedSlider_sliderMoved(int position);

    void on_lightAmbientGreenSlider_sliderMoved(int position);

    void on_lightAmbientBlueSlider_sliderMoved(int position);

    void on_actionAxis_toggled(bool arg1);

//    void leaveEvent(QEvent *event); //triggers when mouse leaves application

private:
    void addRenderWindow();

    void updateWorldOutliner();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
    CoreEngine *mCoreEngine;
    void setColorPreview();
    void setAmbientColor();
};

#endif // MAINWINDOW_H
