#include "terrainball.h"

TerrainBall::TerrainBall(int subDivisions) : Octahedron (subDivisions)
{
}

void TerrainBall::update(GLfloat deltaTime)
{
    getDirection(); //updates mVelocity to correct direction

    mVelocity = mVelocity * deltaTime * mMovementSpeed;

    mTerrainBall->getTransform().setPosition(mTerrainBall->getTransform().getPosition() + mVelocity);

    previousTriangle = barycentricSearch(previousTriangle);
    findHeight();
}

void TerrainBall::keyboardMovement(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
    {
        mForward = true;
    }
    if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down)
    {
        mBack = true;
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
    {
        mLeft = true;
    }
    if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
    {
        mRight = true;
    }
}

void TerrainBall::keyboardRelease(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
    {
        mForward = false;
    }
    if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down)
    {
        mBack = false;
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
    {
        mLeft = false;
    }
    if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
    {
        mRight = false;
    }
}

void TerrainBall::getDirection()
{
    GLint x{0},y{0},z{0};
    if(mForward)
    {
        z = -1.f;
        x = 0.f;    //to garantee only one direction at a time
    }
    if(mBack)
    {
        z = 1.f;
        x = 0.f;
    }
    if(mRight)
    {
        x = 1.f;
        z = 0.f;
    }
    if(mLeft)
    {
        x = -1.f;
        z = 0.f;
    }
    mVelocity = gsl::Vec3(x,y,z).normalized();
}

void TerrainBall::setTerrain(Entity *terrain, Entity *terrainBall)
{
    mTerrain = terrain;
    mTerrainBall = terrainBall;
}

void TerrainBall::buildTriangles(int a)
{
    for(int i = 0; i < mTerrain->mMeshComp->mData->mNumberOfIndices; i+= 3)
    {
        triangles2.push_back(Triangle2 (mTerrain->mMeshComp->mData->mVertices[mTerrain->mMeshComp->mData->mIndices[i]].get_xyz(),
                                        mTerrain->mMeshComp->mData->mVertices[mTerrain->mMeshComp->mData->mIndices[i + 1]].get_xyz(),
                                        mTerrain->mMeshComp->mData->mVertices[mTerrain->mMeshComp->mData->mIndices[i + 2]].get_xyz(),
                                        mTerrain->mMeshComp->mData->mIndices[i],
                                        mTerrain->mMeshComp->mData->mIndices[i + 1],
                                        mTerrain->mMeshComp->mData->mIndices[i + 2]));
    };

    for(unsigned int i = 0; i < triangles2.size(); i++)
    {
        for(unsigned int j = 0; j < triangles2.size(); j++)
        {
            if(i == j)
            {
                continue;
            }
            if(triangles2[j].pIndex == triangles2[i].qIndex && triangles2[j].qIndex == triangles2[i].pIndex)
            {
                triangles2[i].wNeighbor = j;
            }
            if(triangles2[j].pIndex == triangles2[i].rIndex && triangles2[j].rIndex == triangles2[i].pIndex)
            {
                triangles2[i].vNeighbor = j;
            }
            if(triangles2[j].qIndex == triangles2[i].rIndex && triangles2[j].rIndex == triangles2[i].qIndex)
            {
                triangles2[i].uNeighbor = j;
            }
        }
    }

//    for(int i = 0; i < triangles2.size(); i++)
    //        triangles2[i].printNeighbors();
}

int TerrainBall::barycentricSearch(int currentIndex)
{
    if(currentIndex < 0)
    {
        std::cout << "Location is outside terrain!" << std::endl;
        return 0;
    }
    float bc[3];
    Vec3 x[2];
    x[0] = triangles2[currentIndex].q - triangles2[currentIndex].p;
    x[0].setY(0.f);
    x[1] = triangles2[currentIndex].r - triangles2[currentIndex].p;
    x[1].setY(0.f);
    float xAreal = (x[0] ^ x[1]).length();

    Vec3 u1 = triangles2[currentIndex].q - mTerrainBall->getTransform().getPosition();
    u1.setY(0.f);
    Vec3 u2 = triangles2[currentIndex].r - mTerrainBall->getTransform().getPosition();
    u2.setY(0.f);
//    Vec2 u1v2 = Vec2(u1.getX(), u1.getZ());
//    Vec2 u2v2 = Vec2(u2.getX(), u2.getZ());
    bc[0] = ((u2.getX() * u1.getZ()) - (u1.getX() * u2.getZ()));

    Vec3 v1 = triangles2[currentIndex].r - mTerrainBall->getTransform().getPosition();
    v1.setY(0.f);
    Vec3 v2 = triangles2[currentIndex].p - mTerrainBall->getTransform().getPosition();
    v2.setY(0.f);
    bc[1] =  ((v2.getX() * v1.getZ()) - (v1.getX() * v2.getZ()));

    Vec3 w1 = triangles2[currentIndex].p - mTerrainBall->getTransform().getPosition();
    w1.setY(0.f);
    Vec3 w2 = triangles2[currentIndex].q - mTerrainBall->getTransform().getPosition();
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
        if(triangles2[currentIndex].uNeighbor != -1)
        {
            return barycentricSearch(triangles2[currentIndex].uNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }
    else if(bc[1] < bc[2])
    {
        if(triangles2[currentIndex].vNeighbor != -1)
        {
            return barycentricSearch(triangles2[currentIndex].vNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }
    else
    {
        if(triangles2[currentIndex].wNeighbor != -1)
        {
            return barycentricSearch(triangles2[currentIndex].wNeighbor);
        }
        else
        {
            std::cout << "Location is outside terrain!" << std::endl;
            return 0;
        }
    }

}

void TerrainBall::findHeight()
{
    float x = mTerrainBall->getTransform().getPosition().getX();
    float y = mTerrainBall->getTransform().getPosition().getZ();

    float x1 = triangles2[previousTriangle].p.getX();
    float x2 = triangles2[previousTriangle].q.getX();
    float x3 = triangles2[previousTriangle].r.getX();

    float y1 = triangles2[previousTriangle].p.getZ();
    float y2 = triangles2[previousTriangle].q.getZ();
    float y3 = triangles2[previousTriangle].r.getZ();

    float z1 = triangles2[previousTriangle].p.getY();
    float z2 = triangles2[previousTriangle].q.getY();
    float z3 = triangles2[previousTriangle].r.getY();



    float l1 = (((y2 - y3) * (x - x3)) + ((x3 - x2) * (y - y3))) / (((y2 - y3) * (x1 - x3)) + ((x3 - x2) * (y1 - y3)));
    float l2 = (((y3 - y1) * (x - x3)) + ((x1 - x3) * (y - y3))) / (((y2 - y3) * (x1 - x3)) + ((x3 - x2) * (y1 - y3)));
    float l3 = 1 - l1 - l2;
    float height = l1 * z1 + l2 * z2 + l3 * z3;

    Vec3 ballPosition = Vec3(mTerrainBall->getTransform().getPosition().getX(), height + 1.f, mTerrainBall->getTransform().getPosition().getZ());
    mTerrainBall->getTransform().setPosition(ballPosition);
}


