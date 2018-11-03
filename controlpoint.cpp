#include "controlpoint.h"

ControlPoint::ControlPoint(int subDivisions) : Octahedron(subDivisions)
{
    mRadius = 1.f;
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i].set_normal(1.f, 0.f, 0.f);
    }
}
