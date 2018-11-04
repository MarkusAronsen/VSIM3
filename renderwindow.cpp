#include "renderwindow.h"
#include <QTimer>
#include "mat4.h"
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

#include <memory>   //smart pointers

#include "shader.h"
#include "mainwindow.h"
#include "octahedron.h"
#include "terrain.h"
#include "terrainball.h"
#include "npc.h"
#include "texture.h"
#include "vertex.h"
#include "transform.h"
#include "constants.h"
#include "material.h"
#include "colormaterial.h"
#include "texturematerial.h"
#include "lightcolormaterial.h"
#include "camera.h"
#include "controlledball.h"
#include "pointlight.h"
#include "coreengine.h"

#include "componentbase.h"
#include "rendersystem.h"
#include "meshfactory.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext{nullptr}, mInitialized{false}, mMainWindow{mainWindow}

{
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    mContext = new QOpenGLContext(this);    //automatically deleted
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
    }
}

RenderWindow::~RenderWindow()
{
    //OEF: when ECS is finished - check what has to be done here!
}

void RenderWindow::init()
{
    //Some general stuff
    if (!mContext->makeCurrent(this)) {
        emit error(tr("makeCurrent() failed"));
        return;
    }

    if (!mInitialized)
        mInitialized = true;

    initializeOpenGLFunctions();
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    glEnable(GL_CULL_FACE);     //draws only front side of models
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    makeShaders();

    //make textures
    makeTextures();

    //Make directional light
    mLight = new PointLight(gsl::Vec3(6.f, 3.f, -2.f), gsl::Vec3(0.2f, 0.2f, 0.2f), gsl::Vec3(1.f, 0.f, 1.f), gsl::Vec3(0.7f, 0.7f, 0.7f) );

    //Make all materials
    makeMaterials();

    //Initialize camera
    mCamera = new Camera();

    //perspectiv matrix
    mCamera->setPerspectiveMatrix(width(), height());

    //view matrix
    mCamera->setViewMatrix();

    //Give camera to materials:
    Material::setCurrentCamera(mCamera);

    checkForGLerrors();

    mMeshFactory = new MeshFactory;

    //Test new system - this should be inside a CreateEntity-thing:
    //Axis:
    MeshComponent *testAxis = new MeshComponent(mMeshFactory->getMeshComponent("axis"));
    testAxis->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = testAxis;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("Axis");
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);

    //Terrain:
    MeshComponent *testTerrain = new MeshComponent(mMeshFactory->getMeshComponent("terrain"));
    testTerrain->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = testTerrain;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::LIGHTCOLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("Terrain");
    //    mTestEntity->getTransform().setPosition(0.f, -50.f, 0.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;

    mCoreEngine->mEntities.push_back(mTestEntity);

    terrainEntity = mCoreEngine->mEntities.back();
    buildTerrainData();

    //Patrol marker
    MeshComponent *meshCube01 = new MeshComponent(mMeshFactory->getMeshComponent("cube"));
    meshCube01->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshCube01;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("Cube");
    mTestEntity->getTransform().setPosition(10.f, findHeight(Vec3(10.f, 0.f, 10.f), barycentricSearch(Vec3(10.f, 0.f, 10.f), 0)) + 1, 10.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);

    //Patrol marker
    MeshComponent *meshCube02 = new MeshComponent(mMeshFactory->getMeshComponent("cube"));
    meshCube02->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshCube02;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("Cube");
    mTestEntity->getTransform().setPosition(90.f, findHeight(Vec3(90.f, 0.f, 40.f), barycentricSearch(Vec3(90.f, 0.f, 40.f), 0)) + 1, 40.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);

    //TerrainBall
    MeshComponent *meshTerrainBall = new MeshComponent(mMeshFactory->getMeshComponent("terrainball"));
    meshTerrainBall->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshTerrainBall;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("TerrainBall");
    mTestEntity->getTransform().setPosition(0.f, 0.f, 0.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);

    terrainBallEntity = mCoreEngine->mEntities.back();

    updateTerrainBall = new TerrainBall(0);
    updateTerrainBall->setTerrain(terrainEntity, terrainBallEntity);
    updateTerrainBall->buildTriangles(0);

    MeshComponent *meshControlPoint01 = new MeshComponent(mMeshFactory->getMeshComponent("ControlPoint"));
    meshControlPoint01->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshControlPoint01;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("ControlPoint01");
    mTestEntity->getTransform().setPosition(40.f, 0.f, 15.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);
    mTestEntity->getTransform().setPosition(mTestEntity->getTransform().getPosition().getX(),
                                            findHeight(mTestEntity, barycentricSearch(mTestEntity, 0)) + 1.f,
                                            mTestEntity->getTransform().getPosition().getZ());

    controlPoints[0] = mCoreEngine->mEntities.back();


    MeshComponent *meshControlPoint02 = new MeshComponent(mMeshFactory->getMeshComponent("ControlPoint"));
    meshControlPoint02->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshControlPoint02;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("ControlPoint02");
    mTestEntity->getTransform().setPosition(20.f, 0.f, 20.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);
    mTestEntity->getTransform().setPosition(mTestEntity->getTransform().getPosition().getX(),
                                            findHeight(mTestEntity, barycentricSearch(mTestEntity, 0)) + 1.f,
                                            mTestEntity->getTransform().getPosition().getZ());

    controlPoints[1] = mCoreEngine->mEntities.back();

    MeshComponent *meshControlPoint03 = new MeshComponent(mMeshFactory->getMeshComponent("ControlPoint"));
    meshControlPoint03->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshControlPoint03;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("ControlPoint03");
    mTestEntity->getTransform().setPosition(50.f, 0.f, 50.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);
    mTestEntity->getTransform().setPosition(mTestEntity->getTransform().getPosition().getX(),
                                            findHeight(mTestEntity, barycentricSearch(mTestEntity, 0)) + 1,
                                            mTestEntity->getTransform().getPosition().getZ());

    controlPoints[2] = mCoreEngine->mEntities.back();

    MeshComponent *meshControlPoint04 = new MeshComponent(mMeshFactory->getMeshComponent("ControlPoint"));
    meshControlPoint04->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshControlPoint04;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("ControlPoint04");
    mTestEntity->getTransform().setPosition(70.f, 0.f, 20.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);
    mTestEntity->getTransform().setPosition(mTestEntity->getTransform().getPosition().getX(),
                                            findHeight(mTestEntity, barycentricSearch(mTestEntity, 0)) + 1,
                                            mTestEntity->getTransform().getPosition().getZ());

    controlPoints[3] = mCoreEngine->mEntities.back();

    BSplineData NPCData;
    NPCData = createBSplineData();

    mMeshFactory->createBSpline();
    mMeshFactory->createNPC();

    //NPC
    MeshComponent *meshNPC = new MeshComponent(mMeshFactory->getMeshComponent("NPC"));
    meshNPC->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshNPC;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("NPC");
    mTestEntity->getTransform().setPosition(10.f, findHeight(Vec3(10.f, 0.f, 10.f), barycentricSearch(Vec3(10.f, 0.f, 10.f), 0)) + 1, 10.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);

    NPCBody = mCoreEngine->getEntities().back();
    NPCInstance = new NPC(2, NPCData.controlPoints, NPCData.degree, NPCData.knots);
    NPCInstance->setBody(NPCBody);

    //BSpline
    meshBSpline = new MeshComponent(mMeshFactory->getMeshComponent("BSpline"));
    meshBSpline->componentType = gsl::MESH;
    mTestEntity = new Entity();
    mTestEntity->mMeshComp = meshBSpline;
    mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
    checkForGLerrors();
    mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
    mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
    checkForGLerrors();
    mTestEntity->setObjectName("BSpline");
    mTestEntity->getTransform().setPosition(0.f, 0.f, 0.f);
    mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
    mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
    mCoreEngine->mEntities.push_back(mTestEntity);


    emit ready();   //tell the mainWindow that init is finished
}

