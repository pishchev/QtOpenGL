#ifndef BALL_H
#define BALL_H

#include "object.h"

class Ball
{
public:
    Ball();
    Ball(Object obj , int number , float radius);

    void render(QOpenGLShaderProgram *m_program ,QOpenGLFunctions* scene);

    void move();

    static void collision(Ball& b1 , Ball& b2);

    float dx = 2;
    float dz = 2;

    float radius = 1.0f;

    Object obj;
    int number;
};

class BallsPool
{
public:
    BallsPool();

    void addBall(int number, Ball ball);

    void move();

    void render(QOpenGLShaderProgram *m_program ,QOpenGLFunctions* scene);

    std::map<int , Ball> balls = {};
};

#endif // BALL_H
