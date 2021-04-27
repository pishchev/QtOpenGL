#ifndef COUB_H
#define COUB_H

#include <QOpenGLShaderProgram>
#include <vector>

class Coub
{
public:
    Coub();      
    const std::vector<GLfloat>& getVertex();
    size_t vertexCount();
    void setStep(GLfloat stp);
private:
    enum types {X,Y,Z};
    std::vector<GLfloat> vert ={};
    GLfloat step = 0.2f;

    std::vector<GLfloat> side();
    std::vector<GLfloat> slideSide(types type);
    std::vector<GLfloat> coubVertex();
};

#endif // COUB_H
