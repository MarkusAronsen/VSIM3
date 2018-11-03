#include "vec3.h"
#include <cmath>
#include "math_constants.h"
#include "gsl_math.h"

namespace gsl
{

Vec3::Vec3(GLfloat x_in, GLfloat y_in, GLfloat z_in)
    : x{x_in}, y{y_in}, z{z_in}
{
}

Vec3::Vec3(const int v) :
    x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}
{
}

Vec3::Vec3(const double v) :
    x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}
{
}

const Vec3& Vec3::operator =(const Vec3 &rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

Vec3 Vec3::operator +(const Vec3 &rhs) const
{
    Vec3 u;
    u.x = x + rhs.x;
    u.y = y + rhs.y;
    u.z = z + rhs.z;
    return u;
    // return { x + rhs.x, y + rhs.y, z + rhs.z };
}

Vec3 Vec3::operator -(const Vec3 &rhs) const
{
    Vec3 u;
    u.x = x - rhs.x;
    u.y = y - rhs.y;
    u.z = z - rhs.z;
    return u;
    // return { x - rhs.x, y - rhs.y, z - rhs.z };
}

Vec3 &Vec3::operator+=(const Vec3 &rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vec3 Vec3::operator-() const
{
    return { -x, -y, -z };
}

Vec3 Vec3::operator*(float rhs) const
{
    return { x * rhs, y * rhs, z * rhs };
}

bool Vec3::operator==(const Vec3 &rhs)
{
    if(x == rhs.x && y == rhs.y && z == rhs.z)
        return true;
    else
        return false;
}

//OEF: må lages som friend funksjon
//Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs)
//{
//    return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
//}

Vec3 Vec3::operator ^(const Vec3 &rhs) const
{
    Vec3 w; // this = [-1, 0, 0], rhs = [0, 0, 1]
    w.x =  y*rhs.z - z*rhs.y; // = 0 * 1 - 0 * 0
    w.y =  z*rhs.x - x*rhs.z; // = 0 * 0 - (-1) * 1
    w.z =  x*rhs.y - y*rhs.x; // = (-1) * 0 - 0 * (-1)
    return w;
}

void Vec3::normalize()
{
    GLfloat d = x*x+y*y+z*z;
    d = std::sqrt(d);
    if (d > 0.f)
    {
        x=x/d;
        y=y/d;
        z=z/d;
    }
    //Alex versjon:
    //    void Vec3::normalize()
    //    {
    //        auto l = length();
    //        x = x / l;
    //        y = y / l;
    //        z = z / l;
    //    }
}

Vec3 Vec3::normalized()
{
    Vec3 normalized;
    GLfloat d = x*x+y*y+z*z;
    d = std::sqrt(d);
    if (d > 0.f)
    {
        normalized.x=x/d;
        normalized.y=y/d;
        normalized.z=z/d;
    }
    return normalized;
}

void Vec3::rotateX(GLfloat angle)
{
    Vec3 dir;
    angle = gsl::deg2rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x;
    dir.y = y * std::cos(angle) + z * -std::sin(angle);
    dir.z = y * std::sin(angle) + z * std::cos(angle) ;

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

void Vec3::rotateY(GLfloat angle)
{
    Vec3 dir;
    angle = gsl::deg2rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x * std::cos(angle) + z * std::sin(angle);
    dir.y = y;
    dir.z = x * -std::sin(angle) + z * std::cos(angle);

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

void Vec3::rotateZ(GLfloat angle)
{
    Vec3 dir;
    angle = gsl::deg2rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x * std::cos(angle) + y * -std::sin(angle);
    dir.y = x * std::sin(angle) + y * std::cos(angle);
    dir.z = z;

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

//Vec3 Vec3::operator*(Matrix4x4 q) const
//{
//    q = q;  //just noncense for now to get rid of warning

    //            -------------- MATRIX MULTIPLICATION ----------------------
    //            | 0  | 1  | 2  | 3  |
    //            | 4  | 5  | 6  | 7  |        X         | x  | y  | z  | 1 |
    //            | 8  | 9  | 10 | 11 |
    //            | 12 | 13 | 14 | 15 |


    //    float _x,_y,_z,_w;
    //    _x = (q.column(0).x() * x + q.column(0).y() * y + q.column(0).z() * z + q.column(0).w());
    //    _y = (q.column(1).x() * x + q.column(1).y() * y + q.column(1).z() * z + q.column(1).w());
    //    _z = (q.column(2).x() * x + q.column(2).y() * y + q.column(2).z() * z + q.column(2).w());
    //    _w = (q.column(3).x() * x + q.column(3).y() * y + q.column(3).z() * z + q.column(3).w());

    //    if(_w == 1.0f)
    //        return Vec3(_x, _y, _z);
    //    else
    //        return Vec3(_x/_w, _y/_w, _z/_w);
//    return *this;   //just noncense for now to get rid of warning
//}

GLfloat Vec3::length() const
{
    return std::sqrt(x*x+y*y+z*z);
}

//std::array<float, 3> Vec3::getArray() const
//{
//    return {{ x, y, z }};
//}

void Vec3::print()
{
    std::cout << "(" << x << "," << y << "," << z << ")\n";
}

//QDebug operator<<(QDebug out, const Vec3 &rhs)
//{
//    out << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
//    return  out;
//}

//friend function #include <fstream>
//const std::string &operator<<(const std::string &path, const std::vector<Vec3> &array)
//{
//    std::ofstream outf{ path, std::ios::app };
//    if (!outf) {
//        std::cout << "Could not open \"" << path.c_str() << "\" for writing";
//        std::exit(1);
//    }
//    for (unsigned int i = 0; i < array.size(); ++i)
//        outf << "(" << array[i].x << ", " << array[i].y << ", " << array[i].z << ")\n";
//    return path;
//}

//friend function #include <fstream>
//const std::string &operator>>(const std::string &path, std::vector<Vec3> &array)
//{
//    std::ifstream inf{ path };
//    if (!inf) {
//        std::cout << "Could not open \"" << path.c_str() << "\" for reading";
//        std::exit(1);
//    }
//    Vec3 temp;
//    char skip;
//    while (!inf.eof()) {
//        inf >> skip >> temp.x >> skip >> temp.y >> skip >> temp.z >> skip;
//        array.push_back(temp);
//    }
//    return path;
//}

GLfloat Vec3::getX() const
{
    return x;
}

void Vec3::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vec3::getY() const
{
    return y;
}

void Vec3::setY(const GLfloat &value)
{
    y = value;
}

GLfloat Vec3::getZ() const
{
    return z;
}

void Vec3::setZ(const GLfloat &value)
{
    z = value;
}

//QVector3D Vec3::getQVector() const
//{
//    return QVector3D(x, y, z);
//}

GLfloat Vec3::operator *(const Vec3 &v) const
{
    return x*v.x + y*v.y + z*v.z;
}

Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
    return { v1.y * v2.z - v1.z * v2.y, -(v1.x * v2.z - v1.z * v2.x), v1.x * v2.y - v1.y * v2.x };
}

GLfloat dot(const Vec3 &v1, const Vec3 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 normalize(const Vec3 &v)
{
    float l = std::sqrt(std::pow(v.x, 2.0f) + std::pow(v.y, 2.0f) + std::pow(v.z, 2.0f));
    return { v.x / l, v.y / l, v.z / l };
}

} //namespace
