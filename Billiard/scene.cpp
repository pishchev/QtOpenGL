#include "scene.h"
#include <QScreen>

void Scene::initializeGL()
{
    initProgram();
    initObject();
    initShadowBuffers();
    mouse.mouseX = QCursor::pos().x();
    mouse.mouseY = QCursor::pos().y();

}

void Scene::paintGL()
{
    ballsPool.move();

    createShadowMap();

    keyEvent();
    mouseEvent();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    m_program->setUniformValue("viewmatrix" , camera.getPerspective()*camera.getView());
    m_program->setUniformValue("camPos" , QVector4D(camera.getPos(),1));
    m_program->setUniformValueArray("lightSpaceMatrix" , lightSpaceMatrix,4);

    lightUniforms();

    for (int i = 0 ; i < 4; i ++)
    {
        glActiveTexture(GL_TEXTURE2+i);
        glBindTexture(GL_TEXTURE_2D, depthMap[i]);
    }

    arrow.follow();

    Object::render(ballsPool.balls,m_program , this);

    Object::render(table,m_program , this);
    Object::render(lamps,m_program , this);
    Object::render(walls,m_program , this);
    Object::render(lights,m_program , this);

    if(arrow.preparing)
    {
        arrow.prepare();
    }
    if(arrow.shot)
    {
        arrow.shot = false;

        QVector2D hit = arrow.hit();
        ballsPool.balls[8].hit(hit.x(),hit.y());

        arrow.drop();
    }

    m_program->setUniformValue("ambientColor", QColor(255 * arrow.arrowColorFactor() , (1- arrow.arrowColorFactor())*255 , 0 ));
    Object::render(arrow.arrow,m_program , this);

    m_program->release();

    ++m_frame;

}

