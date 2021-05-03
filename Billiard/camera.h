#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>
#include <QQuaternion>

class Camera
{
public:
    Camera();
    ~Camera() = default;

    QMatrix4x4 getPerspective();
    QMatrix4x4 getView();
    QVector3D getPos();
    void rotate(float upDown, float leftRight);
    void step(float forwBack, float leftRight);
    void fly(float upDown);


    QVector3D center;
    QVector3D eyeLaser;
    QVector3D headDir;
    QVector3D rightDir;

};

inline Camera::Camera()
{
    center = QVector3D(0,12,0);
    eyeLaser = QVector3D(0,0,-1);
    headDir = QVector3D(0,1,0);
    rightDir = QVector3D(1,0,0);
}

inline QMatrix4x4 Camera::getPerspective()
{
    QMatrix4x4 perspective;
    perspective.perspective(60.0f, 16.0f/9.0f, 0.1f, 130.0f);
    return perspective;
}

inline QMatrix4x4 Camera::getView()
{
    QMatrix4x4 view;
    view.lookAt(center, center+eyeLaser, headDir);
    return view;
}

inline QVector3D Camera::getPos()
{
    return center;
}

inline void Camera::rotate(float upDown, float leftRight)
{
    QQuaternion rotateLeftRight = QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), leftRight);
    QQuaternion rotateUpDown = QQuaternion::fromAxisAndAngle(rightDir, upDown);
    QVector3D tmp = rotateLeftRight * rotateUpDown * headDir;
    if (tmp.y() > 0)
    {
        eyeLaser = rotateLeftRight * rotateUpDown * eyeLaser;
        headDir = rotateLeftRight * rotateUpDown * headDir;
        rightDir = rotateLeftRight * rotateUpDown * rightDir;
    }
    else
    {
        eyeLaser = rotateLeftRight  * eyeLaser;
        headDir = rotateLeftRight * headDir;
        rightDir = rotateLeftRight * rightDir;
    }

}

static void boxCollide(QVector3D camera , QVector3D& nadd)
{
    if (abs((camera + nadd).x()) >= 27.0f)
    {
        nadd = QVector3D(0, nadd.y() , nadd.z());
    }

    if (abs((camera + nadd).z()) >= 38.0f)
    {
        nadd = QVector3D(nadd.x(), nadd.y() , 0);
    }

    if ((camera + nadd).y() >= 30.0f || (camera + nadd).y() <= 0.0f)
    {
        nadd = QVector3D(nadd.x(), 0 ,nadd.z());
    }
}

static void tableCollide(QVector3D camera , QVector3D& nadd)
{
    if ((abs((camera + nadd).x()) <= 19.0f)&&
        (abs((camera + nadd).z()) <= 32.0f)&&
        ((camera + nadd).y() <= 12.0f))
    {
        if (abs(camera.x()) > 19.0f )
            nadd = QVector3D(0, nadd.y() , nadd.z());
        if (abs(camera.z()) > 32.0f )
            nadd = QVector3D(nadd.x(), nadd.y() , 0);
        if (abs(camera.y()) > 12.0f )
            nadd = QVector3D(nadd.x(), 0, nadd.z());
    }

}


inline void Camera::step(float forwBack, float leftRight)
{
    auto nadd = eyeLaser*forwBack+ rightDir*leftRight;

    boxCollide(center,nadd);
    tableCollide(center,nadd);


    center = center + nadd;

}

inline void Camera::fly(float upDown)
{
    auto nadd = QVector3D(0,upDown ,0);
    boxCollide(center,nadd);
    tableCollide(center,nadd);
    center = center + nadd;
}

#endif // CAMERA_H
