#include "ball.h"

Ball::Ball(Object obj , int number , float radius): obj(obj),number(number), radius(radius)
{
    dx = static_cast<float>(rand()%30)/100;
    dz = static_cast<float>(rand()%30)/100;
};

void Ball::move()
{
    obj.model.tx +=dx;
    obj.model.tz +=dz;

    if (obj.model.tx > 14.2 - radius || obj.model.tx < -14.2 + radius)
    {
        dx = -dx;
    }
    if (obj.model.tz > 27.6 - radius || obj.model.tz < -27.6 + radius)
    {
        dz = -dz;
    }
}

void Ball::render(QOpenGLShaderProgram *m_program ,QOpenGLFunctions* scene)
{
    obj.render(m_program ,scene);

}

BallsPool::BallsPool()
{

};

void BallsPool::addBall(int number, Ball ball)
{
    balls.emplace(number, ball);
};

void BallsPool::render(QOpenGLShaderProgram *m_program ,QOpenGLFunctions* scene)
{
    for (auto it = balls.begin() ; it!= balls.end() ; ++it)
    {
        it->second.render(m_program,scene);
    }
}

void BallsPool::move()
{
    for (auto it = balls.begin() ; it!= balls.end() ; ++it)
    {
        it->second.move();
    }
}
