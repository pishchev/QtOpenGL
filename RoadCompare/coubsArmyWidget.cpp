#include "coubsArmyWidget.h"
#include <QScreen>


void CoubsArmy::initializeGL()
{
    initProgram();
    initShadersParam();

    initColors();
    initObject();
}
void CoubsArmy::paintGL()
{   
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    QMatrix4x4 projection;
    projection.perspective(60.0f, 4.0f / 3.0f, 0.1f, 150.0f);
    glEnable(GL_DEPTH_TEST);
    colorOwner.checkColor();
    lightUniforms();

    for (auto obj = objects.begin() ; obj!= objects.end(); ++obj)
    {
        auto mat = obj->model.getMatrix();
        m_program->setUniformValue(m_trMatrix , mat);
        m_program->setUniformValue(obj->textName.data(),0);
        m_program->setUniformValue(obj->normName.data(),1);
        m_program->setUniformValue(m_matrixUniform, projection*mat);
        obj->m_vao->bind();
        glActiveTexture(GL_TEXTURE0);
        obj->texture->bind();
        glActiveTexture(GL_TEXTURE1);
        obj->normalMap->bind();
        glPolygonMode(GL_FRONT, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, obj->vertexs.size());
    }

    m_program->release();
    ++m_frame;

}
void CoubsArmy::lightUniforms()
{
    m_program->setUniformValue(coefs.m_ka , coefs.ka);
    m_program->setUniformValue(coefs.m_kd , coefs.kd);
    m_program->setUniformValue(coefs.m_k , coefs.k);
    m_program->setUniformValue(coefs.m_p , coefs.p);

    m_program->setUniformValue(pointLight.getFlagUniform() , pointLight.state());
    m_program->setUniformValue(pointLight.getPosUniform() , pointLight.getPosition());
    m_program->setUniformValue(pointLight.getColorUniform(), pointLight.getColor());

    m_program->setUniformValue(directionLight.getFlagUniform() , directionLight.state());
    m_program->setUniformValue(directionLight.getPosUniform() , directionLight.getPosition());
    m_program->setUniformValue(directionLight.getColorUniform(), directionLight.getColor());

    m_program->setUniformValue(ambientLight.getColorUniform() , ambientLight.getColor());
    m_program->setUniformValue(ambientLight.getFlagUniform() , ambientLight.state());

}

