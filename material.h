#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLFunctions_4_1_Core>
#include "mat4.h"
#include <memory>

class Shader;
class Camera;

using namespace gsl;

class Material : protected QOpenGLFunctions_4_1_Core
{
public:
    Material(Shader *shader);

    Shader *getShader() const;

    virtual void setMMatrix(Mat4* mMatrix) = 0;
    virtual void setupShader(GLuint VAOin);

    static void setCurrentCamera(Camera *currentCamera);

protected:

    static Camera* mCurrentCamera;
    Mat4* mMMatrix{nullptr};
    Shader* mShader{nullptr};
};

#endif // MATERIAL_H
