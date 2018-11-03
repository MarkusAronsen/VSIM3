#include "vec4.h"
#include "vec3.h"

namespace gsl
{

Vec4::Vec4(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat w_in)
    : x{x_in}, y{y_in}, z{z_in}, w{w_in}
{
}

Vec4::Vec4(Vec3 vec3_in, GLfloat w_in)
    : x{vec3_in.getX()}, y{vec3_in.getY()}, z{vec3_in.getZ()}, w{w_in}
{
}

Vec4::Vec4(const Vec3 &vec3_in) :
    x{vec3_in.getX()}, y{vec3_in.getY()}, z{vec3_in.getZ()}, w{1.0f}
{
}

Vec4::Vec4(const int v) :
     x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{1.0f}
 {
 }

 Vec4::Vec4(const double v)
     : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{1.0f}
 {
 }

GLfloat Vec4::operator *(const Vec4 &v) const
{
    return x*v.getX() + y*v.getY() + z*v.getZ() + w*v.getW();
}

GLfloat Vec4::length()
{
    return sqrt(x*x+y*y+z*z+w*w);
}

Vec3 Vec4::toVector3D()
{
    return Vec3(x, y, z);
}

Vec3 Vec4::getVec3()
{
    return { x, y, z };
}

void Vec4::print()
{
    std::cout << "(" << x << "," << y << "," << z << "," << w << ")\n";
}

//friend function
//QDebug operator<<(QDebug out, const Vec4 &rhs)
//{
//    out << "(" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << ")";
//    return out;
//}

GLfloat Vec4::getX() const
{
    return x;
}

void Vec4::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vec4::getY() const
{
    return y;
}

void Vec4::setY(const GLfloat &value)
{
    y = value;
}

GLfloat Vec4::getZ() const
{
    return z;
}

void Vec4::setZ(const GLfloat &value)
{
    z = value;
}

GLfloat Vec4::getW() const
{
    return w;
}
void Vec4::setW(const GLfloat &value)
{
    if(value == 0.f || value == 1.f)    //w should be only 0 or 1
        w = value;
}

} //namespace
