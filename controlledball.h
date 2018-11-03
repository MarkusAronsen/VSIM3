#ifndef CONTROLLEDBALL_H
#define CONTROLLEDBALL_H
#include "entity.h"
#include "octahedron.h"

class QKeyEvent;

class ControlledBall : public Octahedron
{
public:
    ControlledBall(int subDivisions=0);
    void update(GLfloat deltaTime);
    void keyboardMovement(QKeyEvent *event);
    void keyboardRelease(QKeyEvent *event);
    void getDirection();

private:
    GLfloat mMovementSpeed;
    float mRadius;
    Vec3 mVelocity;

    bool mForward{false};
    bool mBack{false};
    bool mRight{false};
    bool mLeft{false};
};

#endif // CONTROLLEDBALL_H
