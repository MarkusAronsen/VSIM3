#ifndef NPC_H
#define NPC_H

#include "octahedron.h"
#include "vec3.h"
#include "vertex.h"
#include "entity.h"
#include "renderwindow.h"
#include <algorithm>

using namespace gsl;

enum State
{
    PATROL,
    LEARN,
    CHASE,
    STOP
};

class NPC : public Octahedron
{
private:
    Vec3 startPoint;
    Vec3 endPoint;
    int state;
    int degree;
    std::vector<float> knots;
    std::vector<Vec3> controlPoints;
    std::vector<Vec3> tempControlPoints;
    float t;
    bool endPointArrived;
    bool direction;
    bool itemCollected;
    int collectedItems;
    std::vector<int> missingItems;
    Entity *body;
    BSplineData data;

public:
    NPC(int subDivisions, std::vector<Vec3> controlPoints, int degree, std::vector<float> knots);
    void Update(float height);
    void setBody(Entity *e);
    void setItemCollected();
    void updateBSpline();
    void receiveData(BSplineData a);
    Vec3 evaluateBSpline();
};

#endif // NPC_H
