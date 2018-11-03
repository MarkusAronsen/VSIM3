#ifndef OBJMESH_H
#define OBJMESH_H

#include "meshbase.h"
#include <string>

class ObjMesh : public MeshBase
{
    public:
        ObjMesh(std::string filePath);

    private:
        void readObjFile();

        std::string mFilePath;
};

#endif // OBJMESH_H
