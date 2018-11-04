#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "constants.h"
#include "gltypes.h"
#include "componentbase.h"
#include "renderwindow.h"
#include "bspline.h"
#include <string>
#include <map>

class Cube;
class Plane;
class Octahedron;
class Terrain;
class MeshBase;
class Axis;

class MeshFactory
{
    public:
        MeshFactory();
        ~MeshFactory();

        //MeshBase* getMesh(gsl::ShapeType shape, GLint recursions = 2);
        MeshBase* getMesh(const std::string &identifier);

        MeshComponent getMeshComponent(const std::string &identifier);

        void receiveData(BSplineData inData);
        void createBSpline();
        void createNPC();
private:

        std::string getFileExtension(const std::string& filename);
        // ./path/file.png
        // cube
        // sphere
        // axis
        // usage:
        // mMeshes["./path/file.png"] = new Mesh();
        // mMeshes["cube"]
        std::map<std::string, MeshBase*> mMeshes;
        BSplineData data;
};

#endif // MESHFACTORY_H
