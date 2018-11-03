#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "renderwindow.h"
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QTextEdit>
#include <QDebug>

#include "pointlight.h"
#include "entity.h"
#include "coreengine.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    //Setting up the OpenGL 4.1 surface:
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSamples(4);    // Set the number of samples used for multisampling
    format.setOption(QSurfaceFormat::DebugContext); //Should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger()
    format.setDepthBufferSize(24);      // The example rendering will need a depth buffer.
    qDebug() << "Requesting surface format" << format;

    //Making the renderwindow with this surface - testing if it went well:
    mRenderWindow = new RenderWindow(format, this);
    if (!mRenderWindow->context()) {
        qDebug() << (tr("Failed to create context"));
        //delete mRenderWindow;
        mRenderWindowContainer = new QWidget(this);
        addRenderWindow();
        return;
    }

    connect(mRenderWindow, &RenderWindow::ready, this, &MainWindow::renderWindowReady);
    connect(mRenderWindow, &RenderWindow::error, this, &MainWindow::renderWindowError);

    //Adds the renderwindow to the visual program - inside the
    // RenderWindowContainer that we can see in the form-designer.
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    addRenderWindow();

    mRenderWindowContainer->setFocus(); //sets the keyboard input focus to this window when program starts
    mCoreEngine = new CoreEngine(mRenderWindow, this);
    mRenderWindow->setCoreEngine(mCoreEngine);
}

void MainWindow::renderWindowReady()
{
    //Testing if the context is valid
    QOpenGLContext *context = QOpenGLContext::currentContext();
    Q_ASSERT(context);
    setAmbientColor();
    setColorPreview();
    updateWorldOutliner();
}

void MainWindow::renderWindowError(const QString &msg)
{
    //Generic error writing. Could easly be removed and handeled in other ways
    qDebug() << (tr("An error has occurred:\n%1").arg(msg));
}

void MainWindow::addRenderWindow()
{
    ui->renderLayout->addWidget(mRenderWindowContainer);
}

void MainWindow::updateWorldOutliner()
{
    std::list<Entity*> sceneObjects = mCoreEngine->getEntities();
    for (auto tempGameObject: sceneObjects) {
        ui->lwHierarchy->addItem(QString::fromStdString(tempGameObject->getObjectName()));
    }
}

//https://forum.qt.io/topic/61832/restrict-mouse-cursor-movement/12
//Tries to restrict the mouse to leave the program.
//void MainWindow::leaveEvent(QEvent * event)
//{
//    // Get the window geometry & cursor position
//    const QRect & rect = geometry();
//    QPoint position = QCursor::pos();

//    // Check the bounds
//    qint32 x = qBound(rect.left(), position.x(), rect.right());
//    qint32 y = qBound(rect.top(), position.y(), rect.bottom());

//    // Adjust the cursor
//    if (x != position.x() || y != position.y())
//        QCursor::setPos(x, y);

////    qDebug() << "Leaving! ************************************************************************************";
//    event->accept();
//    QMainWindow::leaveEvent(event);
//}

void MainWindow::on_actionWireframe_toggled(bool arg1)
{
    mRenderWindow->setWireframe(arg1);
}

void MainWindow::on_actionAxis_toggled(bool arg1)
{
    mRenderWindow->setShowAxis(!arg1);  //starts with true
}

void MainWindow::on_lightIntensitySlider_sliderMoved(int) // position)
{
//    mRenderWindow->getLight()->mAmbientPower = position/255.f;
//    setColorPreview();
}

void MainWindow::on_lightAmbientRedSlider_sliderMoved(int position)
{
    mRenderWindow->getLight()->mDiffuseColor.setX(position/255.f);
    setColorPreview();
}

void MainWindow::on_lightAmbientGreenSlider_sliderMoved(int position)
{
    mRenderWindow->getLight()->mDiffuseColor.setY(position/255.f);
    setColorPreview();
}

void MainWindow::on_lightAmbientBlueSlider_sliderMoved(int position)
{
    mRenderWindow->getLight()->mDiffuseColor.setZ(position/255.f);
    setColorPreview();
}

void MainWindow::setColorPreview()
{
    int red = ui->lightAmbientRedSlider->value();
    int green = ui->lightAmbientGreenSlider->value();
    int blue = ui->lightAmbientBlueSlider->value();
    int power = ui->lightIntensitySlider->value();

    //background-color: rgb(255, 0, 4)
    QString result = QString("background-color: rgb(") + QString::number(red * power/255) + QString(", ") +
    QString::number(green * power/255) + QString(", ") + QString::number(blue * power/255) + QString(")");

    ui->lightAmbientPreview->setStyleSheet(result);
}

void MainWindow::setAmbientColor()
{
    ui->lightAmbientRedSlider->setValue(static_cast<int>(255*mRenderWindow->getLight()->mDiffuseColor.getX()));
    ui->lightAmbientGreenSlider->setValue(static_cast<int>(255*mRenderWindow->getLight()->mDiffuseColor.getX()));
    ui->lightAmbientBlueSlider->setValue(static_cast<int>(255*mRenderWindow->getLight()->mDiffuseColor.getX()));
    //ui->lightIntensitySlider->setValue(static_cast<int>(255*mRenderWindow->getLight()->mAmbientPower));
}
