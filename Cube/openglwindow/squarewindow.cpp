#include "squarewindow.hpp"
#include "shaders.hpp"
#include "square.hpp"


void SquareWindow::initialize()
{
    dialog.show();
    dialog.setOption(QColorDialog::NoButtons);
    dialog.setCurrentColor("red");

    button1.move(15,230);
    button2.move(80,230);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, shaders::vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, shaders::fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);

}

void SquareWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2.0f);

    //вращение мышкой
    QPoint coord = cursor.pos() - this->position() - QPoint(this->width()/2, this->height()/2);
    auto speed = sqrt( coord.y()* coord.y()+ coord.x()* coord.x())/5;
    //speed *= m_frame / screen()->refreshRate(); //если очень хочется чтобы по дефолту вращалось
    matrix.rotate(speed, coord.y(), coord.x(), 0);

    m_program->setUniformValue(m_matrixUniform, matrix);


    //отсечения
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //грани
    if (button2.isChecked())
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        formalDrawing(true);
    }

    //ребра
    if (button1.isChecked())
    {
        glPolygonMode(GL_FRONT, GL_LINE);
        formalDrawing(false);
    }

    m_program->release();

    ++m_frame;
}


void SquareWindow::formalDrawing(bool color)
{
    qreal r=255,g=255,b=255;
    (dialog.currentColor()).getRgbF(&r,&g,&b);
    if (color)
    {
        r = r/100 * 40;
        g = g/100 * 40;
        b = b/100 * 40;
    }
    for (int i = 0 ; i < 18 ; i ++)
    {
        colors[i*3] = r;
        colors[i*3+1] =g;
        colors[i*3+2] =b;
    }

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
    glDrawArrays(GL_TRIANGLE_STRIP, 10, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 14, 4);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
}