///called to set up renderWindow before each frame
void RenderWindow::preRender(float deltaTime)
{   
    //    mCamera->movement(mForward, mBackward, mRight, mLeft, deltaTime);
    Vec3 camPos = Vec3(terrainBallEntity->getTransform().getPosition() + Vec3(-20.f, 20.f, -20.f));
    mCamera->getTransform().mTransform.setPosition(camPos);

    //    vertical += 0.01f;
    //    std::cout << vertical << std::endl;
    mCamera->setDirection(7.f, 12.f);

    onControlPointOverlap();

    NPCHeight = findHeight(NPCBody->getTransform().getPosition(), barycentricSearch(NPCBody->getTransform().getPosition(), 0));
    NPCInstance->Update(NPCHeight);

    updateTerrainBall->update(deltaTime);
    mLight->setPosition(terrainBallEntity->getTransform().getPosition() + Vec3(0.f, 10.f, 0.f));

    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

///called after each rendered frame
void RenderWindow::postRender()
{
    glBindVertexArray(0);   //not really needed, but good practice
    if (isExposed())    //I get a runtime warning on Mac if I don't have this test - only on first frame
        mContext->swapBuffers(this);
}

void RenderWindow::buildTerrainData()
{
    for(int i = 0; i < terrainEntity->mMeshComp->mData->mNumberOfIndices; i+= 3)
    {
        triangleData.push_back(TerrainTriangleData(terrainEntity->mMeshComp->mData->mVertices[terrainEntity->mMeshComp->mData->mIndices[i]].get_xyz(),
                               terrainEntity->mMeshComp->mData->mVertices[terrainEntity->mMeshComp->mData->mIndices[i + 1]].get_xyz(),
                terrainEntity->mMeshComp->mData->mVertices[terrainEntity->mMeshComp->mData->mIndices[i + 2]].get_xyz(),
                terrainEntity->mMeshComp->mData->mIndices[i],
                terrainEntity->mMeshComp->mData->mIndices[i + 1],
                terrainEntity->mMeshComp->mData->mIndices[i + 2]));
    };

    for(unsigned int i = 0; i < triangleData.size(); i++)
    {
        for(unsigned int j = 0; j < triangleData.size(); j++)
        {
            if(i == j)
            {
                continue;
            }
            if(triangleData[j].pIndex == triangleData[i].qIndex && triangleData[j].qIndex == triangleData[i].pIndex)
            {
                triangleData[i].wNeighbor = j;
            }
            if(triangleData[j].pIndex == triangleData[i].rIndex && triangleData[j].rIndex == triangleData[i].pIndex)
            {
                triangleData[i].vNeighbor = j;
            }
            if(triangleData[j].qIndex == triangleData[i].rIndex && triangleData[j].rIndex == triangleData[i].qIndex)
            {
                triangleData[i].uNeighbor = j;
            }
        }
    }
}

int RenderWindow::barycentricSearch(Entity *a, int currentIndex = 0)
{
    if(currentIndex < 0)
    {
        std::cout << "Location is outside terrain!" << std::endl;
        return 0;
    }
    float bc[3];
    Vec3 x[2];
    x[0] = triangleData[currentIndex].q - triangleData[currentIndex].p;
    x[0].setY(0.f);
    x[1] = triangleData[currentIndex].r - triangleData[currentIndex].p;
    x[1].setY(0.f);
    float xAreal = (x[0] ^ x[1]).length();

    Vec3 u1 = triangleData[currentIndex].q - a->getTransform().getPosition();
    u1.setY(0.f);
    Vec3 u2 = triangleData[currentIndex].r - a->getTransform().getPosition();
    u2.setY(0.f);
    //    Vec2 u1v2 = Vec2(u1.getX(), u1.getZ());
    //    Vec2 u2v2 = Vec2(u2.getX(), u2.getZ());
    bc[0] = ((u2.getX() * u1.getZ()) - (u1.getX() * u2.getZ()));

    Vec3 v1 = triangleData[currentIndex].r - a->getTransform().getPosition();
    v1.setY(0.f);
    Vec3 v2 = triangleData[currentIndex].p - a->getTransform().getPosition();
    v2.setY(0.f);
    bc[1] =  ((v2.getX() * v1.getZ()) - (v1.getX() * v2.getZ()));

    Vec3 w1 = triangleData[currentIndex].p - a->getTransform().getPosition();
    w1.setY(0.f);
    Vec3 w2 = triangleData[currentIndex].q - a->getTransform().getPosition();
    w2.setY(0.f);
    bc[2] =  ((w2.getX() * w1.getZ()) - (w1.getX() * w2.getZ()));

    bc[0] = bc[0] / xAreal;
    bc[1] = bc[1] / xAreal;
    bc[2] = bc[2] / xAreal;

    if(bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0)
    {
        return currentIndex;
    }

    else if(bc[0] < bc[1] && bc[0] < bc[2])
    {
        if(triangleData[currentIndex].uNeighbor != -1)
        {
            return barycentricSearch(a, triangleData[currentIndex].uNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }
    else if(bc[1] < bc[2])
    {
        if(triangleData[currentIndex].vNeighbor != -1)
        {
            return barycentricSearch(a, triangleData[currentIndex].vNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }
    else
    {
        if(triangleData[currentIndex].wNeighbor != -1)
        {
            return barycentricSearch(a, triangleData[currentIndex].wNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }

}

float RenderWindow::findHeight(Entity *a, int index)
{
    float x = a->getTransform().getPosition().getX();
    float y = a->getTransform().getPosition().getZ();

    float x1 = triangleData[index].p.getX();
    float x2 = triangleData[index].q.getX();
    float x3 = triangleData[index].r.getX();

    float y1 = triangleData[index].p.getZ();
    float y2 = triangleData[index].q.getZ();
    float y3 = triangleData[index].r.getZ();

    float z1 = triangleData[index].p.getY();
    float z2 = triangleData[index].q.getY();
    float z3 = triangleData[index].r.getY();

    float l1 = (((y2 - y3) * (x - x3)) + ((x3 - x2) * (y - y3))) / (((y2 - y3) * (x1 - x3)) + ((x3 - x2) * (y1 - y3)));
    float l2 = (((y3 - y1) * (x - x3)) + ((x1 - x3) * (y - y3))) / (((y2 - y3) * (x1 - x3)) + ((x3 - x2) * (y1 - y3)));
    float l3 = 1 - l1 - l2;
    return l1 * z1 + l2 * z2 + l3 * z3;
}

int RenderWindow::barycentricSearch(Vec3 inPos, int currentIndex)
{
    if(currentIndex < 0)
    {
        std::cout << "Location is outside terrain!" << std::endl;
        return 0;
    }
    float bc[3];
    Vec3 x[2];
    x[0] = triangleData[currentIndex].q - triangleData[currentIndex].p;
    x[0].setY(0.f);
    x[1] = triangleData[currentIndex].r - triangleData[currentIndex].p;
    x[1].setY(0.f);
    float xAreal = (x[0] ^ x[1]).length();

    Vec3 u1 = triangleData[currentIndex].q - inPos;
    u1.setY(0.f);
    Vec3 u2 = triangleData[currentIndex].r - inPos;
    u2.setY(0.f);
    //    Vec2 u1v2 = Vec2(u1.getX(), u1.getZ());
    //    Vec2 u2v2 = Vec2(u2.getX(), u2.getZ());
    bc[0] = ((u2.getX() * u1.getZ()) - (u1.getX() * u2.getZ()));

    Vec3 v1 = triangleData[currentIndex].r - inPos;
    v1.setY(0.f);
    Vec3 v2 = triangleData[currentIndex].p - inPos;
    v2.setY(0.f);
    bc[1] =  ((v2.getX() * v1.getZ()) - (v1.getX() * v2.getZ()));

    Vec3 w1 = triangleData[currentIndex].p - inPos;
    w1.setY(0.f);
    Vec3 w2 = triangleData[currentIndex].q - inPos;
    w2.setY(0.f);
    bc[2] =  ((w2.getX() * w1.getZ()) - (w1.getX() * w2.getZ()));

    bc[0] = bc[0] / xAreal;
    bc[1] = bc[1] / xAreal;
    bc[2] = bc[2] / xAreal;

    if(bc[0] >= 0 && bc[1] >= 0 && bc[2] >= 0)
    {
        return currentIndex;
    }

    else if(bc[0] < bc[1] && bc[0] < bc[2])
    {
        if(triangleData[currentIndex].uNeighbor != -1)
        {
            return barycentricSearch(inPos, triangleData[currentIndex].uNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }
    else if(bc[1] < bc[2])
    {
        if(triangleData[currentIndex].vNeighbor != -1)
        {
            return barycentricSearch(inPos, triangleData[currentIndex].vNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }
    else
    {
        if(triangleData[currentIndex].wNeighbor != -1)
        {
            return barycentricSearch(inPos, triangleData[currentIndex].wNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }
}

float RenderWindow::findHeight(Vec3 inPos, int index)
{
    float x = inPos.getX();
    float y = inPos.getZ();

    float x1 = triangleData[index].p.getX();
    float x2 = triangleData[index].q.getX();
    float x3 = triangleData[index].r.getX();

    float y1 = triangleData[index].p.getZ();
    float y2 = triangleData[index].q.getZ();
    float y3 = triangleData[index].r.getZ();

    float z1 = triangleData[index].p.getY();
    float z2 = triangleData[index].q.getY();
    float z3 = triangleData[index].r.getY();

    float l1 = (((y2 - y3) * (x - x3)) + ((x3 - x2) * (y - y3))) / (((y2 - y3) * (x1 - x3)) + ((x3 - x2) * (y1 - y3)));
    float l2 = (((y3 - y1) * (x - x3)) + ((x1 - x3) * (y - y3))) / (((y2 - y3) * (x1 - x3)) + ((x3 - x2) * (y1 - y3)));
    float l3 = 1 - l1 - l2;
    return l1 * z1 + l2 * z2 + l3 * z3;
}

void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        qDebug() << "Can this system run QOpenGLDebugLogger? :" << format.testOption(QSurfaceFormat::DebugContext);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);  //will be deleted automatically
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
            //Try to silence some messages that is not critical
            mOpenGLDebugLogger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::NotificationSeverity);
        }
    }
}

BSplineData RenderWindow::createBSplineData()
{

    BSplineData data;

    data.degree = -1;

    for(int i = 0; i < 4; i++)
    {
        if(controlPoints[i]->visibility)
        {
            data.degree++;
            data.controlPoints.push_back(controlPoints[i]->getTransform().getPosition());
            data.knots.insert(data.knots.begin(), 0);
            data.knots.insert(data.knots.end(), 1);
        }
    }

    data.degree += 2;
    //NPC startpunkt
    data.controlPoints.insert(data.controlPoints.begin(), Vec3(10.f, findHeight(Vec3(10.f, 0.f, 10.f), barycentricSearch(Vec3(10.f, 0.f, 10.f), 0)) + 1, 10.f));
    //NPC sluttpunkt
    data.controlPoints.insert(data.controlPoints.end(), Vec3(90.f, findHeight(Vec3(90.f, 0.f, 40.f), barycentricSearch(Vec3(90.f, 0.f, 40.f), 0)) + 1, 40.f));

    data.knots.insert(data.knots.begin(), 0);
    data.knots.insert(data.knots.end(), 1);
    data.knots.insert(data.knots.begin(), 0);
    data.knots.insert(data.knots.end(), 1);

    mMeshFactory->receiveData(data);
    return data;
}

void RenderWindow::onControlPointOverlap()
{
    for(int i = 0; i < 4; i++)
    {
        if(controlPoints[i]->visibility &&
                (controlPoints[i]->getTransform().getPosition() - terrainBallEntity->getTransform().getPosition()).length() <= 1.f)
        {
            controlPoints[i]->visibility = false;

            mCoreEngine->mEntities.back()->visibility = false;
            createBSplineData();
            mMeshFactory->createBSpline();

            NPCInstance->setItemCollected(i + 1);

            meshBSpline = new MeshComponent(mMeshFactory->getMeshComponent("BSpline"));
            meshBSpline->componentType = gsl::MESH;
            mTestEntity = new Entity();
            mTestEntity->mMeshComp = meshBSpline;
            mTestEntity->mMeshComp->ownerEntityID = mTestEntity->mEntityID;
            checkForGLerrors();
            mTestEntity->setMaterial(mMaterials[gsl::COLOR]);
            mTestEntity->mMaterialComp->ownerEntityID = mTestEntity->mEntityID;
            mTestEntity->mMaterialComp->componentType = gsl::MATERIAL;
            checkForGLerrors();
            mTestEntity->setObjectName("BSpline");
            mTestEntity->getTransform().setPosition(0.f, 0.f, 0.f);
            mTestEntity->mTransformComp->ownerEntityID = mTestEntity->mEntityID;
            mTestEntity->mTransformComp->componentType = gsl::TRANSFORM;
            mCoreEngine->mEntities.push_back(mTestEntity);

        }
    }
}

void RenderWindow::setCoreEngine(CoreEngine *coreEngine)
{
    mCoreEngine = coreEngine;
}

Shader* RenderWindow::getShader(ShaderTypes s)
{
    return mShaders[s];
}

PointLight *RenderWindow::getLight() const
{
    return mLight;
}

void RenderWindow::setShowAxis(bool showAxis)
{
    mShowAxis = showAxis;
}

void RenderWindow::setWireframe(bool wireframe)
{
    mWireframe = wireframe;
    if (mWireframe)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );    //turn on wireframe mode
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );    //turn off wireframe mode
        glEnable(GL_CULL_FACE);
    }
}

void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

//This function is called from Qt when window is exposed / shown
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    int retinaScale = static_cast<int>(devicePixelRatio());
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    mCamera->setPerspectiveMatrix(width(), height());

    if (isExposed())
    {
        mCoreEngine->startLoop();
    }
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mMouseControl)
    {
        //using mouseXYlast as deltaXY so we don't need extra variables
        mouseXlast = event->pos().x() - mouseXlast;
        mouseYlast = event->pos().y() - mouseYlast;
        //        qDebug() << "Mouse " << event->pos().x() << event->pos().y();

        //if delta is to big the movement will be jerky
        //Happens if mouse is moved much between presses.
        if (mouseXlast > 40 || mouseYlast > 40 || mouseXlast < -40 || mouseYlast < -40)
        {
            mouseXlast = 0;
            mouseYlast = 0;
        }
        //        qDebug() << mouseXlast << mouseYlast;

        // Rotate camera
        float cameraSpeed{0.001f};

        mHorizontalAngle += mouseXlast * cameraSpeed;
        mVerticalAngle   += mouseYlast * cameraSpeed;

        //        qDebug() << "Angle: " << mHorizontalAngle << mVerticalAngle;

        mCamera->setDirection(mHorizontalAngle, mVerticalAngle);

        mouseXlast = event->pos().x();
        mouseYlast = event->pos().y();

        //        if(mouseXlast > width() )
        //            mouseXlast = 0;
        //        if(mouseXlast < 0)
        //            mouseXlast = width();
        //        if(mouseYlast > height() )
        //            mouseYlast = 0;
        //        if(mouseYlast < 0)
        //            mouseYlast = height();

        //        QCursor c = cursor();
        //        c.setPos(mouseXlast, mouseYlast);
        //        setCursor(c);
    }
}

void RenderWindow::wheelEvent(QWheelEvent* event)
{
    //http://doc.qt.io/qt-4.8/qwheelevent.html
    int numDegrees = event->delta() / 8;
    float numSteps = numDegrees / 15;

    mDistance += numSteps;

    //    if (event->orientation() == Qt::Horizontal) {
    //        mCamera->getViewMatrix()->translate(numSteps, 0.0, 0.0);
    //    } else {
    //        mCamera->getViewMatrix()->translate(0.0, 0.0, numSteps);
    //    }
    event->accept();
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{

    //    static_cast <TerrainBall*> (mTerrainBall) ->keyboardMovement(event);

    updateTerrainBall->keyboardMovement(event);

    if(event->key() == Qt::Key_Escape) //shuts down whole program
    {
        mMainWindow->close();
    }
    if(event->key() == Qt::Key_Space)
    {
        mCoreEngine->mEntities.back()->visibility = false;
    }

    if(event->key() == Qt::Key_W)
    {
        mForward = true;
    }
    if(event->key() == Qt::Key_S)
    {
        mBackward = true;
    }
    if(event->key() == Qt::Key_D)
    {
        mRight = true;
    }
    if(event->key() == Qt::Key_A)
    {
        mLeft = true;
    }

}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{

    //    mControlledBall->keyboardRelease(event);

    updateTerrainBall->keyboardRelease(event);

    if(event->key() == Qt::Key_Space)
    {
        mCoreEngine->mEntities.back()->visibility = true;
    }

    if(event->key() == Qt::Key_W)
    {
        mForward = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mBackward = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mRight = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mLeft = false;
    }
}

void RenderWindow::makeShaders()
{
    mShaders[gsl::COLOR] = new Shader("plainvertex.vert", "plainfragment.frag");
    mShaders[gsl::TEXTURE] = new Shader("textureshader.vert", "textureshader.frag");
    mShaders[gsl::LIGHTCOLOR] = new Shader("lightcolor.vert", "lightcolor.frag");
}

void RenderWindow::makeTextures()
{
    mTextures[0] = new Texture("hund.bmp");
    mTextures[1] = new Texture("uvtemplate.bmp");
}

void RenderWindow::makeMaterials()
{
    mMaterials[gsl::COLOR] = new ColorMaterial(mShaders[gsl::COLOR]);
    checkForGLerrors();

    //make Texture material
    TextureMaterial* texMat = new TextureMaterial(mShaders[gsl::TEXTURE]);
    mMaterials[gsl::TEXTURE] = texMat;
    texMat->setTexture1(mTextures[0]);
    texMat->setTexture2(mTextures[1]);
    checkForGLerrors();

    //make LightColor material
    LightColorMaterial* lightColorMat = new LightColorMaterial(mShaders[gsl::LIGHTCOLOR], mLight);
    mMaterials[gsl::LIGHTCOLOR] = lightColorMat;
    lightColorMat->setColors(gsl::Vec3(0.2f, 0.2f, 0.2f), gsl::Vec3(0.1f, 0.8f, 1.f), gsl::Vec3(1.f, 0.f, 0.f), 64.f);
    checkForGLerrors();
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        //        QCursor c = cursor();
        //        c.setPos(width() / 2, height() / 2);
        //        //c.setShape(Qt::BlankCursor);
        //        setCursor(c);
        mMouseControl = true;
    }
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        setCursor(Qt::ArrowCursor);
        mMouseControl = false;
    }
}