void Scene::initShadowBuffers()
{
    for (int i = 0 ; i < 4 ; i ++)
    {
        glGenFramebuffers(1, &depthMapFBO[i]);
        glGenTextures(1, &depthMap[i]);
        glBindTexture(GL_TEXTURE_2D, depthMap[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[i], 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    float near_plane = 0.1f, far_plane = 60.0f;
    for (int i = 0 ; i < 4 ; i ++)
    {
        QMatrix4x4 lightProjection;
        lightProjection.perspective(120,1, near_plane, far_plane);


        QMatrix4x4 lightView ;
        lightView.lookAt({lights[i].model.tx,lights[i].model.ty,lights[i].model.tz},
                         {lights[i].model.tx,lights[i].model.ty-1,lights[i].model.tz},
                         {-1,0,0});

        lightSpaceMatrix[i] = lightProjection * lightView;
    }
}

void Scene::createShadowMap()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    for (int i = 0 ; i < 4 ; i ++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        m_programDepth->bind();
        m_programDepth->setUniformValue("lightSpaceMatrix" , lightSpaceMatrix[i]);

        Object::render(ballsPool.balls,m_programDepth, this);
        Object::render(table,m_programDepth , this);

        m_programDepth->release();
    }

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
    if (event->buttons() == Qt::RightButton)
    {
        int dx = QCursor::pos().x() - mouse.mouseX;
        int dy = QCursor::pos().y() - mouse.mouseY;

        camera.rotate(-1*(float)dy/5,-1*(float)dx/5);
    }

    if (event->buttons() == Qt::LeftButton)
    {
        int dx = QCursor::pos().x() - mouse.mouseX;

        //camera.rotate(-1*(float)dy/5,-1*(float)dx/5);
        arrow.addRotate(1*(float)dx/5);
    }
}

void Scene::mouseEvent()
{
    mouse.mouseX = QCursor::pos().x();
    mouse.mouseY = QCursor::pos().y();    
}

void Scene::keyEvent()
{
    GLfloat camera_speed = 0.5;
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
    if (keys.keys[Qt::Key_Left])
    {
        arrow.addRotate(-2.3f);
    }
    if (keys.keys[Qt::Key_Right])
    {
        arrow.addRotate(2.3f);
    }

    if (keys.keys[Qt::Key_E])
    {
        arrow.preparing = true;
        arrow.shot = false;
    }
    else
    {
        if (arrow.preparing)
        {
            arrow.shot = true;
        }
        arrow.preparing = false;
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

    //BALLS
    {
        Sphere s(0.8,50,50);
        if (true)
        {

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
                           "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_metal.jpg",
                           shaiders);
                    o.model.setTranslate(-4+2*k , 6.6f , 8.0f*i-26);
                    o.model.rotate(true);
                    //balls.push_back(Ball(o,num));
                    ballsPool.addBall(num , Ball(o,num, 0.8f));

                }
            }
        }
        else
        {
            int num = 0;
            Object o;
            Ball b = Ball(o,num, 0.75f);

            o.Init(s.getVertexs(),
                   screen()->refreshRate()/100,
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_metal.jpg",
                   shaiders);
            o.model.setTranslate(2*2 , 6.5f , 8.0f*2-27);
            num++;
            b = Ball(o,num, 0.75f);
            b.dx = 0;
            b.dz = 0;
            ballsPool.addBall(num , b);

            o.Init(s.getVertexs(),
                   screen()->refreshRate()/100,
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\1.jpg",
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_metal.jpg",
                   shaiders);
            o.model.setTranslate(2*2 , 6.5f , 13.0f*2-27);
            num++;
            b = Ball(o,num, 0.75f);
            b.dx = 0;
            b.dz = 0;
            ballsPool.addBall(num , b);

            o.Init(s.getVertexs(),
                   screen()->refreshRate()/100,
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\2.jpg",
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_metal.jpg",
                   shaiders);
            o.model.setTranslate(2*2 , 6.5f , 12.0f*2-27);
            num++;
            b = Ball(o,num, 0.75f);
            b.dx = 0;
            b.dz = -0.3f;
            ballsPool.addBall(num , b);


        }

    }
    //TABLE
    {
        Object o;
        o.Init(Surface::surface(14.2f,27.6f,15,15),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\f.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_text.png",
               shaiders);
        o.model.setTranslate(0 , 5.75f , 0);
        o.model.setRotate(0,0,0,1.0f);
        Filtering(o,2);

        table.push_back(o);

        o.Init(MeshLoader::loadMesh("D:\\source\\repos\\Qt\\Billiards\\Billiards\\meshes\\MeshTableOnly2.obj"),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\w.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_wood2.jpg",
               shaiders);
        o.model.setTranslate(0 , -6 , 0);
        o.model.setRotate(0,0,0,1.0f);
        table.push_back(o);

    }
    //LAMPS
    {
        Object o;
        for (int i = -1 ; i <= 1; i += 2)
        for (int k = -1 ; k <= 1; k+=2)
        {
            o.Init(MeshLoader::loadMesh("D:\\source\\repos\\Qt\\Billiards\\Billiards\\meshes\\lamp.obj"),
                   screen()->refreshRate()/100,
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\b_w.jpg",
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_metal.jpg",
                   shaiders);
            o.model.setTranslate(7*i , 36  , 13*k);
            lamps.push_back(o);
        }

    }
    //LIGHTS
    {
        for (int i = -1 ; i <= 1; i += 2)
        for (int k = -1 ; k <= 1; k+=2)
        {
            LightObject o;
            Sphere s2(0.75,50,50);
            o.Init(s2.getVertexs(),
                   screen()->refreshRate()/100,
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\white.jpg",
                   "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_metal.jpg",
                   shaiders);
            o.model.setTranslate(7*i ,35 , k*13);
            o.model.setRotate(0,0,0,1.0f);
            lights.push_back(o);
        }

    }
    //WALLS
    {
        //ПОЛ
        Object o;
        o.Init(Surface::surface(35.0f,50.0f,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate(0 , -6.1f , 0);
        o.model.setRotate(0,0,0,1.0f);
        walls.push_back(o);



        //ПОТОЛОК
        o.Init(Surface::surface(35.0f,50.0f,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate(0 , 41.0f , 0);
        o.model.setRotate(90.0f*2,0,0,1.0f);
        walls.push_back(o);


        //БОКОВЫЕ НИЖНИЕ
        o.Init(Surface::surface((41.0f+6.1f)/2,50.0f,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate(35.0f , (41.0f+6.1f)/2-6.1f-(41.0f+6.1f) , 0);
        o.model.setRotate(90.0f,0,0,1.0f);


        walls.push_back(o);

        o.Init(Surface::surface((41.0f+6.1f)/2,50.0f,1,2),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\maectro.png",
               shaiders);
        o.model.setTranslate(-35.0f , (41.0f+6.1f)/2-6.1f-(41.0f+6.1f) , 0);
        o.model.setRotate(90*3,0,0,1.0f);


        walls.push_back(o);

        //БОКОВЫЕ ВЕРХНИЕ
        o.Init(Surface::surface((41.0f+6.1f)/2,50.0f,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate(35.0f , (41.0f+6.1f)/2-6.1f+(41.0f+6.1f) , 0);
        o.model.setRotate(90.0f,0,0,1.0f);


        walls.push_back(o);

        o.Init(Surface::surface((41.0f+6.1f)/2,50.0f,1,2),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\maectro.png",
               shaiders);
        o.model.setTranslate(-35.0f , (41.0f+6.1f)/2-6.1f+(41.0f+6.1f) , 0);
        o.model.setRotate(90*3,0,0,1.0f);


        walls.push_back(o);


        //ДАЛЬНИЕ НИЖНИЕ
        o.Init(Surface::surface(35.0f,(41.0f+6.1f)/2,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate( 0, (41.0f+6.1f)/2-6.1f-(41.0f+6.1f) , -50);
        o.model.setRotate(90,1.0f,0,0);


        walls.push_back(o);

        o.Init(Surface::surface(35.0f,(41.0f+6.1f)/2,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate( 0, (41.0f+6.1f)/2-6.1f-(41.0f+6.1f) , 50);
        o.model.setRotate(90*3,1.0f,0,0);


        walls.push_back(o);

        //ДАЛЬНИЕ ВЕРХНИЕ
        o.Init(Surface::surface(35.0f,(41.0f+6.1f)/2,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate( 0, (41.0f+6.1f)/2-6.1f+(41.0f+6.1f) , -50);
        o.model.setRotate(90,1.0f,0,0);


        walls.push_back(o);

        o.Init(Surface::surface(35.0f,(41.0f+6.1f)/2,5,5),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);
        o.model.setTranslate( 0, (41.0f+6.1f)/2-6.1f+(41.0f+6.1f) , 50);
        o.model.setRotate(90*3,1.0f,0,0);


        walls.push_back(o);
    }
    //ARROW
    {
        Object o;
        o.Init(Triangle::triangle(),
               screen()->refreshRate()/100,
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\title.jpg",
               "D:\\source\\repos\\Qt\\Billiards\\Billiards\\maps\\norm_title.jpg",
               shaiders);

        o.model.setRotate(0,0,0,1.0f);
        arrow.arrow.push_back(o);
        arrow.followedBy = &(ballsPool.balls[8].obj);

    }
}

void Scene::Filtering(const Object& o, int i)
{
    if (i == 0)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        o.texture->setMaximumAnisotropy(0);
        o.normalMap->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
        o.normalMap->setMaximumAnisotropy(0);
    }
    else if(i == 1)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapNearest);
        o.texture->setMaximumAnisotropy(0);
        o.normalMap->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapNearest);
        o.normalMap->setMaximumAnisotropy(0);
    }
    else if(i == 2)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        o.texture->setMaximumAnisotropy(0);
        o.normalMap->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapNearest);
        o.normalMap->setMaximumAnisotropy(0);
    }
    else if(i == 3)
    {
        o.texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        o.texture->setMaximumAnisotropy(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);
        o.normalMap->setMinMagFilters(QOpenGLTexture::LinearMipMapNearest, QOpenGLTexture::LinearMipMapNearest);
        o.normalMap->setMaximumAnisotropy(0);
    }
}

void Scene::setLightCoefs(std::vector<int> vec)
{
    coefs.ka = static_cast<float>(vec[0])/100;
    coefs.kd = vec[1];
    coefs.k = vec[2];
    coefs.p = static_cast<float>(vec[3])/10;
    coefs.n = static_cast<float>(vec[4])/10;
    coefs.ks = vec[5];
}

