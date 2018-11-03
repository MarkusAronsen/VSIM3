QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = INNgine

RC_ICONS = INNgine.ico

INCLUDEPATH +=  ./GSL \
                ./Resources \
                ./Editor \
                ./ECS   \
                ./ECS/Components \
                ./ECS/Systems

SOURCES += main.cpp \
    renderwindow.cpp \
    Resources/shader.cpp \
    Editor/mainwindow.cpp \
    vertex.cpp \
    Resources/octahedron.cpp \
    Resources/texture.cpp \
    transform.cpp \
    material.cpp \
    colormaterial.cpp \
    texturematerial.cpp \
    camera.cpp \
    controlledball.cpp \
    pointlight.cpp \
    lightcolormaterial.cpp \
    GSL/vec4.cpp \
    GSL/vec3.cpp \
    GSL/vec2.cpp \
    GSL/quaternion.cpp \
    GSL/gsl_math.cpp \
    ECS/entity.cpp \
    Resources/meshbase.cpp \
    ECS/Systems/rendersystem.cpp \
    ECS/Systems/systembase.cpp \
    coreengine.cpp \
    GSL/mat2.cpp \
    GSL/mat3.cpp \
    GSL/mat4.cpp \
    Resources/meshfactory.cpp \
    Resources/objmesh.cpp \
    Resources/lasmesh.cpp \
    Resources/LAS/lasreader.cpp \
    Resources/LAS/lasreader_alex.cpp \
    terrain.cpp \
    terrainball.cpp \
    bspline.cpp \
    controlpoint.cpp

HEADERS += \
    renderwindow.h \
    Resources/shader.h \
    Editor/mainwindow.h \
    vertex.h \
    Resources/octahedron.h \
    Resources/texture.h \
    transform.h \
    constants.h \
    material.h \
    colormaterial.h \
    texturematerial.h \
    camera.h \
    controlledball.h \
    pointlight.h \
    lightcolormaterial.h \
    GSL/vec4.h \
    GSL/vec3.h \
    GSL/vec2.h \
    GSL/matrix.h \
    GSL/math_constants.h \
    GSL/gsl_math.h \
    GSL/quaternion.h \
    ECS/Components/componentbase.h \
    ECS/entity.h \
    gltypes.h \
    Resources/meshbase.h \
    ECS/Systems/rendersystem.h \
    ECS/Systems/systembase.h \
    coreengine.h \
    GSL/mat2.h \
    GSL/mat3.h \
    GSL/mat4.h \
    Resources/meshfactory.h \
    Resources/objmesh.h \
    Resources/lasmesh.h \
    Resources/LAS/lasreader.h \
    Resources/LAS/headerstock.h \
    Resources/LAS/lasreader_alex.h \
    terrain.h \
    terrainball.h \
    bspline.h \
    controlpoint.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainfragment.frag \
    plainvertex.vert \
    textureshader.frag \
    textureshader.vert \
    lightcolor.vert \
    lightcolor.frag

RESOURCES += \
    programicons.qrc
