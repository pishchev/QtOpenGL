QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
LIBS += -lopengl32
LIBS += -lglu32


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    meshloader.cpp \
    object.cpp \
    objectmodel.cpp \
    scene.cpp \
    sourcelight.cpp \
    sphere.cpp \
    surface.cpp

HEADERS += \
    camera.h \
    keyboard.h \
    mainwindow.h \
    meshloader.h \
    mouse.h \
    object.h \
    objectmodel.h \
    scene.h \
    sourcelight.h \
    sphere.h \
    surface.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fragmentShaider.glsl \
    vertexShaider.glsl
