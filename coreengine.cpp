#include "coreengine.h"
#include <QStatusBar>
#include "renderwindow.h"
#include "mainwindow.h"
#include "rendersystem.h"

CoreEngine::CoreEngine(RenderWindow *renderWindow, MainWindow *mainWindow, QObject *parent)
    : QObject(parent), mRenderWindow(renderWindow), mMainWindow(mainWindow)
{
    mRenderSystem = new RenderSystem(renderWindow);
}

///The main game loop for the whole program
void CoreEngine::timerEvent(QTimerEvent *)
{
    int msSinceLastFrame = mTimeStart.restart();    //restart() returns ms since last restart.
    mDeltaTime = msSinceLastFrame/ 1000.f;

    //The render part of the loop
    //The camera still belongs to RenderWindow - so it needs deltaTime
    mRenderWindow->preRender(mDeltaTime);  //This just sets up the rendering
    for (auto entity : mEntities)
    {
        if(entity->visibility == true)
        {
            mRenderSystem->drawMesh(entity->mMeshComp, entity->mMaterialComp, entity->mTransformComp);
        }
    }
    mRenderWindow->postRender();

    //The rest here is just to show the frame rate:

    calculateFramerate(msSinceLastFrame);
}

void CoreEngine::startLoop()
{
    mTimer.start(16, this);
    mTimeStart.start();
}

std::list<Entity *> CoreEngine::getEntities() const
{
    return mEntities;
}

void CoreEngine::calculateFramerate(int msSinceLastFrame)
{
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 60) //once pr 60 frames =  update the message once pr second
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" FrameDraw: " + QString::number(msSinceLastFrame) + " ms");
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}
