#ifndef SOURCELIGHT_H
#define SOURCELIGHT_H
#include <QOpenGLShaderProgram>
#include <QColor>
#include <vector>
#include "object.h"

class LightObject : public Object
{
public:
    LightObject();
    virtual void render(QOpenGLShaderProgram *m_program ,QOpenGLFunctions* scene) override;
    void setColor(QColor color_);
    QColor getColor();

private:
    QColor color = QColor(255,255,255);
};

class LightFactors
{
public:
    LightFactors();

    float ka = 0.5;
    float kd = 40;
    float ks = 70;
    int n = 4;
    float k = 0.8f;
    int p = 2;
};

#endif // SOURCELIGHT_H
