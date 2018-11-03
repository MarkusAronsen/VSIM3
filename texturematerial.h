#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "gltypes.h"
#include <memory>

#include "material.h"

class Shader;
class Matrix4x4;
class Texture;

class TextureMaterial : public Material
{
public:
    TextureMaterial(Shader* shader);

    void setMMatrix(Mat4* modelMatrixIn) override;
    void setupShader(GLuint VAOin) override;

    void setTexture1(Texture *texture1);
    void setTexture2(Texture *texture2);

private:
    GLuint mModelMatrixUniform{0};
    GLuint mViewMatrixUniform{0};
    GLuint mPerspectiveMatrixUniform{0};

    GLuint mTextureUniform1{0};
    GLuint mTextureUniform2{0};

    Texture* mTexture1{nullptr};
    Texture* mTexture2{nullptr};

    //Matrix4* mMVPMatrix; //not used
};

#endif // TEXTUREMATERIAL_H
