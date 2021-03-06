#include "material.h"
#include "shader.h"
#include "vertex.h"
#include "camera.h"
#include <memory>

//Static member - this will be the same for all materials!
Camera* Material::mCurrentCamera{};

Material::Material(Shader* shader) : mShader{shader}
{
}

Shader* Material::getShader() const
{
    return mShader;
}

void Material::setupShader(GLuint VAOin)
{
    // Our Vertex class structure is like this, so every model has
    // the same layout for vertex buffers
    // and all shaders use the same layout for this

    initializeOpenGLFunctions();
    glUseProgram(mShader->getProgram());

    glBindVertexArray(VAOin);   //has to know what VAO to set this up for

    // 1rst attribute buffer : positions
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);
    // 2nd attribute buffer : colors or normals
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 3rd attribute buffer : uvs (ST)
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
}

void Material::setCurrentCamera(Camera* currentCamera)
{
    mCurrentCamera = currentCamera;
}
