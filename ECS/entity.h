#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include "mat4.h"
#include "componentbase.h"
#include <vector>

class Transform;
class Material;
class Vertex;
class RenderWindow;
class SystemBase;

using namespace gsl;

class Entity
{
public:
    Entity();
    ~Entity();

    //OEF: should probably be in mComponents to be more flexible
    MeshComponent *mMeshComp;
    MaterialComponent *mMaterialComp;
    TransformComponent *mTransformComp;

    //OEF: Not used, so to clearify, I commented it out
//    void createObject();

    Entity *getParent() const;
    void setParent(Entity *parent);

    Transform &getTransform();

    void setMaterial(Material *material);
    Material *getMaterial();

    void setObjectName(const std::string &objectName);
    std::string getObjectName() const;

    unsigned int mEntityID;       //unique ID for every entity

    Entity* mParent{nullptr};

    std::string mObjectName;

    std::vector<ComponentBase*> mComponents;

    bool visibility = true;

private:
    static unsigned int ID;
};

#endif // ENTITY_H
