#ifndef COUBSARMYWIDGET_H
#define COUBSARMYWIDGET_H

#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QLabel>
#include <QTime>
#include <QOpenGLTexture>
#include "coub.h"
#include "objectmodel.h"
#include "normals.h"
#include "object.h"
#include "sourcelight.h"
#include "colorowner.h"
#include "sphere.h"
#include "meshloader.h"
#include "surface.h"

class CoubsArmy : public QOpenGLWidget, protected QOpenGLFunctions
{
public:

void initializeGL() override;
void initProgram();
void initShadersParam();
void initObject();
void initColors();

void paintGL() override;
void lightUniforms();

void setSphere(float x, float y);
void setPosition(float x ,float y,float z);
void setColor(QColorDialog* cd , int active);
void setFactor(float nf);
void setRotate(bool fl);
void setLightCoefs(std::vector<int> vec);
void setActivateLight(std::vector<bool>vec);

void setDirectionLightPosition(QVector4D pos);
void setPointLightPosition(QVector4D pos);

void SetFilterMode(int filterMode);
void SetWrapMode(int wrapMode);
void PassParamsToShaider();

void Filtering(const Object& o, int i);

CoubsArmy(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()):QOpenGLWidget(parent,f), timer(new QTimer(this)){}

~CoubsArmy(){}

private:

std::vector<Object> objects;

SourceLight directionLight;
SourceLight ambientLight;
SourceLight pointLight;

LightFactors coefs;

ColorOwner colorOwner;

GLint m_norAttr = 0;
GLint m_posAttr = 0;

GLint m_trMatrix =0;
GLint m_matrixUniform = 0;

int m_minFilterMode  =0 ;
int m_magFilterMode =0 ;
int m_useAniso =0 ;
int m_wrapMode =0 ;

QOpenGLFunctions *f = nullptr;
QOpenGLShaderProgram *m_program = nullptr;
int m_frame = 0;
QTimer* timer = nullptr;
};

#endif // COUBSARMYWIDGET_H
