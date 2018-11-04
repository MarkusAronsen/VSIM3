#ifndef BSPLINE_H
#define BSPLINE_H

#include "meshbase.h"
#include "vec3.h"
#include <vector>
#include "vertex.h"

using namespace gsl;

class BSpline : public MeshBase
{
public:
    BSpline(std::vector<Vec3> controlPoints, int inDegree, std::vector<float> knots);

    Vec3 evaluateBSpline(int degree, float t);
    void createCurvePoints();

private:

    std::vector<Vec3> b;

    uint degree;

    std::vector<float> mKnots;
};

#endif
