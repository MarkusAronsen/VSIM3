#include "vec2.h"
#include "vec3.h"
#include <cmath>

namespace gsl
{
Vec2::Vec2(GLfloat x_in, GLfloat y_in) : x{x_in}, y{y_in} { }

Vec2::Vec2(const int v):
    x{ static_cast<float>(v) }, y{ static_cast<float>(v) }
{
}

Vec2::Vec2(const double v) :
    x{ static_cast<float>(v) }, y{ static_cast<float>(v) }
{
}

const Vec2 &Vec2::operator =(const Vec2 &rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
    Vec2 u;
    u.x = x + rhs.x;
    u.y = y + rhs.y;
    return u;

    // alt  return { x + rhs.x, y + rhs.y };
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
    Vec2 u;
    u.x = x - rhs.x;
    u.y = y - rhs.y;
    return u;

    // alt  return { x - rhs.x, y - rhs.y };
}

Vec2 &Vec2::operator+=(const Vec2 &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vec2 &Vec2::operator-=(const Vec2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vec2 Vec2::operator-() const
{
    return { -x, -y };
}

Vec2 Vec2::operator*(float lhs) const
{
    return { x * lhs, y * lhs };
}

//friend function
Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs)
{
    return { lhs.x * rhs.x, lhs.y * rhs.y };
}


GLfloat Vec2::getX() const
{
    return x;
}

void Vec2::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vec2::getY() const
{
    return y;
}

float Vec2::length() const
{
    return sqrt(pow(x, 2.0f) + pow(y, 2.0f));
}

void Vec2::setY(const GLfloat &value)
{
    y = value;
}

void Vec2::normalize()
{
    auto l = length();
    x = x / l;
    y = y / l;
}

Vec3 Vec2::barycentricCoordinates(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3)
{
    Vec2 p12 = p2 - p1;
    Vec2 p13 = p3 - p1;
    Vec3 n = cross(Vec3{ p12.x, p12.y, 0.0f }, Vec3{ p13.x, p13.y, 0.0f });
    float doubleArea = cross(p12, p13);

    Vec3 result;
    // u
    Vec2 p = p2 - *this;
    Vec2 q = p3 - *this;
    n = cross(Vec3{ p.x, p.y, 0.0f }, Vec3{ q.x, q.y, 0.0f });
    result.setX(n.getZ() / doubleArea);
    // v
    p = p3 - *this;
    q = p1 - *this;
    n = cross(Vec3{ p.x, p.y, 0.0f }, Vec3{ q.x, q.y, 0.0f });
    result.setY(n.getZ() / doubleArea);
    // w
    p = p1 - *this;
    q = p2 - *this;
    n = cross(Vec3{ p.x, p.y, 0.0f }, Vec3{ q.x, q.y, 0.0f });
    result.setZ(n.getX() / doubleArea);
    return result;
}

void Vec2::print()
{
    std::cout << "(" << x << "," << y << ")\n";
}

//std::array<float, 2> Vec2::getArray() const
//{
//    return {{ x, y }};
//}

//    QDebug operator<<(QDebug out, const Vec2 &rhs)
//    {
//        out << "(" << rhs.x << "," << rhs.y << ")";
//        return  out;
//    }

std::ostream &operator<<(std::ostream &out, const Vec2 &rhs)
{
    out << "(" << rhs.x << "," << rhs.y << ")";
    return out;
}

float cross(const Vec2 &v1, const Vec2 &v2)
{
    return std::abs((v1.x * v2.y) - (v1.y * v2.x));
}

float dot(const Vec2  &v1, const Vec2  &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

Vec2 normalize(const Vec2 &v)
{
    GLfloat l = std::sqrt(v.x*v.x + v.y*v.y);
    return { v.x / l, v.y / l };
}

} //namespace
