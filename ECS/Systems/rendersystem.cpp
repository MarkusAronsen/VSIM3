#include "rendersystem.h"
#include "componentbase.h"
#include "material.h"
#include "shader.h"
#include "mat4.h"
#include "renderwindow.h"

RenderSystem::RenderSystem(RenderWindow *renderWindow) : SystemBase(), mRenderWindow(renderWindow)
{
}

void RenderSystem::drawMesh(MeshComponent *mesh, MaterialComponent *material, TransformComponent *transformComponent)
{
    initializeOpenGLFunctions();

    material->mMaterial->getShader()->Use();
    mRenderWindow->checkForGLerrors();
    gsl::Mat4 tempMat4 = transformComponent->mTransform.getModelMatrix();

    material->mMaterial->setMMatrix(&tempMat4);
    mRenderWindow->checkForGLerrors();

    glPointSize(2.f);

    glBindVertexArray(mesh->mVAO);
    mRenderWindow->checkForGLerrors();

    if(mesh->mNumberOfIndices == 0)
    {
        glDrawArrays(mesh->mDrawType, 0, mesh->mNumberOfVertices);
        mRenderWindow->checkForGLerrors();
    }
    else
    {
        glDrawElements(mesh->mDrawType, mesh->mNumberOfIndices, GL_UNSIGNED_INT, nullptr);
        mRenderWindow->checkForGLerrors();
    }
}
