#include "coub.h"

Coub::Coub()
{
    vert = coubVertex();
}

std::vector<GLfloat> Coub::side()
{
    std::vector<GLfloat> vec ={};
    for (GLfloat x = -1.0f ; x <= 1.01f-step ;x += step )
    {
        for (GLfloat y = -1.0f ; y <=1.01f-step ;y += step )
        {
            vec.push_back(x     );vec.push_back(y     );vec.push_back(1.0f);
            vec.push_back(x+step);vec.push_back(y     );vec.push_back(1.0f);
            vec.push_back(x     );vec.push_back(y+step);vec.push_back(1.0f);
            vec.push_back(x     );vec.push_back(y+step);vec.push_back(1.0f);
            vec.push_back(x+step);vec.push_back(y     );vec.push_back(1.0f);
            vec.push_back(x+step);vec.push_back(y+step);vec.push_back(1.0f);
        }
    }
    return vec;
}
std::vector<GLfloat> Coub::slideSide(types type)
{
    auto vec = side();
    int slide = 0;
    if (type==Z)
    {
        slide = 0;
    }
    else if(type ==Y)
    {
        slide = 2;
    }
    else if(type == X)
    {
        slide = 1;
    }
    for(size_t i = 0 ;i< vec.size()/3 ; i++)
    {
        std::vector<GLfloat> v = {vec[i*3],vec[i*3+1],vec[i*3+2]};
        std::rotate(v.rbegin(), v.rbegin() + slide, v.rend());
        for (int k = 0 ; k< 3 ; k ++)
        {
            vec[i*3+k] = v[k];
        }

    }
    return vec;
}
std::vector<GLfloat> Coub::coubVertex()
{
    std::vector<GLfloat>result = {};
    std::vector<GLfloat> v ={};

    v = slideSide(X);
    result.insert(result.end() , v.begin() , v.end());
    v = slideSide(Y);
    result.insert(result.end() , v.begin() , v.end());
    v = slideSide(Z);
    result.insert(result.end() , v.begin() , v.end());

    std::vector<GLfloat> reverse ={};
    v = slideSide(X);
    reverse.insert(reverse.end() , v.begin() , v.end());
    v = slideSide(Y);
    reverse.insert(reverse.end() , v.begin() , v.end());
    v = slideSide(Z);
    reverse.insert(reverse.end() , v.begin() , v.end());

    for (auto it = reverse.begin(); it!= reverse.end();++it)
    {
        *it *=-1;
    }
    result.insert(result.end() , reverse.begin() , reverse.end());
    return result;
}
const std::vector<GLfloat>& Coub::getVertex()
{
    return vert;
}
size_t Coub::vertexCount()
{
    return vert.size();
}
void Coub::setStep(GLfloat stp)
{
    step  = stp;
    vert = coubVertex();
}
