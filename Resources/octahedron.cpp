#include "octahedron.h"
#include "cmath"
#include "vertex.h"
#include "vec3.h"
#include "material.h"
#include "mat4.h"

Octahedron::Octahedron(int n)
    : mRecursions(n), mIndex(0)
{
    mDrawType = GL_TRIANGLES;

    mNumberOfVertices = static_cast<GLuint>(3 * 8 * pow(4, mRecursions));
    mVertices = new Vertex[mNumberOfVertices];
    makeUnitOctahedron();
    init();
}

void Octahedron::makeTriangle(const gsl::Vec3 &v1, const gsl::Vec3 &v2, const gsl::Vec3 &v3)
{
    mVertices[mIndex].set_xyz(v1.getX(), v1.getY(), v1.getZ());
    mVertices[mIndex].set_normal(v1.getX(), v1.getY(), v1.getZ());
    mVertices[mIndex].set_st(0.0f, 0.0f);
    ++mIndex;
    mVertices[mIndex].set_xyz(v2.getX(), v2.getY(), v2.getZ());
    mVertices[mIndex].set_normal(v2.getX(), v2.getY(), v2.getZ());
    mVertices[mIndex].set_st(1.0f, 0.0f);
    ++mIndex;
    mVertices[mIndex].set_xyz(v3.getX(), v3.getY(), v3.getZ());
    mVertices[mIndex].set_normal(v3.getX(), v3.getY(), v3.getZ());
    mVertices[mIndex].set_st(0.5f, 1.0f);
    ++mIndex;
}

void Octahedron::subDivide(const gsl::Vec3 &a, const gsl::Vec3 &b, const gsl::Vec3 &c, int n)
{
    if (n>0) {
        gsl::Vec3 v1 = a+b; v1.normalize();
        gsl::Vec3 v2 = a+c; v2.normalize();
        gsl::Vec3 v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    } else {
        makeTriangle(a, b, c);
    }
}

void Octahedron::makeUnitOctahedron()
{
    gsl::Vec3 v0(0, 0, 1);
    gsl::Vec3 v1(1, 0, 0);
    gsl::Vec3 v2(0, 1, 0);
    gsl::Vec3 v3(-1, 0, 0);
    gsl::Vec3 v4(0, -1, 0);
    gsl::Vec3 v5(0, 0, -1);

    subDivide(v0, v1, v2, mRecursions);
    subDivide(v0, v2, v3, mRecursions);
    subDivide(v0, v3, v4, mRecursions);
    subDivide(v0, v4, v1, mRecursions);
    subDivide(v5, v2, v1, mRecursions);
    subDivide(v5, v3, v2, mRecursions);
    subDivide(v5, v4, v3, mRecursions);
    subDivide(v5, v1, v4, mRecursions);
}
