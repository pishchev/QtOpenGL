include(openglwindow.pri)

SOURCES += \
    main.cpp \
    squarewindow.cpp

QT += widgets
LIBS += -lopengl32

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target

HEADERS += \
    shaders.hpp \
    square.hpp \
    squarewindow.hpp
