#ifndef LIGHTCOLORMATERIAL_H
#define LIGHTCOLORMATERIAL_H

#include <QtGui/qopengl.h> // for GLuint
#include "material.h"
#include "mat4.h"

class Shader;
class PointLight;

class LightColorMaterial : public Material
{
public:
    LightColorMaterial(Shader* shader, PointLight *pointLight);

    void setMMatrix(Mat4* modelMatrixIn) override;
    void setupShader(GLuint VAOin) override;

    void setPointLight(PointLight *pointLight);
    PointLight *getPointLight() const;

    void setColors(const gsl::Vec3 &ambientColor, const gsl::Vec3 &diffuseColor, const gsl::Vec3 &specularColor, float shinyness);

private:
    GLuint mModelMatrixUniform;
    GLuint mViewMatrixUniform;
    GLuint mPerspectiveMatrixUniform;

    GLuint mCameraPosition;
    GLuint mLightPosition;

    gsl::Vec3 mAmbientColor;
    gsl::Vec3 mDiffuseColor;
    gsl::Vec3 mSpecularColor;
    float mShinyness;

    PointLight *mPointLight;

    GLuint mLightAmbientColor;
    GLuint mLightDiffuseColor;
    GLuint mLightSpecularColor;

    GLuint mMaterialAmbientColor;
    GLuint mMaterialDiffuseColor;
    GLuint mMaterialSpecularColor;
    GLuint mMaterialShininess;

    gsl::Mat4 mMVPMatrix;
};

#endif // LIGHTCOLORMATERIAL_H
