#ifndef COLORMATERIAL_H
#define COLORMATERIAL_H

#include <QtGui/qopengl.h> // for GLuint
#include "material.h"
#include "mat4.h"

class Shader;
//class Matrix4;

using namespace gsl;

class ColorMaterial : public Material
{
public:
    ColorMaterial(Shader* shader);

    void setMMatrix(Mat4* modelMatrixIn) override;
    void setupShader(GLuint VAOin) override;

private:
    GLint mMVPUniform{0};
    Mat4 mMVPMatrix;
};
#endif // COLORMATERIAL_H
