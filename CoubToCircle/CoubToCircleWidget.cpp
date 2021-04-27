#include "CoubToCircleWidget.h"
#include <QScreen>


void CoubToCircleWidget::initializeGL()
{
    color.setRgb(255,0,0);

    initializeOpenGLFunctions();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\source\\repos\\Qt\\CoubToCircle\\vertexShaider.txt");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\source\\repos\\Qt\\CoubToCircle\\fragmentShaider.txt");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->uniformLocation("color");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    m_ctsfct = m_program->uniformLocation("factor");
    Q_ASSERT(m_ctsfct != -1);

    m_vao = new QOpenGLVertexArrayObject;
    if (m_vao->create())
    m_vao->bind();
    m_program->bind();

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    m_vbo->allocate( &coubVert.getVertex()[0], coubVert.vertexCount() * sizeof(GLfloat));

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
    nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
    reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_vbo->release();
}

void CoubToCircleWidget::paintGL()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_vao->bind();
    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -5.0f);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate()/3 ,0.5f , 0.5f, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    m_program->setUniformValue(m_ctsfct,factor );

    glEnable(GL_DEPTH_TEST);

    if (colorDialog)
    {
        color = colorDialog->currentColor();
    }

    QColor color2;
    color2.setRgb(color.red()*0.4 ,color.green()*0.4 , color.blue()*0.4 , color.alpha());
    m_program->setUniformValue(m_colAttr, color2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, coubVert.vertexCount()/3);

    m_program->setUniformValue(m_colAttr, color);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, coubVert.vertexCount()/3);

    m_program->release();
    ++m_frame;
}

void CoubToCircleWidget::setColor(QColorDialog* cd)
{
    colorDialog = cd;
}

void CoubToCircleWidget::setPercistenceLvl(int lvl)
{
    GLfloat step = static_cast<GLfloat>(2) / static_cast<GLfloat>(lvl);
    m_vbo->bind();
    coubVert.setStep(step);
    m_vbo->allocate( &coubVert.getVertex()[0], coubVert.vertexCount() * sizeof(GLfloat));
    m_vbo->release();

}

void CoubToCircleWidget::setFactor(float nf)
{
    factor = nf;
}

QColor CoubToCircleWidget::getColor()
{
    return color;
}
