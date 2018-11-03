#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace gsl  //Game School Lib
{
    const std::string assetFilePath{"../INNgine/Assets/"};
    const std::string shaderFilePath{"../INNgine/"};

    enum ShaderTypes
    {
        COLOR,
        TEXTURE,
        LIGHTCOLOR,
        LIGHTTEXTURE
    };

    enum ShapeType
    {
        CUBE,
        PLANE,
        SPHERE,
        AXIS
    };

    enum ComponentType
    {
        MESH,
        MATERIAL,
        TRANSFORM,
        CAMERA,

        LAST
    };
}

#endif // CONSTANTS_H
