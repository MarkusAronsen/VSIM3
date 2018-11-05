#include "npc.h"

NPC::NPC(int subDivisions, std::vector<Vec3> inControlPoints, int inDegree, std::vector<float> inKnots) : Octahedron (subDivisions)
{
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i].set_normal(1.f, 1.f, 0.f);
    }

    state = LEARN;
    for(int i = 0; i < inControlPoints.size(); i++)
    {
        controlPoints.push_back(inControlPoints[i]);
    }
    degree = inDegree;
    for(int i = 0; i < inKnots.size(); i++)
    {
        knots.push_back(inKnots[i]);
    }
    t = 0;
    direction = true;
    itemCollected = false;
    collectedItems = 0;

    for(int i = 0; i < controlPoints.size(); i++)
    {
        tempControlPoints.push_back(controlPoints[i]);
    }

}

void NPC::Update(float height)
{
    if(controlPoints.size() == 2)
        state = STOP;

    switch(state)
    {
    case PATROL:

        body->getTransform().setPosition(evaluateBSpline().getX(), height + 1.f, evaluateBSpline().getZ());

        if(t < 1.f && direction)
        {
            t = t + 0.001f;
            if(t >= 1.f)
            {
                direction = false;
                endPointArrived = true;
                state = LEARN;
            }
        }
        else if(t > 0.f && !direction)
        {
            t = t - 0.001f;
            if(t <= 0.f)
            {
                direction = true;
                endPointArrived = true;
                state = LEARN;
            }
        }
        break;

    case LEARN:

        if(itemCollected)
        {
            updateBSpline();
            itemCollected = false;
        }
        state = PATROL;

        break;

       case STOP:
        break;

    }
}

void NPC::setBody(Entity *e)
{
    body = e;
}

void NPC::setItemCollected()
{
    itemCollected = true;
//    collectedItems++;
//    missingItems.push_back(controlPointIndex);

//    for(int i = 0; i < missingItems.size(); i++)
//        std::cout << missingItems[i] << std::endl;
}

void NPC::updateBSpline()
{
    std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
    controlPoints.clear();
    knots.clear();
    for(int i = 0; i < data.controlPoints.size(); i++)
    {
        controlPoints.push_back(data.controlPoints[i]);
    }
    for(int i = 0; i < data.knots.size(); i++)
    {
        knots.push_back(data.knots[i]);
    }
    degree = data.degree;

}

void NPC::receiveData(BSplineData a)
{
    data.degree = a.degree;
    data.controlPoints.clear();
    data.knots.clear();
    for(int i = 0; i < a.controlPoints.size(); i++)
    {
        data.controlPoints.push_back(a.controlPoints[i]);
    }
    for(int i = 0; i < a.knots.size(); i++)
    {
        data.knots.push_back(a.knots[i]);
    }
}

Vec3 NPC::evaluateBSpline()
{

    std::vector<Vec3> a;
    for(int i = 0; i < controlPoints.size(); i++)
    {
        a.push_back(controlPoints[i]);
    }
    for(int d = degree; d >= 1; d--)
    {
        int j = degree-d;
        for(int i = 0; i < d; i++)
        {
            j = j+1;
            float w = (t - knots[j])/(knots[j+d]-knots[j]);
            a[i] = (a[i] * (1-w) + (a[i+1] * w));
        }
    }
    return a[0];

}
