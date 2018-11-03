#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
//#include <fstream>

class Matrix4x4;
//class QVector3D;

namespace gsl
{
    typedef float  GLfloat;

    class Vec3
    {
        template <size_t row, size_t col>
        friend class Matrix;

    public:
        // Constructor
        Vec3(GLfloat x_in=0.0, GLfloat y_in=0.0, GLfloat z_in=0.0);
        Vec3(const int v);
        Vec3(const double v);

        // Operators
        const Vec3 &operator = (const Vec3& rhs);       // v = v
        Vec3 operator+(const Vec3 &rhs) const;          // v + v
        Vec3 operator-(const Vec3 &rhs) const;          // v - v
        Vec3 &operator+=(const Vec3 &rhs);              // v += v
        Vec3 &operator-=(const Vec3 &rhs);              // v -= v
        Vec3 operator-() const;                         // -v
        Vec3 operator*(GLfloat rhs) const;              // v * f
        bool operator==(const Vec3 &rhs);               // v == v

        //friend Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs);// v * v
        //friend Vec3 operator*(float lhs, const Vec3 &rhs);      // f * v
        Vec3 operator^ (const Vec3& rhs) const;  // Cross product
        GLfloat operator *(const Vec3& v) const; // Dot product
        //Vec3 operator* (Matrix4x4 q) const;

        // Functions
        void normalize();
        Vec3 normalized();

        void rotateX(GLfloat angle);
        void rotateY(GLfloat angle);
        void rotateZ(GLfloat angle);

        GLfloat length() const;
        //std::array<float, 3> getArray() const; // OEF: trenger #include <array>

        void print();

    //    friend QDebug operator<<(QDebug out, const Vec3 &rhs);        //    QVector3D getQVector() const;   //for use with qDebug()
    //    QVector3D getQVector() const;   //for use with qDebug()

        friend std::ostream &operator<<(std::ostream &output, const Vec3 &rhs )
        {
            output << "X = " << rhs.x << ", Y = " << rhs.y << ", Z = " << rhs.z;
            return output;
        }

        //OEF: disse krever #include <fstream>
    //    friend const std::string &operator<<(const std::string &path, const std::vector<Vec3> &array);    // Write an Array of vectors to file
    //    friend const std::string &operator>>(const std::string &path, std::vector<Vec3> &array);          // Read all vectors from file

        friend Vec3 cross(const Vec3 &v1, const Vec3 &v2);
        friend GLfloat dot(const Vec3 &v1, const Vec3 &v2);
        friend Vec3 normalize(const Vec3 &v);

        GLfloat getX() const;
        void setX(const GLfloat &value);
        GLfloat getY() const;
        void setY(const GLfloat &value);
        GLfloat getZ() const;
        void setZ(const GLfloat &value);

    private:
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };

} //namespace

#endif // VEC3_H
