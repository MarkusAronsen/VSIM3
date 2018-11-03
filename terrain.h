#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "vec3.h"
#include "meshbase.h"
#include "vertex.h"

using namespace gsl;

struct Triangle
{
    Triangle(Vec3 inP, Vec3 inQ, Vec3 inR){p = inP; q = inQ; r = inR;}
    Vec3 p;
    Vec3 q;
    Vec3 r;
    Vec3 normal;
};

class Terrain : public MeshBase
{
public:
    Terrain(std::string fileName);
    void readFile(std::string fileName);
    void findFrame();
    void centralizeData();
    void generateVertices();
    void generateIndices();
    void setHeights();
    void setNormals();
    float findLocalVertices(int index, float radius);
private:
    int xMin, xMax, yMin, yMax;
    std::vector<Vec3> inData;
//    std::vector <Triangle> triangles;
};

#endif // TERRAIN_H
