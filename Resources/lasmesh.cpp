#include "lasmesh.h"
#include "vertex.h"
#include "LAS/lasreader.h"

LasMesh::LasMesh(const std::string &filePath)
{
    mDrawType = GL_TRIANGLES;
    mFilePath = filePath;

    readLasFile();
}

void LasMesh::readLasFile()
{
    LASReader reader(mFilePath);

    GLuint realHeight;
    GLuint realWidth;
    double** downscaled = reader.makeHeightmapFromPointData(realHeight, realWidth);

    GLfloat sX = -static_cast<GLfloat>(realWidth)/2.f, sZ = -static_cast<GLfloat>(realHeight)/2.f;

    mNumberOfVertices = static_cast<GLint>(realHeight*realWidth);
    mVertices = static_cast<Vertex*>(malloc(sizeof(Vertex)* static_cast<GLuint>(mNumberOfVertices)));
    // Create a terrain that is realWidth x realHeight
    // Where we plot in our heightmap data
    for(GLuint i = 0; i < realHeight; i++)
    {
        for(GLuint j = 0; j < realWidth; j++)
        {
            mVertices[i*realWidth + j] = {Vec3(sX, static_cast<GLfloat>(downscaled[i][j]), sZ), Vec3(0, 1, 0), Vec2()};
            sX += 1;
        }
        sZ += 1;
        sX = -static_cast<GLfloat>(realWidth)/2.f;
    }

    // Create an index array to save space
    mNumberOfIndices = static_cast<int>(6*(realHeight-1)*(realWidth-1));
    mIndices = new GLuint[mNumberOfIndices];

    GLuint k = 0;
    for(GLuint i = 0; i < realHeight-1; i++)
    {
        for(GLuint j = 0; j < realWidth-1; j++)
        {
            mIndices[k] = static_cast<int>(realWidth*i + realWidth + j);
            mIndices[k+1] = static_cast<int>(realWidth*i + j+1);
            mIndices[k+2] = static_cast<int>(realWidth*i + j);

            mIndices[k+3] = static_cast<int>(realWidth*i + realWidth + j);
            mIndices[k+4] = static_cast<int>(realWidth*i + realWidth + 1 + j);
            mIndices[k+5] = static_cast<int>(realWidth*i + j+1);
            k += 6;
        }
    }

    // We have no need for the heightmap anymore
    for(unsigned int i = 0; i < realHeight; i++)
        delete[] downscaled[i];
    delete[] downscaled;
}
