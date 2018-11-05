#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTime>

#include <memory>   //smart pointers
#include <vector>

#include "constants.h"
#include "vec3.h"

class QOpenGLContext;
class Shader;
class MainWindow;
class Triangle;
class Octahedron;
class Entity;
class Texture;
class Matrix4x4;
class Material;
class QOpenGLDebugLogger;
class Camera;
class PointLight;
class CoreEngine;
class RenderSystem;
struct MeshComponent;
struct MaterialComponent;
class MeshFactory;
class Terrain;
class TerrainBall;
class NPC;

using namespace gsl;

struct TerrainTriangleData
{
    TerrainTriangleData(Vec3 inP, Vec3 inQ, Vec3 inR, int inPIndex, int inQIndex, int inRIndex)
    {
        p = inP;
        q = inQ;
        r = inR;

        uNeighbor = -1;
        vNeighbor = -1;
        wNeighbor = -1;

        pIndex = inPIndex;
        qIndex = inQIndex;
        rIndex = inRIndex;
    }

    Vec3 p;
    Vec3 q;
    Vec3 r;

    unsigned int pIndex;
    unsigned int qIndex;
    unsigned int rIndex;

    int uNeighbor;
    int vNeighbor;
    int wNeighbor;
};

//BSpline
struct BSplineData
{
    std::vector<Vec3> controlPoints;
    int degree;
    std::vector<float> knots;
};

class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;
    QOpenGLContext *context() { return mContext; }
    void exposeEvent(QExposeEvent *) override;

    void setWireframe(bool wireframe);

    void setShowAxis(bool showAxis);

    PointLight *getLight() const;

    Shader *getShader(gsl::ShaderTypes s);

    void setCoreEngine(CoreEngine *coreEngine);
    void checkForGLerrors();

    void preRender(float deltaTime);
    void postRender();

    void buildTerrainData();
    int barycentricSearch(Entity *a, int currentIndex);
    int barycentricSearch(Vec3 inPos, int currentIndex);
    float findHeight(Entity *a, int index);
    float findHeight(Vec3 inPos, int index);

signals:
    void ready();
    void error(const QString &msg);

private:
    void init();

    void startOpenGLDebugger();

    QOpenGLContext *mContext{nullptr};

    CoreEngine *mCoreEngine{nullptr};

    bool mInitialized{false};

    Shader* mShaders[4]{nullptr};
    Texture* mTextures[4]{nullptr};
    Material* mMaterials[4]{nullptr};

    QTime mTimeStart;       //used to calculate deltaTime

    MainWindow *mMainWindow{nullptr};

    QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    Camera* mCamera{nullptr};

    PointLight* mLight{nullptr}; //Only temporary
    Entity* mTestEntity{nullptr};
    MeshComponent *mMeshComponent{nullptr};
    MaterialComponent *mMaterialComponent{nullptr};

    MeshFactory *mMeshFactory{nullptr};

    TerrainBall *updateTerrainBall;
    Entity *terrainBallEntity;
    Entity *terrainEntity;
    std::vector<TerrainTriangleData> triangleData;
    Entity *controlPoints[4];
    BSplineData createBSplineData();
    Entity *BSpline;
    void onControlPointOverlap();
    Entity *NPCBody;
    NPC *NPCInstance;
    float NPCHeight;

    MeshComponent *meshBSpline;

    bool mWireframe{false};
    bool mShowAxis{true};
    int mouseXlast{0};
    int mouseYlast{0};
    bool mMouseControl{false};

    //float mDeltaTime{0.f};  //used for deltaTime into update functions

    int renderLoop{0};

    void makeShaders();
    void makeTextures();
    void makeMaterials();

    //for camera control
    float mHorizontalAngle{3.14f};
    float mVerticalAngle{0.0f};
    float mDistance{40.f};
    float horizontal = 0.f;
    float vertical = 0.f;

    bool mForward{false};
    bool mBackward{false};
    bool mRight{false};
    bool mLeft{false};

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
