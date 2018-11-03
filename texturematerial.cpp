#include "texturematerial.h"
#include "shader.h"
#include "constants.h"
#include "texture.h"
#include "camera.h"

TextureMaterial::TextureMaterial(Shader *shader) : Material(shader)
{

}

void TextureMaterial::setMMatrix(Mat4 *modelMatrixIn)
{
    glUniform1i(mTextureUniform1, 0);
    glUniform1i(mTextureUniform2, 1);

    glUniformMatrix4fv( mModelMatrixUniform, 1, GL_TRUE, modelMatrixIn->constData());
    glUniformMatrix4fv( mViewMatrixUniform, 1, GL_TRUE, mCurrentCamera->getViewMatrix()->constData());
    glUniformMatrix4fv( mPerspectiveMatrixUniform, 1, GL_TRUE, mCurrentCamera->getPerspectiveMatrix()->constData());
}

void TextureMaterial::setupShader(GLuint VAOin)
{
    Material::setupShader(VAOin);

    //enable the uniforms
    //enable the matrixUniform
    mModelMatrixUniform = glGetUniformLocation( mShader->getProgram(), "mMatrix" );
    mViewMatrixUniform = glGetUniformLocation( mShader->getProgram(), "vMatrix" );
    mPerspectiveMatrixUniform = glGetUniformLocation( mShader->getProgram(), "pMatrix" );

    mTextureUniform1 = glGetUniformLocation(mShader->getProgram(), "textureSampler1");
    mTextureUniform2 = glGetUniformLocation(mShader->getProgram(), "textureSampler2");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture1->id());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture2->id());
    glUseProgram(0);
    glBindVertexArray(0);
}

void TextureMaterial::setTexture1(Texture *texture1)
{
    mTexture1 = texture1;
}

void TextureMaterial::setTexture2(Texture *texture2)
{
    mTexture2 = texture2;
}
