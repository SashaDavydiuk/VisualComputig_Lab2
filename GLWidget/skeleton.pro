QT += core gui widgets opengl openglwidgets
DEFINES += QT_WIDGETS_LIB
INCLUDEPATH += .
#LIBS += -lopengl32 -lglu32
#FORMS += renderview.ui
HEADERS += GLWidget.h \
           Window.h
SOURCES += GLWidget.cpp \
           Window.cpp \
           main.cpp
RESOURCES += shaders.qrc
