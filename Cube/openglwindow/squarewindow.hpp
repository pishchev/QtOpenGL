#ifndef SQUAREWINDOW_HPP
#define SQUAREWINDOW_HPP
#include "openglwindow.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>
#include <QDialog>
#include <QColorDialog>
#include <QCursor>
#include <QCheckBox>

#include <vector>


class SquareWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    SquareWindow():button1("Lines" , &dialog) , button2("Fill" , &dialog){}

    void initialize() override;
    void render() override;

    void formalDrawing(bool color);

private:

    QColorDialog dialog;
    QCursor cursor;
    QCheckBox button1;
    QCheckBox button2;

    //QScreen* screen;

    GLfloat colors[54];

    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};

#endif // SQUAREWINDOW_HPP
