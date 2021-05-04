#include "triangle.h"

Triangle::Triangle()
{

}

std::vector<Vertex> Triangle::triangle()
{
    float size = 0.5f;
    float offset = 1.5f;

    std::vector <Vertex> vert;

    Vertex a;
    a.normal = {0,1,0};

    a.vertex = {-size,0,size+offset};
    a.texCoord = {0,0};
    vert.push_back(a);

    a.vertex = {size,0,size+offset};
    a.texCoord = {1,0};
    vert.push_back(a);

    a.vertex = {0,0,-size+offset};
    a.texCoord = {0.5f,1};
    vert.push_back(a);

    return vert;

}
