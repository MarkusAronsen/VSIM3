#include "colormaterial.h"
#include "shader.h"
#include "vertex.h"
#include "camera.h"

ColorMaterial::ColorMaterial(Shader *shader) : Material(shader)
{
}

void ColorMaterial::setupShader(GLuint VAOin)
{
    //Common stuff for all materials / shaders
    Material::setupShader(VAOin);

    //enable the uniforms
    mMVPUniform = glGetUniformLocation( mShader->getProgram(), "matrix" );
    glUseProgram(0);
    glBindVertexArray(0);
}

void ColorMaterial::setMMatrix(Mat4 *modelMatrixIn)
{
    Mat4* persp = mCurrentCamera->getPerspectiveMatrix();
    Mat4* view = mCurrentCamera->getViewMatrix();
    mMVPMatrix = (*persp) * (*view) * (*modelMatrixIn);
    glUniformMatrix4fv(mMVPUniform, 1, GL_TRUE, mMVPMatrix.constData());
}