void CoubsArmy::initProgram()
{
    initializeOpenGLFunctions();
    f = QOpenGLContext::currentContext()->functions();
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\source\\repos\\Qt\\RoadCompare\\vertexShaider.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\source\\repos\\Qt\\RoadCompare\\fragmentShaider.glsl");
    m_program->link();
    m_program->bind();
}
void CoubsArmy::initShadersParam()
{
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);  
    m_norAttr = m_program->attributeLocation("norAttr");
    Q_ASSERT(m_matrixUniform != -1);

    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    m_trMatrix = m_program->uniformLocation("trmatrix");
    Q_ASSERT(m_trMatrix  != -1);

    coefs.m_ka = m_program->uniformLocation("ka");
    Q_ASSERT(coefs.m_ka != -1);
    coefs.m_kd = m_program->uniformLocation("kd");
    Q_ASSERT(coefs.m_kd != -1);
    coefs.m_k = m_program->uniformLocation("k");
    Q_ASSERT(coefs.m_k != -1);
    coefs.m_p = m_program->uniformLocation("p");
    Q_ASSERT(coefs.m_p != -1);

    pointLight.setPosUniform( m_program->uniformLocation("pointPos"));
    Q_ASSERT(pointLight.getPosUniform()  != -1);
    pointLight.setColorUniform( m_program->uniformLocation("pointColor"));
    Q_ASSERT(pointLight.getColorUniform() != -1);
    pointLight.setFlagUniform( m_program->uniformLocation("pointFlag"));
    Q_ASSERT(pointLight.getFlagUniform()  != -1);

    directionLight.setPosUniform( m_program->uniformLocation("directionPos"));
    Q_ASSERT(directionLight.getPosUniform()  != -1);
    directionLight.setColorUniform( m_program->uniformLocation("directionColor"));
    Q_ASSERT(directionLight.getColorUniform() != -1);
    directionLight.setFlagUniform( m_program->uniformLocation("directionFlag"));
    Q_ASSERT(directionLight.getFlagUniform()  != -1);

    ambientLight.setColorUniform( m_program->uniformLocation("ambientColor"));
    Q_ASSERT(ambientLight.getColorUniform() != -1);
    ambientLight.setFlagUniform( m_program->uniformLocation("ambientFlag"));
    Q_ASSERT(ambientLight.getFlagUniform()  != -1);


}
void CoubsArmy::initObject()
{
    Object o;

    int count = 4;
    GLfloat mult = 1.01f;
    GLfloat strt = -1;
    for (int i  = 0; i < count ; i++)
    {
        o.Init(Surface::surface(0.5f,20 , 1,20),
                    screen()->refreshRate()/100,
                    "D:\\source\\repos\\Qt\\RoadCompare\\road_0.jpg",
                    "D:\\source\\repos\\Qt\\RoadCompare\\textileTextureNormal.jpg",
                    m_program);
        o.model.setStartPosition(strt+mult*i,-1,0);

        Filtering(o, i);

        objects.push_back(o);
    }

    for (int i  = 0; i < count ; i++)
    {
        o.Init(Surface::surface(0.5f,20 , 1,20),
                    screen()->refreshRate()/100,
                    "D:\\source\\repos\\Qt\\RoadCompare\\road_2.jpg",
                    "D:\\source\\repos\\Qt\\RoadCompare\\textileTextureNormal.jpg",
                    m_program);
        o.model.setStartPosition(strt+mult*i,1,0);

        Filtering(o, i);

        objects.push_back(o);
    }

    for (int i  = 0; i < count ; i++)
    {
        o.Init(Surface::surface(0.5f,20 , 1,20),
                    screen()->refreshRate()/100,
                    "D:\\source\\repos\\Qt\\RoadCompare\\road_3.jpg",
                    "D:\\source\\repos\\Qt\\RoadCompare\\textileTextureNormal.jpg",
                    m_program);
        o.model.setStartPosition(strt+mult*i,3,0);

        Filtering(o, i);

        objects.push_back(o);
    }

    for (int i  = 0; i < count ; i++)
    {
        o.Init(Surface::surface(0.5f,20 , 1,20),
                    screen()->refreshRate()/100,
                    "D:\\source\\repos\\Qt\\RoadCompare\\sand.jpg",
                    "D:\\source\\repos\\Qt\\RoadCompare\\textileTextureNormal.jpg",
                    m_program);
        o.model.setStartPosition(strt+mult*i,-3,0);

        Filtering(o, i);

        objects.push_back(o);
    }

}

void CoubsArmy::Filtering(const Object& o, int i)
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

void CoubsArmy::initColors()
{
    colorOwner.setSources({&ambientLight , &directionLight , &pointLight});

    ambientLight.setColor(QColor(255,255,255));

    directionLight.setPosition(QVector4D( QVector3D(1,0,0), 1 ));
    directionLight.setColor(QColor(255,255,255));
    directionLight.off();

    pointLight.setPosition(QVector4D( QVector3D(4,5,-4), 1 ));
    pointLight.setColor(QColor(255,255,255));

}



void CoubsArmy::setPosition(float x,float y,float z)
{
    for (auto obj = objects.begin() ; obj!= objects.end(); ++obj)
    {
        obj->model.setTranslate( x,y, z);
    }
}
void CoubsArmy::setColor(QColorDialog* cd , int active)
{
    colorOwner.setActive(active);
    colorOwner.setDialog(cd);
}
void CoubsArmy::setRotate(bool fl)
{
    for (auto obj = objects.begin() ; obj!= objects.end(); ++obj)
    {
        obj->model.rotate(fl);
    }
}
void CoubsArmy::setLightCoefs(std::vector<int> vec)
{
    coefs.ka = static_cast<float>(vec[0])/100;
    coefs.kd = static_cast<float>(vec[1])/100;
    coefs.k = static_cast<float>(vec[2])/10;
    coefs.p = vec[3];
}
void CoubsArmy::setActivateLight(std::vector<bool> vec)
{
    directionLight.setState(vec[0]);
    pointLight.setState(vec[1]);
}
void CoubsArmy::setDirectionLightPosition(QVector4D pos)
{
    directionLight.setPosition(pos);
}
void CoubsArmy::setPointLightPosition(QVector4D pos)
{
    pointLight.setPosition(pos);
}
