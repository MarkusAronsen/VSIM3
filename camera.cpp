#include "camera.h"
#include "mat4.h"

#include "vec3.h"
#include "transform.h"
#include <cmath>
#include <QDebug>

using namespace gsl;

Camera::Camera(int width, int height)
    : mWidth{width}, mHeight{height}
{
    //Initialize matrixes
    mViewMatrix = new Mat4(true);

    mPerspectiveMatrix = new Mat4;
}

void Camera::setPerspectiveMatrix(int width, int height)
{
    mWidth = width;
    mHeight = height;
    mPerspectiveMatrix->setToIdentity();
    //mOrtho = true;
    if (mOrtho)
    {
        float scale{0.005f};
        mPerspectiveMatrix->ortho(-mWidth*scale/2, mWidth*scale/2, -mHeight*scale/2 , mHeight*scale/2, mNearPlane, mFarPlane);
    }
    else
    {
        gsl::GLfloat aspectRatio = static_cast<float>(mWidth)/static_cast<float>(mHeight ? mHeight : 1); //height never == 0
        mPerspectiveMatrix->perspective(mFieldOfView, aspectRatio, mNearPlane, mFarPlane);
    }
}

void Camera::setViewMatrix()
{
    //std::shared_ptr<Matrix4x4> View = std::shared_ptr<Matrix4x4>(new Matrix4x4);

    mViewMatrix->lookAt(mTransform.mTransform.getPosition(), mTransform.mTransform.getPosition()+mDirection, mUp);
}

void Camera::setTarget(const gsl::Vec3 &target)
{
    mTarget = target;
}

void Camera::setDirection(float horizontalAngle, float verticalAngle)
{
    mDirection = gsl::Vec3(
        std::cos(verticalAngle) * std::sin(horizontalAngle),
        std::sin(verticalAngle),
        std::cos(verticalAngle) * std::cos(horizontalAngle)
    );
//    qDebug() << "X = " << mDirection.getX() << ", Y = " << mDirection.getY()
//             << ", Z = " << mDirection.getZ();

    mRight = gsl::Vec3(
        std::sin(horizontalAngle - 3.14f/2.0f),
        0,
        std::cos(horizontalAngle - 3.14f/2.0f)
    );

    mUp = mRight ^ mDirection;

    gsl::Vec3 correctedViewPosition{-mDirection.getX(), -mDirection.getY(), -mDirection.getZ()};

    //mViewMatrix->lookAt(correctedViewPosition, correctedViewPosition + mDirection, Vec3(0.f, 1.f, 0.f));
}

void Camera::movement(bool w, bool s, bool d, bool a, float deltaTime)
{
    float speed{50.f};
    gsl::Vec3 currentPosition = mTransform.mTransform.getPosition();
    if (w){
        currentPosition = currentPosition + mDirection * deltaTime * speed;
    }
    // Move backward
    if (s){
        currentPosition = currentPosition - mDirection * deltaTime * speed;
    }
    // Strafe right
    if (d){
        currentPosition =  currentPosition + mRight * deltaTime * speed;
    }
    // Strafe left
    if (a)
    {
        currentPosition =  currentPosition - mRight * deltaTime * speed;
    }
    mTransform.mTransform.setPosition(currentPosition);
    //mViewMatrix->lookAt(currentPosition, currentPosition + mDirection, Vec3(0.f, 1.f, 0.f));
//    setViewMatrix();
}

Mat4* Camera::getPerspectiveMatrix() const
{
    return mPerspectiveMatrix;
}

TransformComponent& Camera::getTransform()
{
    return mTransform;
}

Mat4* Camera::getViewMatrix()
{
    Mat4* View = new gsl::Mat4;

    View->lookAt(mTransform.mTransform.getPosition(), mTransform.mTransform.getPosition()+mDirection, mUp);
    return View;
}
