#include "bspline.h"

BSpline::BSpline(std::vector<Vec3> controlPoints, int inDegree, std::vector<float> knots)
{
    for(int i = 0; i < controlPoints.size(); i++)
    {
        b.push_back(controlPoints[i]);
    }
    degree = inDegree;
    mKnots = knots;
    mDrawType = GL_LINE_STRIP;
    createCurvePoints();

    init();
}

Vec3 BSpline::evaluateBSpline(int degree, float t)
{
    std::vector<Vec3> a;
    for(int i = 0; i < b.size(); i++)
    {
        a.push_back(b[i]);
    }
    for(int d = degree; d >= 1; d--)
    {
        int j = degree-d;
        for(int i = 0; i < d; i++)
        {
            j = j+1;
            float w = (t - mKnots[j])/(mKnots[j+d]-mKnots[j]);
            a[i] = (a[i] * (1-w) + (a[i+1] * w));
        }
    }
    return a[0];
}

void BSpline::createCurvePoints()
{
    std::vector <Vec3> v;
    mNumberOfVertices = 0;
    for(float t = 0.f; t < 1.f; t+= 0.02f)
    {
        mNumberOfVertices++;
        v.push_back(evaluateBSpline(degree, t));
    }
    mVertices = new Vertex[mNumberOfVertices];
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i].set_xyz(v[i]);
        mVertices[i].set_normal(0.f, 1.f, 0.f);
    }


}
