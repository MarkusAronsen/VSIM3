#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include "gltypes.h"
#include "transform.h"

class Material;
class Transform;
class Vertex;
class MeshBase;

static unsigned int ID = 0;

struct ComponentBase
{
    ComponentBase()
    {
        mComponentID = ID;
        ID++;
    }
    int componentType{0};
    unsigned int ownerEntityID{0};
    unsigned int mComponentID;
};


struct MeshComponent : ComponentBase
{
    MeshBase* mData{nullptr};
    GLenum mDrawType{0};           //the way this object should be rendered
    GLuint mNumberOfVertices{0};
    GLuint mNumberOfIndices{0};
    GLuint mVAO{0};
};

struct MaterialComponent : ComponentBase
{
    Material* mMaterial{nullptr};   //should probably just be an int into the Material array.
};

struct TransformComponent : ComponentBase
{
    Transform mTransform;
};

#endif // COMPONENTBASE_H
