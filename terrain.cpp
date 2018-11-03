#include "terrain.h"

Terrain::Terrain(std::string fileName)
{
    mDrawType = GL_TRIANGLES;
    readFile(fileName);
    findFrame();
    centralizeData();
    generateVertices();
    generateIndices();
    setHeights();
    setNormals();

    init();
}

void Terrain::readFile(std::string fileName)
{
    std::ifstream read(fileName);
    float xTemp, yTemp, zTemp;
    while(!read.eof())
    {
        read >> xTemp >> yTemp >> zTemp;
        inData.push_back(Vec3(xTemp, yTemp, zTemp));
    }

}

void Terrain::findFrame()
{
    std::vector<float> x;
    std::vector<float> y;
    for(unsigned int i = 0; i < inData.size(); i++)
    {
        x.push_back(inData[i].getX());
        y.push_back(inData[i].getY());
    }

    std::vector <float>::iterator it = std::min_element(x.begin(), x.end());
    xMin = *it;
    it = std::max_element(x.begin(), x.end());
    xMax = *it;
    it = std::min_element(y.begin(), y.end());
    yMin = *it;
    it = std::max_element(y.begin(), y.end());
    yMax = *it;

}

void Terrain::centralizeData()
{
    for(unsigned int i = 0; i < inData.size(); i++)
    {
        inData[i].setX(inData[i].getX() - xMin);
        inData[i].setY(inData[i].getY() - yMin);
    }

    xMax = xMax - xMin;
    yMax = yMax - yMin;
    xMin = xMin - xMin;
    yMin = yMin - yMin;

    mNumberOfVertices = (yMax - yMin) * (xMax - xMin);
    mNumberOfIndices = (yMax - yMin - 1) * (xMax - xMin - 1) * 2 * 3;
    mIndices = new GLuint[mNumberOfIndices];
    mVertices = new Vertex[mNumberOfVertices];
}

void Terrain::generateVertices()
{

    for(int i = xMin; i < xMax; i++)
    {
        for(int j = yMin; j < yMax; j++)
        {
            mVertices[i*(yMax - yMin) + j].set_xyz(i, 0.f, j);
            mVertices[i*(yMax - yMin) + j].set_uv(0.f, 0.f);
        }
    }
}

void Terrain::generateIndices()
{
    int n = 0;
    int x = yMax - 1;
    for(GLuint i = 0; i < (mNumberOfVertices - yMax); i++)
    {
        if(i == x)
        {
            x += yMax;
            continue;
        }

        mIndices[n] = i;
        mIndices[n+1] = i + 1;
        mIndices[n+2] = i + yMax;

        mIndices[n+3] = i + yMax + 1;
        mIndices[n+4] = i + yMax;
        mIndices[n+5] = i + 1;

        n += 6;
    }
}

void Terrain::setHeights()
{
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i].set_xyz(mVertices[i].get_x(), findLocalVertices(i, 4.f), mVertices[i].get_z());
    }
}

float Terrain::findLocalVertices(int index, float radius)
{
    Vec2 tempVec = Vec2(mVertices[index].get_x(), mVertices[index].get_z());
    std::vector <float> heightValues;
    for(int i = 0; i < inData.size(); i++)
    {
        if((tempVec - Vec2(inData[i].getX(), inData[i].getY())).length() < radius)
        {
            heightValues.push_back(inData[i].getZ());
        }
    }
    if(heightValues.size() == 0)
        return -50.f;

    float averageHeight;

    for(int i = 0; i < heightValues.size(); i++)
    {
        averageHeight += heightValues[i];
    }
    averageHeight = averageHeight / heightValues.size();

    return averageHeight;
}

void Terrain::setNormals()
{
    Vec3 u;
    Vec3 v;
    int x = yMax - 1;

    for(int i = 0; i < mNumberOfVertices - yMax; i++)
    {
        if(i == x)
        {
            x += yMax;

            u = mVertices[i - 1].get_xyz() - mVertices[i].get_xyz();
            v = mVertices[i + yMax].get_xyz() - mVertices[i].get_xyz();
            mVertices[i].set_normal(v ^ u);

            continue;
        }

        u = mVertices[i+1].get_xyz() - mVertices[i].get_xyz();
        v = mVertices[i + yMax].get_xyz() - mVertices[i].get_xyz();
        mVertices[i].set_normal(u ^ v);
    }

    for(int i = mNumberOfVertices - yMax; i < mNumberOfVertices - 1; i++)
    {
        u = mVertices[i - yMax].get_xyz() - mVertices[i].get_xyz();
        v = mVertices[i + 1].get_xyz() - mVertices[i].get_xyz();
        mVertices[i].set_normal(u ^ v);
    }

    u = mVertices[mNumberOfVertices - yMax - 1].get_xyz() - mVertices[mNumberOfVertices - 1].get_xyz();
    v = mVertices[mNumberOfVertices - 1 - 1].get_xyz() - mVertices[mNumberOfVertices - 1].get_xyz();
    mVertices[mNumberOfVertices - 1].set_normal(v ^ u);

    struct vertexNormal
    {
        Vertex v;
        std::vector <Vec3> normals;
    };
    std::vector <vertexNormal> vertexNormals;
    std::vector <Triangle> triangles;

    for(int i = 0; i < mNumberOfVertices; i++)
    {
        vertexNormals.push_back(vertexNormal());
        vertexNormals[i].v = mVertices[i];
    }

    for(int i = 0; i < mNumberOfIndices; i += 6)
    {
        triangles.push_back(Triangle(mVertices[mIndices[i]].get_xyz(), mVertices[mIndices[i+1]].get_xyz(), mVertices[mIndices[i+2]].get_xyz()));
        u = triangles[triangles.size() - 1].r - triangles[triangles.size() - 1].p;
        v = triangles[triangles.size() - 1].q - triangles[triangles.size() - 1].p;
        triangles[triangles.size() - 1].normal = (v ^ u).normalized();

        vertexNormals[mIndices[i]].normals.push_back(triangles[triangles.size() - 1].normal);
        vertexNormals[mIndices[i + 1]].normals.push_back(triangles[triangles.size() - 1].normal);
        vertexNormals[mIndices[i + 2]].normals.push_back(triangles[triangles.size() - 1].normal);

        triangles.push_back(Triangle(mVertices[mIndices[i+3]].get_xyz(), mVertices[mIndices[i+4]].get_xyz(), mVertices[mIndices[i+5]].get_xyz()));
        u = triangles[triangles.size() - 1].r - triangles[triangles.size() - 1].p;
        v = triangles[triangles.size() - 1].q - triangles[triangles.size() - 1].p;
        triangles[triangles.size() - 1].normal = (v ^ u).normalized();

        vertexNormals[mIndices[i + 3]].normals.push_back(triangles[triangles.size() - 1].normal);
        vertexNormals[mIndices[i + 4]].normals.push_back(triangles[triangles.size() - 1].normal);
        vertexNormals[mIndices[i + 5]].normals.push_back(triangles[triangles.size() - 1].normal);
    }

    Vec3 averageNormal;

    for(int i = 0; i < mNumberOfVertices; i++)
    {
        for(int j = 0; j < vertexNormals[i].normals.size(); j++)
        {
            averageNormal += vertexNormals[i].normals[j];
        }
        mVertices[i].set_normal(averageNormal.normalized());
        averageNormal = Vec3(0.f, 0.f, 0.f);
    }

}
