#include "entity.h"
#include "vertex.h"
#include "transform.h"
#include "material.h"
#include "shader.h"
#include "mat4.h"
#include "meshbase.h"
#include <cassert>

//init static var
unsigned int Entity::ID = 0;

Entity::Entity()
{
    mTransformComp = new TransformComponent;    //OEF: for now, to get things working
    mEntityID = ID;
    ID++;
}

Entity::~Entity()
{
    //OEF: when ECS is finished - check what has to be done here!
}

//OEF: Not used, so to clearify, I commented it out
//void Entity::createObject()
//{
//    // Init meshBase
//    mMeshComp = new MeshComponent;
//    mMeshComp->mData->init();
//    mMeshComp->mDrawType = mMeshComp->mData->mDrawType;
//    mMeshComp->mVAO = mMeshComp->mData->mVAO;
//    mMeshComp->mNumberOfVertices = mMeshComp->mData->mNumberOfVertices;
//    mMeshComp->mNumberOfIndices = mMeshComp->mData->mNumberOfIndices;
//    // Set up vertexAttribPointers
//    mMaterialComp->mMaterial->setupShader(mMeshComp->mVAO);
//}

Entity* Entity::getParent() const
{
    return mParent;
}

void Entity::setParent(Entity* parent)
{
    mParent = parent;
}

Transform& Entity::getTransform()
{
    return mTransformComp->mTransform;
}

void Entity::setMaterial(Material* material)
{
    mMaterialComp = new MaterialComponent;
    mMaterialComp->mMaterial = material;
    mMaterialComp->mMaterial->setupShader(mMeshComp->mVAO);
}

Material* Entity::getMaterial()
{
    return mMaterialComp->mMaterial;
}

void Entity::setObjectName(const std::string &objectName)
{
    mObjectName = objectName;
}

std::string Entity::getObjectName() const
{
    return mObjectName;
}
