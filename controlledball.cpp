#include "controlledball.h"
#include "octahedron.h"
#include <QKeyEvent>
#include "transform.h"
#include <QDebug>
#include "octahedron.h"
#include "mat4.h"

ControlledBall::ControlledBall(int subDivisions) : Octahedron(subDivisions)
{
    mRadius = 1.f;
    mVelocity = gsl::Vec3(0.f,0.f,0.f);
    mMovementSpeed = 2.f;
}


void ControlledBall::update(GLfloat deltaTime)
{
    getDirection(); //updates mVelocity to correct direction

    gsl::Vec3 rotationAxis = mVelocity ^ gsl::Vec3(0.f,1.f,0.f);
    rotationAxis.normalize();

    mVelocity = mVelocity * deltaTime * mMovementSpeed;

    //OEF: this is broken now because of restructuring into ECS
//    float theta = mVelocity.length() / mRadius * (180/M_PI);
//    mTransform.mTransform->rotate(-(rotationAxis.getX()*theta),-(rotationAxis.getY()*theta),-(rotationAxis.getZ()*theta));
//    mTransform.mTransform->setPosition(getTransform()->getPosition() + mVelocity);
}

void ControlledBall::getDirection()
{
    GLint x{0},y{0},z{0};
    if(mForward)
    {
        z = -1.f;
        x = 0.f;    //to garantee only one direction at a time
    }
    if(mBack)
    {
        z = 1.f;
        x = 0.f;
    }
    if(mRight)
    {
        x = 1.f;
        z = 0.f;
    }
    if(mLeft)
    {
        x = -1.f;
        z = 0.f;
    }
    mVelocity = gsl::Vec3(x,y,z).normalized();
}

void ControlledBall::keyboardMovement(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
    {
        mForward = true;
    }
    if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down)
    {
        mBack = true;
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
    {
        mLeft = true;
    }
    if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
    {
        mRight = true;
    }
}

void ControlledBall::keyboardRelease(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
    {
        mForward = false;
    }
    if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down)
    {
        mBack = false;
    }
    if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
    {
        mLeft = false;
    }
    if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
    {
        mRight = false;
    }
}

