#include "vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(gsl::Vec3 a, gsl::Vec3 b , gsl::Vec2 c)
{
    mXYZ = a;
    mNormal = b;
    mST = c;
}

Vertex::~Vertex()
{
    //qDebug() << "Vertex::~Vertex()";
}

void Vertex::set_xyz(gsl::GLfloat* xyz)
{
    mXYZ.setX(xyz[0]);
    mXYZ.setY(xyz[1]);
    mXYZ.setZ(xyz[2]);
}

void Vertex::set_xyz(gsl::GLfloat x, gsl::GLfloat y, gsl::GLfloat z)
{
    mXYZ.setX(x); mXYZ.setY(y); mXYZ.setZ(z);
}

void Vertex::set_xyz(gsl::Vec3 xyz_in)
{
    mXYZ = xyz_in;
}

void Vertex::set_rgb(gsl::GLfloat* rgb)
{
    mNormal.setX(rgb[0]);
    mNormal.setY(rgb[1]);
    mNormal.setZ(rgb[2]);
}

void Vertex::set_rgb(gsl::GLfloat r, gsl::GLfloat g, gsl::GLfloat b)
{
    mNormal.setX(r); mNormal.setY(g); mNormal.setZ(b);
}

void Vertex::set_normal(gsl::GLfloat* normal)
{
    mNormal.setX(normal[0]);
    mNormal.setY(normal[1]);
    mNormal.setZ(normal[2]);
}

void Vertex::set_normal(gsl::GLfloat x, gsl::GLfloat y, gsl::GLfloat z)
{
    mNormal.setX(x); mNormal.setY(y); mNormal.setZ(z);
}

void Vertex::set_normal(gsl::Vec3 normal_in)
{
    mNormal = normal_in;
}

void Vertex::set_st(gsl::GLfloat* st)
{
    mST.setX(st[0]);
    mST.setY(st[1]);
}

void Vertex::set_st(gsl::GLfloat s, gsl::GLfloat t)
{
    mST.setX(s); mST.setY(t);
}

void Vertex::set_uv(gsl::GLfloat u, gsl::GLfloat v)
{
    mST.setX(u); mST.setY(v);
}

gsl::Vec3 Vertex::get_xyz()
{

    return mXYZ;
}

gsl::GLfloat Vertex::get_x()
{
    return mXYZ.getX();
}

gsl::GLfloat Vertex::get_y()
{
    return mXYZ.getY();
}

gsl::GLfloat Vertex::get_z()
{
    return mXYZ.getZ();
}

gsl::Vec3 Vertex::get_normal()
{
    return mNormal;
}

std::ostream& operator<< (std::ostream& os, const Vertex& v)
{
   os << "(" << v.mXYZ.getX() << ", " << v.mXYZ.getY() << ", " << v.mXYZ.getZ() << ") ";
   os << "(" << v.mNormal.getX() << ", " << v.mNormal.getY() << ", " << v.mNormal.getZ() << ") ";
//   os << "(" << v.mST.getX() << ", " << v.mST.getY() << ") ";
   return os;
}
//std::istream& operator>> (std::istream& is, Vertex& v)
//{
//// Four temp variables to hold parentecis and comma
//   char dum, dum2, dum3, dum4;
//   is >> dum >> v.mXYZ >> dum2 >> v.mXYZ.y >> dum3 >> v.mXYZ.z >> dum4;
//   is >> dum >> v.mNormal.x >> dum2 >> v.mNormal.y >> dum3 >> v.mNormal.z >> dum4;
//   is >> dum >> v.mST.x >> dum2 >> v.mST.y >> dum3;

//   return is;
//}
