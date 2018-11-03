#include "transform.h"
#include "entity.h"
#include "vec3.h"

Transform::Transform(Entity* owner) : mLocalScale{1.f, 1.f, 1.f}
{
    mEntity = owner;
}

gsl::Vec3& Transform::getPosition()
{
    return mLocalPosition;
}

gsl::Quaternion &Transform::getRotation()
{
    return mLocalRotation;
}

gsl::Vec3& Transform::getScale()
{
    return mLocalScale;
}

Mat4 Transform::getModelMatrix()
{
    // Rotate, then scale, then translate
    Mat4 model(true);
    //model.rotate(mLocalRotation);
    //model.scale(mLocalScale.getX(), mLocalScale.getY(), mLocalScale.getZ());
    model.translate(mLocalPosition.getX(), mLocalPosition.getY(), mLocalPosition.getZ());
    model.scale(mLocalScale);
    model.rotate(mLocalRotation);
    return model;
}

void Transform::setPosition(float x, float y, float z)
{
    mLocalPosition = gsl::Vec3(x, y, z);
}

void Transform::setPosition(gsl::Vec3 pos) { mLocalPosition = pos; }

void Transform::setRotation(float x, float y, float z)
{
    mLocalRotation = gsl::Quaternion(x, y, z);
}

void Transform::setRotation(gsl::Quaternion rot) { mLocalRotation = rot; }

void Transform::setScale(float x, float y, float z)
{
    mLocalScale = gsl::Vec3(x, y, z);
}

void Transform::setScale(gsl::Vec3 scale) { mLocalScale = scale; }

void Transform::translate(float x, float y, float z)
{
    mLocalPosition = mLocalPosition + gsl::Vec3(x, y, z);
}

void Transform::translate(gsl::Vec3 vecIn)
{
    mLocalPosition = mLocalPosition + vecIn;
}

void Transform::rotate(float angle_deg, Vec3 axis)
{
    mLocalRotation = Quaternion(angle_deg, axis) * mLocalRotation;
}

void Transform::rotate(Quaternion quat)
{
    mLocalRotation = quat * mLocalRotation;
}

void Transform::rotate(float x, float y, float z)
{
    mLocalRotation = Quaternion(x, y, z) * mLocalRotation;
}
