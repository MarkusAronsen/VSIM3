#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include "vec3.h"
#include "vec2.h"

class Vertex
{
public:
    Vertex();
    Vertex(gsl::Vec3 a, gsl::Vec3 b, gsl::Vec2 c);
    ~Vertex();
    void set_xyz(gsl::GLfloat* xyz);
    void set_xyz(gsl::GLfloat x, gsl::GLfloat y, gsl::GLfloat z);
    void set_xyz(gsl::Vec3 xyz_in);
    void set_rgb(gsl::GLfloat* rgb);
    void set_rgb(gsl::GLfloat r, gsl::GLfloat g, gsl::GLfloat b);
    void set_normal(gsl::GLfloat* normal);
    void set_normal(gsl::GLfloat x, gsl::GLfloat y, gsl::GLfloat z);
    void set_normal(gsl::Vec3 normal_in);
    void set_st(gsl::GLfloat* st);
    void set_st(gsl::GLfloat s, gsl::GLfloat t);
    void set_uv(gsl::GLfloat u, gsl::GLfloat v);
    gsl::Vec3 get_xyz();
    gsl::GLfloat get_x();
    gsl::GLfloat get_y();
    gsl::GLfloat get_z();
    gsl::Vec3 get_normal();

private:
    gsl::Vec3 mXYZ;
    gsl::Vec3 mNormal;
    gsl::Vec2 mST;

    friend std::ostream& operator<< (std::ostream&, const Vertex&);
//    friend std::istream& operator>> (std::istream&, Vertex&);

};

#endif // VERTEX_H
