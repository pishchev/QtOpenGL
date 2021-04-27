#ifndef OBJECT_H
#define OBJECT_H
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <vector>
#include <array>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>
#include <string>

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
    //Object(const Object&) = default;
    void Init(std::vector<Vertex> verts,
                GLfloat rtSpeed,
                std::string textFile,
                std::string normFile,
                QOpenGLShaderProgram *m_program);

    virtual void render(QOpenGLShaderProgram *m_program ,QOpenGLFunctions* scene);

    std::vector<Vertex> vertexs;

    QOpenGLBuffer *m_vbo = nullptr;
    QOpenGLVertexArrayObject *m_vao = nullptr;

    std::string textName = "textureMap";
    std::string normName = "normalMap";

    std::shared_ptr<QOpenGLTexture> texture, normalMap;

    ObjectModel model;

};

#endif // OBJECT_H
