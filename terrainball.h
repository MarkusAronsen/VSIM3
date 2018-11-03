#ifndef TERRAINBALL_H
#define TERRAINBALL_H

#include <QKeyEvent>

#include "octahedron.h"
#include "meshfactory.h"
#include "entity.h"
#include "vertex.h"
#include <vector>

struct Triangle2
{
    Triangle2(Vec3 inP, Vec3 inQ, Vec3 inR, int inPIndex, int inQIndex, int inRIndex)
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

    void print()
    {
        std::cout << p << " " << q << " " << r << std::endl;
    }

    void printNeighbors()
    {
        std::cout << uNeighbor << " " << vNeighbor << " " << wNeighbor << std::endl;
    }

    void printIndices()
    {
        std::cout << pIndex << " " << qIndex << " " << rIndex << std::endl;
    }
};

class TerrainBall : public Octahedron
{

public:
    TerrainBall(int subDivisions = 0);
    void update(GLfloat deltaTime);
    void keyboardMovement(QKeyEvent *event);
    void keyboardRelease(QKeyEvent *event);
    void getDirection();
    void setTerrain(Entity *terrain, Entity *terrainBall);
    void buildTriangles(int a);
    int barycentricSearch(int currentIndex);
    void findHeight();

private:
    Entity *mTerrain;
    Entity *mTerrainBall;
    GLfloat mMovementSpeed = 10.f;
    gsl::Vec3 mVelocity;
    std::vector <Triangle2> triangles2;
    int previousTriangle = 0;

    bool mForward{false};
    bool mBack{false};
    bool mRight{false};
    bool mLeft{false};
};

#endif // TERRAINBALL_H
