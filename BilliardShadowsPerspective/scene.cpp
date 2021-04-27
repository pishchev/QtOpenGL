#include "scene.h"
#include <QScreen>

void Scene::initializeGL()
{
    initProgram();
    initObject();
    mouse.mouseX = QCursor::pos().x();
    mouse.mouseY = QCursor::pos().y();

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::paintGL()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    float near_plane = 0.1f, far_plane = 30.0f;
    QMatrix4x4 lightProjection;
    //lightProjection.ortho(-50.0f, 50.0f, -30.0f, 30.0f, near_plane, far_plane);
    lightProjection.perspective(120,1, near_plane, far_plane);


    QMatrix4x4 lightView;// = camera.getView();
    lightView.lookAt({lights[3].model.tx,lights[3].model.ty-1,lights[3].model.tz},
                     {lights[3].model.tx,lights[3].model.ty-2,lights[3].model.tz},
                     {-1,0,0});

    QMatrix4x4 lightSpaceMatrix = lightProjection * lightView;

    m_programDepth->bind();
    m_programDepth->setUniformValue("lightSpaceMatrix" , lightSpaceMatrix);


    for (auto obj = balls.begin() ; obj!= balls.end(); ++obj)
    {
        obj->render(m_programDepth , this);
    }

    for (auto obj = table.begin() ; obj!= table.end(); ++obj)
    {
        obj->render(m_programDepth , this);
    }

    for (auto obj = lights.begin() ; obj!= lights.end(); ++obj)
    {
        obj->render(m_programDepth , this);
    }

    m_programDepth->release();

    //--------------------------------------------------------------------------------

    keyEvent();
    mouseEvent();

    //---------------------------------------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (test)
    {

        glViewport(0, 0, 1920, 1080);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        m_programDebugShadows->bind();

        m_programDebugShadows->setUniformValue("near_plane" , near_plane);
        m_programDebugShadows->setUniformValue("far_plane" , far_plane);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D , depthMap);

        quad.render2(m_programDebugShadows,this);

        m_programDebugShadows->release();
    }

    //---------------------------------------------------------------------------------

    else
    {
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(10);
        const qreal retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        m_program->bind();
        lightUniforms();
        m_program->setUniformValue("viewmatrix" , camera.getPerspective()*camera.getView());
        m_program->setUniformValue("camPos" , QVector4D(camera.getPos(),1));
        m_program->setUniformValue("lightSpaceMatrix" , lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D , depthMap);

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
        m_program->release();

    }
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
    if (keys.keys[Qt::Key_T])
    {
        test = true;
    }
    if (keys.keys[Qt::Key_R])
    {
        test = false;
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

    m_programDepth = new QOpenGLShaderProgram(this);
    m_programDepth->addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\source\\repos\\Qt\\Billiards\\Billiards\\vertexDepth.glsl");
    m_programDepth->addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\source\\repos\\Qt\\Billiards\\Billiards\\fragmentDepth.glsl");
    m_programDepth->link();

    m_programDebugShadows = new QOpenGLShaderProgram(this);
    m_programDebugShadows->addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\source\\repos\\Qt\\Billiards\\Billiards\\vertexDebugShadows.glsl");
    m_programDebugShadows->addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\source\\repos\\Qt\\Billiards\\Billiards\\fragmentDebugShadows.glsl");
    m_programDebugShadows->link();

    m_program->bind();

    glEnable(GL_DEPTH_TEST);
}

void Scene::initObject()
{
    std::vector<QOpenGLShaderProgram*> shaiders;
    shaiders.push_back(m_program);
    shaiders.push_back(m_programDepth);

    Sphere s(0.75,50,50);
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
                   shaiders);
            o.model.setTranslate(-4+2*k , 6.5f , 2*i-24);
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
               shaiders);
        o.model.setTranslate(0 , 5.75f , 0);
        table.push_back(o);
    }
    {
        Object o;
        o.Init(MeshLoader::loadMesh("D:\\source\\repos\\Qt\\Billiards\\Billiards\\meshes\\MeshTableOnly2.obj"),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\Wood.png",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
               shaiders);
        o.model.setTranslate(0 , -6 , 0);
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
                   shaiders);
            o.model.setTranslate(0,35 , -19 + i*12.5f);
            lights.push_back(o);
        }

    }

    {
        quad.Init(QuadMesh::getVertexs(),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\f.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
               {m_programDebugShadows});


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

