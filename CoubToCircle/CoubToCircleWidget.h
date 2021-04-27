#ifndef COUBTOCIRCLEWIDGET_H
#define COUBTOCIRCLEWIDGET_H

#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "coub.h"


class CoubToCircleWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:

void initializeGL() override;
void paintGL() override;
void setPercistenceLvl(int lvl);
void setColor(QColorDialog* cd);
void setFactor(float nf);
QColor getColor();

CoubToCircleWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()):QOpenGLWidget(parent,f), timer(new QTimer(this)){}

~CoubToCircleWidget(){}

private:
QOpenGLBuffer *m_vbo = nullptr;
QOpenGLVertexArrayObject *m_vao = nullptr;

Coub coubVert;

float factor=0;

QColor color;
QColorDialog* colorDialog = nullptr;

GLint m_posAttr = 0;
GLint m_colAttr = 0;
GLint m_matrixUniform = 0;
GLint m_ctsfct =0;

QOpenGLShaderProgram *m_program = nullptr;
int m_frame = 0;
QTimer* timer = nullptr;
};

#endif // COUBTOCIRCLEWIDGET_H
