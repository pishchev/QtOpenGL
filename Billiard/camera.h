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
    center = QVector3D(0,9,0);
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

inline void Camera::step(float forwBack, float leftRight)
{
    center = center + eyeLaser*forwBack;
    center = center + rightDir*leftRight;
}

inline void Camera::fly(float upDown)
{
    center = center + QVector3D(0,upDown ,0);
}

#endif // CAMERA_H
