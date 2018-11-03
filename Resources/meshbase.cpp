#include "meshbase.h"
#include "vertex.h"

MeshBase::MeshBase()
{

}

MeshBase::MeshBase(GLenum drawType, std::initializer_list<Vertex> vlist)
{
    mDrawType = drawType;
    mNumberOfVertices = vlist.size();

    int k;

    if(mNumberOfVertices > 0)
    {
        k = 0;
        mVertices = new Vertex[mNumberOfVertices];
        for(auto v : vlist)
            mVertices[k++] = v;
    }

    //init(); Init the MeshBase when we have a material attached
}

MeshBase::MeshBase(GLenum drawType, std::initializer_list<Vertex> vlist, std::initializer_list<GLuint> ilist)
{
    mDrawType = drawType;
    mNumberOfVertices = vlist.size();
    mNumberOfIndices = ilist.size();

    int k;

    if(mNumberOfVertices > 0)
    {
        k = 0;
        mVertices = new Vertex[mNumberOfVertices];
        for(auto v : vlist)
            mVertices[k++] = v;
    }
    if(mNumberOfIndices > 0)
    {
        useIndexArray = true;
        k = 0;
        mIndices = new GLuint[mNumberOfIndices];
        for(auto i : ilist)
            mIndices[k++] = i;
    }

    //init(); Init the MeshBase when we have a material attached
}

MeshBase::~MeshBase()
{

}

void MeshBase::init()
{
    initializeOpenGLFunctions();

    if(!mVertices)
        return;

    if (mNumberOfIndices > 0)
        useIndexArray = true;

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices * sizeof(Vertex), mVertices, GL_STATIC_DRAW);

    if(useIndexArray)
    {
        glGenBuffers(1, &mEAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumberOfIndices * sizeof(GLuint), mIndices, GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}
