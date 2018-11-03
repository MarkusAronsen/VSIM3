#ifndef OCTAHEDRON_H
#define OCTAHEDRON_H

#include "meshbase.h"
#include "vec3.h"

class Octahedron : public MeshBase
{
public:
    Octahedron(int n = 0);
    int getRecursions() const {return mRecursions;}

private:
    void makeTriangle(const gsl::Vec3& v1, const gsl::Vec3& v2, const gsl::Vec3& v3);
    void subDivide(const gsl::Vec3& a, const gsl::Vec3& b, const gsl::Vec3& c, int n);
    void makeUnitOctahedron();

    int mRecursions;          //
    int mIndex;               // brukes i rekursjon, til å bygge m_vertices
};

#endif // OCTAHEDRON_H
