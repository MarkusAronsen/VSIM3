#ifndef COREENGINE_H
#define COREENGINE_H

#include <QBasicTimer>
#include <QTime>
#include <QObject>

#include <list>
#include <vector>

#include "entity.h"
#include "systembase.h"
#include "componentbase.h"
#include "terrainball.h"

class RenderWindow;
class MainWindow;
class RenderSystem;

class CoreEngine : public QObject
{
    Q_OBJECT
public:
    explicit CoreEngine(RenderWindow *renderWindow, MainWindow *mainWindow, QObject *parent = nullptr);

    void timerEvent(QTimerEvent *);

    void startLoop();

    std::list<Entity* > getEntities() const;

    std::list<Entity*> mEntities;            //all Entities in the program
    std::list<SystemBase*> mSystems;        //all Systems in the program
    std::list<std::vector<ComponentBase> *>mComponents; //list of vectors for each ComponentType

    RenderWindow *mRenderWindow;    //the game window
    MainWindow *mMainWindow;        //main program window

    RenderSystem *mRenderSystem;

    QBasicTimer mTimer;     //timer that drives the gameloop
    QTime mTimeStart;       //time variable that reads the actual FPS
    float mDeltaTime{0.f};  //used to show FPS in MainWindow status bar
    
signals:

public slots:

private:
    void calculateFramerate(int msSinceLastFrame);
    TerrainBall *mTerrainBall;
};

#endif // COREENGINE_H
