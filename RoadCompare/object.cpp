#include "object.h"

Object::Object()
{

}

void Object::Init(std::vector<Vertex> verts,
                    GLfloat rtSpeed,
                    std::string textFile,
                    std::string normFile,
                    QOpenGLShaderProgram *m_program)
{
    vertexs = verts;
    model.setStartPosition(0.0f, 0.0f, -2.5f);
    model.setRotate(rtSpeed,1.0f,0.0f,0.0f);
    model.rotate(false);

    texture.reset(new QOpenGLTexture(QImage(textFile.data())));
    texture->setWrapMode(QOpenGLTexture::Repeat);
    normalMap.reset(new QOpenGLTexture(QImage(normFile.data())));

    m_vao = new QOpenGLVertexArrayObject;
    if (m_vao->create())
    m_vao->bind();

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    m_vbo->allocate( vertexs.data(), vertexs.size() * sizeof(Vertex));

    m_program->enableAttributeArray("posAttr");
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, offsetof(Vertex, vertex), 3, sizeof(Vertex));

    m_program->enableAttributeArray("norAttr");
    m_program->setAttributeBuffer("norAttr", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    m_program->enableAttributeArray("texCoord");
    m_program->setAttributeBuffer("texCoord", GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));

    m_vbo->release();
}
