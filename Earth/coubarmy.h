#ifndef COUBARMY_H
#define COUBARMY_H

#include "coub.h"

class coubArmy
{
public:
    coubArmy();

    const std::vector<GLfloat>& getVertex();

    void setCoub(const Coub& cube);
    size_t vertexCount();

    void setCol(int c);
    void setRow(int r);

    void makeArmy();
private:
    int rows = 1;
    int cols = 1;

    GLfloat dist = 3.0f;

    std::vector<GLfloat> vert ={};

    Coub origin;
};

#endif // COUBARMY_H
