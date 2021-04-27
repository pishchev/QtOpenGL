#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <QOpenGLShaderProgram>
#include "object.h"
#include <string>
class MeshLoader
{
public:
    MeshLoader();

    static Object loadMesh(const std::string& filename);
};

#endif // MESHLOADER_H
