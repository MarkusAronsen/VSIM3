#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"
#include "quaternion.h"

class Entity;

class Transform
{
public:
    Transform(Entity *owner = nullptr);
    gsl::Vec3 &getPosition();
    gsl::Quaternion &getRotation();
    gsl::Vec3 &getScale();

    gsl::Mat4 getModelMatrix();

    void setPosition(float x, float y, float z);
    void setPosition(gsl::Vec3 pos);

    void setRotation(float x, float y, float z);
    void setRotation(gsl::Quaternion rot);

    void setScale(float x, float y, float z);
    void setScale(gsl::Vec3 scale);

    void translate(float x, float y, float z);
    void translate(gsl::Vec3 vecIn);
    void rotate(float angle_deg, gsl::Vec3 axis);
    void rotate(gsl::Quaternion quat);
    void rotate(float x, float y, float z);

private:
    Entity* mEntity{nullptr};   //a pointer to the owner of the transform

    gsl::Vec3 mLocalPosition;
    gsl::Quaternion mLocalRotation;
    gsl::Vec3 mLocalScale;
};

#endif // TRANSFORM_H
