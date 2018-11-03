#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QOpenGLFunctions_4_1_Core>
#include "systembase.h"

struct MeshComponent;
struct MaterialComponent;
struct TransformComponent;
class RenderWindow;

/// A system that renders a MeshComponent using a MaterialComponent
class RenderSystem : public SystemBase,  protected QOpenGLFunctions_4_1_Core
{
public:
    RenderSystem(RenderWindow *renderWindow);

//    virtual void init();
    void drawMesh(MeshComponent *mesh, MaterialComponent *material, TransformComponent *transformComponent);
//    virtual void update();

    RenderWindow *mRenderWindow{nullptr};

};

#endif // RENDERSYSTEM_H
