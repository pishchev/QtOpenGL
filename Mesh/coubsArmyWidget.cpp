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

    auto mat = object.model.getMatrix();

    m_program->setUniformValue(m_trMatrix , mat);
    m_program->setUniformValue("textureMap",0);
    m_program->setUniformValue("normalMap",1);
    m_program->setUniformValue(m_matrixUniform, projection*mat);

    object.m_vao->bind();
    glActiveTexture(GL_TEXTURE0);
    object.texture->bind();
    glActiveTexture(GL_TEXTURE1);
    object.normalMap->bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, object.vertexs.size());


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
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\source\\repos\\Qt\\Mesh\\vertexShaider.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\source\\repos\\Qt\\Mesh\\fragmentShaider.glsl");
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
    object = MeshLoader::loadMesh("D:\\source\\repos\\Qt\\Mesh\\MeshPorshe.obj");
    object.model.setTranslate(0.0f, -0.0f, -2.5f);
    object.model.setRotate(screen()->refreshRate()/100,0.0f,1.0f,0.0f);
    object.model.rotate(false);

    object.texture.reset(new QOpenGLTexture(QImage("D:\\source\\repos\\Qt\\Mesh\\textileTexture.jpg")));
    object.texture->setWrapMode(QOpenGLTexture::Repeat);

    object.normalMap.reset(new QOpenGLTexture(QImage("D:\\source\\repos\\Qt\\Mesh\\textileTextureNormal.jpg")));

    object.m_vao = new QOpenGLVertexArrayObject;
    if (object.m_vao->create())
    object.m_vao->bind();

    object.m_vbo = new QOpenGLBuffer;
    object.m_vbo->create();
    object.m_vbo->bind();

    object.m_vbo->allocate( object.vertexs.data(), object.vertexs.size() * sizeof(Vertex));

    m_program->enableAttributeArray("posAttr");
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, offsetof(Vertex, vertex), 3, sizeof(Vertex));

    m_program->enableAttributeArray("norAttr");
    m_program->setAttributeBuffer("norAttr", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    m_program->enableAttributeArray("texCoord");
    m_program->setAttributeBuffer("texCoord", GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));

    object.m_vbo->release();
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
void CoubsArmy::setPosition(float z,float y)
{
    object.model.setTranslate(0.0f, y, z);
}
void CoubsArmy::setColor(QColorDialog* cd , int active)
{
    colorOwner.setActive(active);
    colorOwner.setDialog(cd);
}
void CoubsArmy::setRotate(bool fl)
{
    object.model.rotate(fl);
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
