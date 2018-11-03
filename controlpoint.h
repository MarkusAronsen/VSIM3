#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include "octahedron.h"
#include "vec3.h"
#include "vertex.h"

using namespace gsl;

class ControlPoint : public Octahedron
{
private:
    float mRadius;
public:
    ControlPoint(int);
};

#endif // CONTROLPOINT_H
