#ifndef QUATERNION_H
#define QUATERNION_H

#include "math_constants.h"
#include "mat3.h"
#include "mat4.h"

namespace gsl
{
    class Quaternion
    {
    public:
        Quaternion();

        // From Euler angles
        Quaternion(GLfloat roll, GLfloat pitch, GLfloat yaw);
        Quaternion(const Vec3& rollpitchyaw);
        // From quaternion
        Quaternion(GLfloat ox, GLfloat oy, GLfloat oz, GLfloat ow);
        // From angle-axis
        Quaternion(GLfloat angle_rad, Vec3 axis);

        // Multiply Quaternion with Quaternion to Add the total rotations
        Quaternion operator*(const Quaternion& q);
        GLfloat& operator[](const int index);

        //Operator overloading as friendfunction
        friend std::ostream &operator<<(std::ostream &output, const Quaternion &rhs )
        {
            output << "X = " << rhs.x << ", Y = " << rhs.y <<
                      ", Z = " << rhs.z << ", W = " << rhs.w;
            return output;
        }

        void normalize();
        GLfloat magnitude();

        Mat4 toRotationMatrix4();
        Mat3 toRotationMatrix3();

        void toEulerAngles(GLfloat& roll, GLfloat& pitch, GLfloat& yaw);
        Vec3 toEulerAngles();
    private:
        float x, y, z, w;
    };
}

#endif // QUATERNION_H
