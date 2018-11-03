#ifndef VEC4_H
#define VEC4_H

#include <cmath>
#include <iostream>
#include <cassert>

namespace gsl
{
typedef float GLfloat;
class Vec3;
class Matrix4x4;

class Vec4
{
    template <size_t row, size_t col>
    friend class Matrix;

public:
    //constructors:
    Vec4(GLfloat x_in=0.f, GLfloat y_in=0.f, GLfloat z_in=0.f, GLfloat w_in = 0.f);
    Vec4(Vec3 vec3_in, GLfloat w_in);
    Vec4(const Vec3 &vec3_in);
    Vec4(const int v);
    Vec4(const double v);

    //operators:
    //const Vec4 &operator = (const Vec4& v);
    Vec4 operator + (const Vec4& v) const;
    Vec4 operator - (const Vec4& v) const;

    Vec4 operator ^ (const Vec4& v) const;  // Cross product

    //functions:
    Vec3 toVector3D();
    Vec3 getVec3(); //OEF: samme som toVector3D()
    void print();

    //OEF: Ikke implementert enda
//    void normalize();
//    Vec4 normalized();

    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    GLfloat operator * (const Vec4& v) const; // Dot product
    Vec4 operator * (GLfloat c) const;
    Vec4 operator * (Matrix4x4 q) const;

    GLfloat length();

    //Operator overloading as friendfunction
    friend std::ostream &operator<<(std::ostream &output, const Vec4 &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y <<
                  ", Z = " << rhs.z << ", W = " << rhs.w;
        return output;
    }

    //friend QDebug operator<<(QDebug out, const Vec4 &rhs);

    GLfloat& operator[] (const int index)
    {
        assert(index <4 && index >=0);

        switch (index) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        }
        return x;   //to silence compiler warnings
    }

    GLfloat getX() const;
    void setX(const GLfloat &value);
    GLfloat getY() const;
    void setY(const GLfloat &value);
    GLfloat getZ() const;
    void setZ(const GLfloat &value);
    GLfloat getW() const;
    void setW(const GLfloat &value);

private:
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
};

} //namespace

#endif // VEC4_H
