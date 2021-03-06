#include "quaternion.h"
#include "matrix.h"
#include "vec3.h"
#include "gsl_math.h"

namespace gsl
{
    Quaternion::Quaternion()
    {
        w = 1.f;
        x = 0.f;
        y = 0.f;
        z = 0.f;
    }

    Quaternion::Quaternion(GLfloat roll, GLfloat pitch, GLfloat yaw)
    {
        // Abbreviations for the various angular functions
        GLfloat cy = cos(deg2rad(yaw) * 0.5f);
        GLfloat sy = sin(deg2rad(yaw) * 0.5f);
        GLfloat cr = cos(deg2rad(roll) * 0.5f);
        GLfloat sr = sin(deg2rad(roll) * 0.5f);
        GLfloat cp = cos(deg2rad(pitch) * 0.5f);
        GLfloat sp = sin(deg2rad(pitch) * 0.5f);

        w = cy * cr * cp + sy * sr * sp;
        x = cy * sr * cp - sy * cr * sp;
        y = cy * cr * sp + sy * sr * cp;
        z = sy * cr * cp - cy * sr * sp;
    }

    Quaternion::Quaternion(const Vec3 &rollpitchyaw)
    {
        // Abbreviations for the various angular functions
        GLfloat cy = cos(deg2rad(rollpitchyaw.getZ()) * 0.5f);
        GLfloat sy = sin(deg2rad(rollpitchyaw.getZ()) * 0.5f);
        GLfloat cr = cos(deg2rad(rollpitchyaw.getX()) * 0.5f);
        GLfloat sr = sin(deg2rad(rollpitchyaw.getX()) * 0.5f);
        GLfloat cp = cos(deg2rad(rollpitchyaw.getY()) * 0.5f);
        GLfloat sp = sin(deg2rad(rollpitchyaw.getY()) * 0.5f);

        w = cy * cr * cp + sy * sr * sp;
        x = cy * sr * cp - sy * cr * sp;
        y = cy * cr * sp + sy * sr * cp;
        z = sy * cr * cp - cy * sr * sp;
    }

    Quaternion::Quaternion(GLfloat ox, GLfloat oy, GLfloat oz, GLfloat ow)
        : x{ox}, y{oy}, z{oz}, w{ow}
    {

    }

    Quaternion::Quaternion(GLfloat angle_rad, Vec3 axis)
    {
        w = std::cos(angle_rad/2.f);
        x = axis.getX()*std::sin(angle_rad/2.f);
        y = axis.getY()*std::sin(angle_rad/2.f);
        z = axis.getZ()*std::sin(angle_rad/2.f);
    }

    Quaternion Quaternion::operator*(const Quaternion &q)
    {
        double yy = (w - y) * (q.w + q.z);
        double zz = (w + y) * (q.w - q.z);
        double ww = (z + x) * (q.x + q.y);
        double xx = ww + yy + zz;
        double qq = 0.5 * (xx + (z - x) * (q.x - q.y));

        return Quaternion(qq - ww + (z - y) * (q.y - q.z),
                          qq - xx + (x + w) * (q.x + q.w),
                          qq - yy + (w - x) * (q.y + q.z),
                          qq - zz + (z + y) * (q.w - q.x));
    }

    GLfloat &Quaternion::operator[](const int index)
    {
        switch(index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        }

        return w;
    }

    void Quaternion::normalize()
    {
        GLfloat mag = magnitude();

        if(mag != 0.f)
        {
            w /= mag;
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }

    GLfloat Quaternion::magnitude()
    {
        return std::sqrt(w*w+x*x+y*y+z*z);
    }

    Mat3 Quaternion::toRotationMatrix3()
    {
        normalize();

        return Mat3
        {
            1 - 2*y*y - 2*z*z, 2*x*y - 2*z*w, 2*x*z + 2*y*w,
            2*x*y + 2*z*w, 1 - 2*x*x - 2*z*z, 2*y*z - 2*x*w,
            2*x*z - 2*y*w, 2*y*z + 2*x*w, 1 - 2*x*x - 2*y*y
        };
    }

    Mat4 Quaternion::toRotationMatrix4()
    {
        normalize();

        return Mat4
        {
                1 - 2*y*y - 2*z*z, 2*x*y - 2*z*w, 2*x*z + 2*y*w, 0.f,
                2*x*y + 2*z*w, 1 - 2*x*x - 2*z*z, 2*y*z - 2*x*w, 0.f,
                2*x*z - 2*y*w, 2*y*z + 2*x*w, 1 - 2*x*x - 2*y*y, 0.f,
                0.f,           0.f,       0.f,              1.f
        };
    }

    void Quaternion::toEulerAngles(GLfloat &roll, GLfloat &pitch, GLfloat &yaw)
    {
        // roll (x-axis rotation)
        GLfloat sinr = +2.f * (w * x + y * z);
        GLfloat cosr = +1.f - 2.f * (x * x + y * y);
        roll = rad2deg(atan2(sinr, cosr));

        // pitch (y-axis rotation)
        GLfloat sinp = +2.f * (w * y - z * x);
        if (fabs(sinp) >= 1)
            pitch = rad2deg(copysign(PI / 2, sinp)); // use 90 degrees if out of range
        else
            pitch = rad2deg(asin(sinp));

        // yaw (z-axis rotation)
        GLfloat siny = +2.f * (w * z + x * y);
        GLfloat cosy = +1.f - 2.f * (y * y + z * z);
        yaw = rad2deg(atan2(siny, cosy));
    }

    Vec3 Quaternion::toEulerAngles()
    {
        Vec3 ret;
        // roll (x-axis rotation)
        GLfloat sinr = +2.f * (w * x + y * z);
        GLfloat cosr = +1.f - 2.f * (x * x + y * y);
        ret.setX(rad2deg(atan2(sinr, cosr)));

        // pitch (y-axis rotation)
        GLfloat sinp = +2.f * (w * y - z * x);
        if (fabs(sinp) >= 1)
            ret.setY(rad2deg(copysign(PI / 2, sinp))); // use 90 degrees if out of range
        else
            ret.setY(rad2deg(asin(sinp)));

        // yaw (z-axis rotation)
        GLfloat siny = +2.f * (w * z + x * y);
        GLfloat cosy = +1.f - 2.f * (y * y + z * z);
        ret.setZ(rad2deg(atan2(siny, cosy)));

        return ret;
    }
}
