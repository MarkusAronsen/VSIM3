#include "lightcolormaterial.h"
#include "shader.h"
#include "vertex.h"
#include "camera.h"
#include "transform.h"
#include "pointlight.h"

LightColorMaterial::LightColorMaterial(Shader *shader, PointLight *pointLight)
    : Material{shader}, mPointLight{pointLight}
{
}

void LightColorMaterial::setupShader(GLuint VAOin)
{
    //Common stuff for all materials / shaders
    Material::setupShader(VAOin);

    //enable the uniforms
    mModelMatrixUniform = glGetUniformLocation( mShader->getProgram(), "mMatrix" );
    mViewMatrixUniform = glGetUniformLocation( mShader->getProgram(), "vMatrix" );
    mPerspectiveMatrixUniform = glGetUniformLocation( mShader->getProgram(), "pMatrix" );

    mCameraPosition = glGetUniformLocation( mShader->getProgram(), "cameraPosition" );
    mLightPosition= glGetUniformLocation(  mShader->getProgram(), "light.position" );

    mLightAmbientColor = glGetUniformLocation( mShader->getProgram(), "light.ambient" );
    mLightDiffuseColor = glGetUniformLocation( mShader->getProgram(), "light.diffuse" );
    mLightSpecularColor = glGetUniformLocation( mShader->getProgram(), "light.specular" );

    mMaterialAmbientColor = glGetUniformLocation( mShader->getProgram(), "material.ambient" );
    mMaterialDiffuseColor = glGetUniformLocation( mShader->getProgram(), "material.diffuse" );
    mMaterialSpecularColor = glGetUniformLocation( mShader->getProgram(), "material.specular" );
    mMaterialShininess = glGetUniformLocation( mShader->getProgram(), "material.shininess" );

    // Set material properties
//    glUniform3f( glGetUniformLocation( mShader->getProgram(), "material.ambient" ), 1.f, 0.f, 0.f );
//    glUniform3f( glGetUniformLocation( mShader->getProgram(), "material.diffuse"), 1.f, 0.f, 0.f );
//    glUniform3f( glGetUniformLocation( mShader->getProgram(), "material.specular" ), 0.5f, 0.5f, 0.5f ); // Specular doesn't have full effect on this object's material
//    glUniform1f( glGetUniformLocation( mShader->getProgram(), "material.shininess" ), 32.0f );
    glUseProgram(0);
    glBindVertexArray(0);
}

void LightColorMaterial::setPointLight(PointLight *pointLight)
{
    mPointLight = pointLight;
}

PointLight *LightColorMaterial::getPointLight() const
{
    return mPointLight;
}

void LightColorMaterial::setColors(const gsl::Vec3 &ambientColor, const gsl::Vec3 &diffuseColor, const gsl::Vec3 &specularColor, float shinyness)
{
    mAmbientColor = ambientColor;
    mDiffuseColor = diffuseColor;
    mSpecularColor = specularColor;
    mShinyness = shinyness;
}



void LightColorMaterial::setMMatrix(Mat4 *modelMatrixIn)
{
    glUniformMatrix4fv( mModelMatrixUniform, 1, GL_TRUE, modelMatrixIn->constData());
    glUniformMatrix4fv( mViewMatrixUniform, 1, GL_TRUE, mCurrentCamera->getViewMatrix()->constData());
    glUniformMatrix4fv( mPerspectiveMatrixUniform, 1, GL_TRUE, mCurrentCamera->getPerspectiveMatrix()->constData());

    gsl::Vec3 lightPos = mPointLight->getPosition();
    glUniform3f( mLightPosition, lightPos.getX(), lightPos.getY(), lightPos.getZ() );

    gsl::Vec3 cameraPos = mCurrentCamera->getTransform().mTransform.getPosition();
    glUniform3f( mCameraPosition, cameraPos.getX(), cameraPos.getY(), cameraPos.getZ() );

    glUniform3f( mLightAmbientColor, mPointLight->mAmbientColor.getX(), mPointLight->mAmbientColor.getY(), mPointLight->mAmbientColor.getZ() );
    glUniform3f( mLightDiffuseColor,  mPointLight->mDiffuseColor.getX(),  mPointLight->mDiffuseColor.getY(),  mPointLight->mDiffuseColor.getZ());
    glUniform3f( mLightSpecularColor, mPointLight->mSpecularColor.getX(),  mPointLight->mSpecularColor.getY(),  mPointLight->mSpecularColor.getZ());

    // Set material properties
    glUniform3f( mMaterialAmbientColor, mAmbientColor.getX(), mAmbientColor.getY(), mAmbientColor.getZ() );
    glUniform3f( mMaterialDiffuseColor, mDiffuseColor.getX(), mDiffuseColor.getY(), mDiffuseColor.getZ() );
    glUniform3f( mMaterialSpecularColor, mSpecularColor.getX(), mSpecularColor.getY(), mSpecularColor.getZ() ); // Specular doesn't have full effect on this object's material
    glUniform1f( mMaterialShininess, mShinyness );

}
