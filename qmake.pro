QT += core gui opengl widgets

# For GLU library. On Windows, this might be linked automatically with opengl32.
# On Linux, you might need to add: LIBS += -lGLU
# On macOS, it's usually part of the OpenGL framework.
unix:!macx: LIBS += -lGLU

TARGET = SphereViewer
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    openglwidget.cpp

HEADERS += \
    mainwindow.h \
    openglwidget.h

FORMS += \
    mainwindow.ui # Optional: if you want to design UI with Qt Designer