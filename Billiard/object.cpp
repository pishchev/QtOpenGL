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

void Object::render(QOpenGLShaderProgram *m_program ,QOpenGLFunctions* scene)
{

    m_program->setUniformValue("rtmatrix", model.getRotate());
    m_program->setUniformValue("trmatrix" , model.getTranslate());

    m_program->setUniformValue(textName.data(),0);
    m_program->setUniformValue(normName.data(),1);

    m_vao->bind();
    scene->glActiveTexture(GL_TEXTURE0);
    texture->bind();
    scene->glActiveTexture(GL_TEXTURE1);
    normalMap->bind();
    glPolygonMode(GL_FRONT, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, vertexs.size());
}
