#ifndef GSL_MATH_H
#define GSL_MATH_H

#include <vector>

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "quaternion.h"
//#include "matrix.h"
#include "math_constants.h"

namespace gsl
{
    // Type alias
//    using Mat3 = Matrix<3, 3>;
//    using Mat4 = Matrix<4, 4>;
//    using Vec = Vec3;

    // Functions
    GLfloat rad2deg(GLfloat rad);
    GLfloat deg2rad(GLfloat deg);
    GLfloat mod(GLfloat x, GLfloat y);
    GLfloat clamp(GLfloat x, GLfloat min, GLfloat max);

    // Interpolation
    // remember time can only be between 0 and 1
    Vec2 lerp2D(GLfloat time, Vec2 start, Vec2 end); // Move a straight line with linear time-step between points
    Vec3 lerp3D(GLfloat time, Vec3 start, Vec3 end);

    // Lerp between quaternion rotations,
    // remember time can only be between 0 and 1
    Quaternion qLerp(GLfloat time, Quaternion start, Quaternion end);
    Quaternion qLerp(GLfloat time, Vec3 start, Vec3 end);
    Vec3 qLerpEuler(GLfloat time, Quaternion start, Quaternion end);
    Vec3 qLerpEuler(GLfloat time, Vec3 start, Vec3 end);

    // Curves
    Vec3 bezierCurve(std::vector<Vec3> points, GLfloat t, unsigned long long degree = 3);
    Vec3 bSpline(const std::vector<Vec3>& points, const std::vector<GLfloat>& t, GLfloat x, unsigned long long degree = 3);

    // Basic vector directions
    Vec3 up();
    Vec3 right();
    Vec3 forward();
    Vec3 one();
    Vec3 zero();
}

#endif // GSL_MATH_H
