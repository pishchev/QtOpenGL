#include "sphere.h"
#include <cmath>

Sphere::Sphere(float radius,float stackCount,float sectorCount)
{
    rad = radius;
    buildVertex(stackCount,sectorCount);
    buildIndexes(stackCount,sectorCount);

}

void Sphere::buildVertex(float stackCount,float sectorCount)
{
    // clear memory of prev arrays
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);


    float radius = rad;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }
}
void Sphere::buildIndexes(float stackCount,float sectorCount)
{

    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

std::vector<GLfloat> Sphere::getVertex()
{
    std::vector<GLfloat> v;

    for (auto it : indices)
    {
        v.push_back(vertices[it*3]);
        v.push_back(vertices[it*3+1]);
        v.push_back(vertices[it*3+2]);
    }
    return v;
}
std::vector<GLfloat> Sphere::getNormals()
{
    std::vector<GLfloat> v;

    for (auto it : indices)
    {
        v.push_back(normals[it*3]);
        v.push_back(normals[it*3+1]);
        v.push_back(normals[it*3+2]);
    }
    return v;
}
std::vector<GLfloat> Sphere::getTexCoord()
{
    std::vector<GLfloat> v;

    for (auto it : indices)
    {
        v.push_back(texCoords[it*2]);
        v.push_back(texCoords[it*2+1]);
    }
    return v;
}
std::vector<Vertex> Sphere::getVertexs()
{
    std::vector<Vertex> vertexs;
    Vertex a;

    for (auto it : indices)
    {
        a.texCoord = {texCoords[it*2],texCoords[it*2+1]};
        a.normal = {normals[it*3],normals[it*3+1],normals[it*3+2]};
        a.vertex = {vertices[it*3],vertices[it*3+1],vertices[it*3+2]};

        vertexs.push_back(a);
    }
    return vertexs;
}
