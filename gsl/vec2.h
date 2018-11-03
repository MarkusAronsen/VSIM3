#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

namespace gsl
{
typedef float    GLfloat;

class Vec2
{
public:
    // Constructor
    //OEF: vi bør få vekk avhengigheten av GLfloat - nå gjort med typedef
    Vec2(GLfloat x_in = 0.0, GLfloat y_in = 0.0);
    //OEF: er disse interessante Alex?:
    Vec2(const int v);
    Vec2(const double v);

    // Operators
    //OEF: likte bruken av rhs og lhs fra Alex - lett å skjønne hva som skjer
    Vec2 operator+ (const Vec2 &rhs) const;     // v + v
    Vec2 operator- (const Vec2 &rhs) const;     // v - v
    Vec2 &operator+=(const Vec2 &rhs);          // v += v
    Vec2 &operator-=(const Vec2 &rhs);          // v -= v
    Vec2 operator-() const;         			// -v
    Vec2 operator*(float lhs) const;            // v * f
    const Vec2 &operator = (const Vec2& rhs);   // v = v

    friend Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs);    // v * v
    //OEF: denne skal vel ikke være med i vec2
    //friend Vec3 operator*(float lhs, const Vec3 &rhs);          // f * v


    //functions
    float length() const;
    void normalize();
    class Vec3 barycentricCoordinates(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3);
    void print();
    //std::array<float, 2> getArray() const; //krever #include <array>
    friend std::ostream &operator<<(std::ostream &out, const Vec2 &rhs);
//  friend QDebug operator<<(QDebug out, const Vec2 &rhs);

    friend float cross(const Vec2 &v1, const Vec2 &v2);
    friend float dot(const Vec2 &v1, const Vec2 &v2);
    friend Vec2 normalize(const Vec2 &v);

    GLfloat getX() const;
    void setX(const GLfloat &value);

    GLfloat getY() const;
    void setY(const GLfloat &value);

private:
    GLfloat x;
    GLfloat y;
};

} //namespace

#endif // VEC2_H
