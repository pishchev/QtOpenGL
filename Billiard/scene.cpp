#include "scene.h"
#include <QScreen>


void Scene::initializeGL()
{
    initProgram();
    initObject();
    mouse.mouseX = QCursor::pos().x();
    mouse.mouseY = QCursor::pos().y();
}
void Scene::paintGL()
{   
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    glEnable(GL_DEPTH_TEST);

    keyEvent();
    mouseEvent();

    lightUniforms();

    m_program->setUniformValue("viewmatrix" , camera.getPerspective()*camera.getView());
    m_program->setUniformValue("camPos" , QVector4D(camera.getPos(),1));

    for (auto obj = balls.begin() ; obj!= balls.end(); ++obj)
    {
        obj->render(m_program , this);
    }

    for (auto obj = table.begin() ; obj!= table.end(); ++obj)
    {
        obj->render(m_program , this);
    }

    for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
    {
        obj->render(m_program , this);
    }

    //pointLight.render(m_program , this);

    m_program->release();
    ++m_frame;

}

void Scene::keyPressEvent(QKeyEvent* event)
{
    if (keys.keys.count(event->key()))
    {
        keys.keys[event->key()]= true;
    }
}
void Scene::keyReleaseEvent(QKeyEvent* event)
{
    if (keys.keys.count(event->key()))
    {
        keys.keys[event->key()]= false;
    }
}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
    int dx = QCursor::pos().x() - mouse.mouseX;
    int dy = QCursor::pos().y() - mouse.mouseY;

    mouse.mouseX = QCursor::pos().x();
    mouse.mouseY = QCursor::pos().y();

    if (mouse.pressed)
    {
        camera.rotate(-1*(float)dy/5,-1*(float)dx/5);
    }
    mouse.pressed = true;
}

void Scene::mouseEvent()
{
    mouse.pressed = false;
}

void Scene::keyEvent()
{
    GLfloat camera_speed = 0.2;
    if (keys.keys[Qt::Key_A])
    {
        camera.step(0,-camera_speed);
    }
    if (keys.keys[Qt::Key_D])
    {
        camera.step(0,camera_speed);
    }
    if (keys.keys[Qt::Key_W])
    {
        camera.step(camera_speed,0);
    }
    if (keys.keys[Qt::Key_S])
    {
        camera.step(-camera_speed,0);
    }
    if (keys.keys[Qt::Key_Space])
    {
        camera.fly(camera_speed);
    }
    if (keys.keys[Qt::Key_Control])
    {
        camera.fly(-camera_speed);
    }

    if (keys.keys[Qt::Key_Up])
    {
        for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
        {
            obj->model.tz -= 0.5;
        }
    }
    if (keys.keys[Qt::Key_Down])
    {
        for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
        {
            obj->model.tz += 0.5;
        }
    }
    if (keys.keys[Qt::Key_Left])
    {
        for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
        {
            obj->model.tx -= 0.5;
        }
    }
    if (keys.keys[Qt::Key_Right])
    {
        for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
        {
            obj->model.tx += 0.5;
        }
    }
    if (keys.keys[Qt::Key_0])
    {
        for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
        {
            obj->model.ty -= 0.5;
        }
    }
    if (keys.keys[Qt::Key_1])
    {
        for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
        {
            obj->model.ty += 0.5;
        }
    }
}

void Scene::lightUniforms()
{
    m_program->setUniformValue("ka" , coefs.ka);
    m_program->setUniformValue("kd" , coefs.kd);
    m_program->setUniformValue("k", coefs.k);
    m_program->setUniformValue("p" , coefs.p);
    m_program->setUniformValue("ks" , coefs.ks);
    m_program->setUniformValue("n" , coefs.n);

    m_program->setUniformValue("ambientColor", QColor(255,255,255));

    QVector4D lightsPoints[4];
    QVector4D lightsColors[4];

    for (int i = 0; i < 4 ; i ++)
    {
        lightsPoints[i] = QVector4D(lights[i].model.tx,
                lights[i].model.ty,
                lights[i].model.tz,1);

        lightsColors[i] = QVector4D(lights[i].getColor().redF(),
                lights[i].getColor().greenF(),
                lights[i].getColor().blueF(),1);
    }

    m_program->setUniformValueArray("pointColor" , lightsColors , 4);
    m_program->setUniformValueArray("pointPos", lightsPoints, 4);

}

void Scene::initProgram()
{
    initializeOpenGLFunctions();
    f = QOpenGLContext::currentContext()->functions();
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\source\\repos\\Qt\\Billiards\\Billiards\\vertexShaider.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\source\\repos\\Qt\\Billiards\\Billiards\\fragmentShaider.glsl");
    m_program->link();
    m_program->bind();
}

void Scene::initObject()
{

    Sphere s(0.60,50,50);
    for (int i = 0 ; i< 4 ; i++)
    {
        for (int k = 0 ; k< 4 ; k++)
        {
            Object o;

            std::string folder = "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\";

            int num  = i*4+k;
            if (num == 0)continue;

            std::string img_ =std::to_string(num)+".jpg";

            o.Init(s.getVertexs(),
                   screen()->refreshRate()/100,
                   folder+img_,
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
                   m_program);
            o.model.setTranslate(-4+2*k , 6.5f , 2*i-10);
            o.model.rotate(true);
            balls.push_back(o);

        }
    }

    {
        Object o;
        o.Init(Surface::surface(14.2f,27.6f,15,15),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\f.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
               m_program);
        o.model.setTranslate(0 , 5.75f , -6);
        table.push_back(o);
    }
    {
        Object o;
        o.Init(MeshLoader::loadMesh("D:\\source\\repos\\Qt\\Billiards\\Billiards\\meshes\\MeshTableOnly2.obj"),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\Wood.png",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
               m_program);
        o.model.setTranslate(0 , -6 , -6);
        table.push_back(o);
    }
    {
        for (int i = 0 ; i < 4; ++i)
        {
            LightObject o;
            Sphere s2(0.6,50,50);
            o.Init(s2.getVertexs(),
                   screen()->refreshRate()/100,
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\Wood.png",
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
                   m_program);
            o.model.setTranslate(0,24 , -25 + i*12.5f);
            lights.push_back(o);
        }

    }


}
void Scene::Filtering(const Object& o, int i)
{
    if (i == 0)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        o.texture->setMaximumAnisotropy(0);
    }
    else if(i == 1)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapNearest);
        o.texture->setMaximumAnisotropy(0);
    }
    else if(i == 2)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        o.texture->setMaximumAnisotropy(0);
    }
    else if(i == 3)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        o.texture->setMaximumAnisotropy(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);
    }
}


void Scene::setLightCoefs(std::vector<int> vec)
{
    coefs.ka = static_cast<float>(vec[0])/100;
    coefs.kd = vec[1];
    coefs.k = vec[2];
    coefs.p = vec[3];
    coefs.n = vec[4];
    coefs.ks = vec[5];
}

