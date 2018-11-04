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

    }
}

void NPC::setBody(Entity *e)
{
    body = e;
}

void NPC::setItemCollected(int controlPointIndex)
{
    itemCollected = true;
    collectedItems++;
    missingItems.push_back(controlPointIndex);

    for(int i = 0; i < missingItems.size(); i++)
        std::cout << missingItems[i] << std::endl;
}

void NPC::updateBSpline()
{
    controlPoints.clear();
    for(int i = 0; i < 4; i++)
    {
        if(std::find(missingItems.begin(), missingItems.end(), i) != missingItems.end())
        {
            //nothing
        }
        else
        {
            std::cout << i << std::endl;
            if(i != 0)
            {
                controlPoints.push_back(tempControlPoints[i]);
            }
        }
    }

    controlPoints.insert(controlPoints.begin(), tempControlPoints[0]);
    controlPoints.insert(controlPoints.end(), tempControlPoints[5]);

    for(int i = 0; i < missingItems.size(); i++)
    {
        knots.erase(knots.begin());
        knots.pop_back();
        degree--;
    }
    missingItems.clear();

    for(int i = 0; i < controlPoints.size(); i++)
    {
        std::cout << controlPoints[i] << std::endl;
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
