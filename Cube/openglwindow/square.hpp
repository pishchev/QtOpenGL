#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <QOpenGLShaderProgram>

const GLfloat vertices[] = {
    //стороны
    -0.5f ,  0.5f ,  0.5f ,
    -0.5f , -0.5f ,  0.5f ,
     0.5f ,  0.5f ,  0.5f ,
     0.5f , -0.5f ,  0.5f ,
     0.5f ,  0.5f , -0.5f ,
     0.5f , -0.5f , -0.5f ,
    -0.5f ,  0.5f , -0.5f ,
    -0.5f , -0.5f , -0.5f ,
    -0.5f ,  0.5f ,  0.5f ,
    -0.5f , -0.5f ,  0.5f ,
    //верх
    -0.5f ,  0.5f ,  0.5f ,
     0.5f ,  0.5f ,  0.5f ,
    -0.5f ,  0.5f , -0.5f ,
     0.5f ,  0.5f , -0.5f ,
    //низ
    -0.5f ,  -0.5f , -0.5f ,
     0.5f ,  -0.5f , -0.5f ,
    -0.5f ,  -0.5f ,  0.5f ,
     0.5f ,  -0.5f ,  0.5f ,
};


#endif // SQUARE_HPP
