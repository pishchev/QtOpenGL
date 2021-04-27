#ifndef OBJECT_H
#define OBJECT_H
#include <QOpenGLShaderProgram>
#include <vector>
#include <array>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>


#include "objectmodel.h"


struct Vertex
{
    std::array<GLfloat , 3> vertex;
    std::array<GLfloat , 3> normal;
    std::array<GLfloat , 2> texCoord;
};

class Object
{
public:
    Object();

    std::vector<Vertex> vertexs;

    QOpenGLBuffer *m_vbo = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;

    std::unique_ptr<QOpenGLTexture> texture, normalMap;

    ObjectModel model;

};

#endif // OBJECT_H
