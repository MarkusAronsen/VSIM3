#ifndef MESHBASE_H
#define MESHBASE_H

#include <QOpenGLFunctions_4_1_Core>
#include <initializer_list>
#include "gltypes.h"

class Vertex;

class MeshBase : protected QOpenGLFunctions_4_1_Core
{
public:
    MeshBase();
    MeshBase(GLenum drawType, std::initializer_list<Vertex> vlist);
    MeshBase(GLenum drawType, std::initializer_list<Vertex> vlist, std::initializer_list<GLuint> ilist);
    ~MeshBase();

    void init();

    Vertex* vertices() const;
    GLint* indices() const;

    Vertex* mVertices{nullptr};
    GLuint* mIndices{nullptr};

    GLenum mDrawType;           //the way this object should be rendered
    GLuint mNumberOfVertices{0};
    GLuint mNumberOfIndices{0};
    bool useIndexArray{false};

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAO{0};

    std::string getFilePath() const {return mFilePath;}

    std::string mFilePath;
};

#endif // MESHBASE_H
