#-------------------------------------------------
#
# Project created by QtCreator 2012-05-23T11:36:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FoE-Editor
TEMPLATE = app

LIBS      += -L/usr/lib/panda3d -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct
unix:LIBS += -lX11

SOURCES += main.cpp\
        mainwindow.cpp \
    qpandawidget.cpp \
    qpandaapplication.cpp \
    qangelscriptedit.cpp \
    dialognewscript.cpp \
    dialogeditor.cpp \
    data.cpp \
    json.cpp \
    scenecamera.cpp \
    mouse.cpp \
    dialogsplashscreen.cpp \
    dialognewmap.cpp \
    forminternationalization.cpp \
    wizardobject.cpp \
    dialogobject.cpp \
    world.cpp \
    tabscript.cpp \
    tabdialog.cpp \
    tabl18n.cpp \
    dialogwaypointgenerate.cpp \
    serializer.cpp

HEADERS  += mainwindow.h \
    qpandawidget.h \
    qpandaapplication.h \
    qangelscriptedit.h \
    dialognewscript.h \
    dialogeditor.h \
    json.hpp \
    datatree.hpp \
    scenecamera.h \
    mouse.h \
    dialogsplashscreen.h \
    dialognewmap.h \
    forminternationalization.h \
    wizardobject.h \
    dialogobject.h \
    world.h \
    tabscript.h \
    tabdialog.h \
    tabl18n.h \
    dialogwaypointgenerate.h \
    serializer.hpp

FORMS    += mainwindow.ui \
    dialognewscript.ui \
    dialogeditor.ui \
    dialogsplashscreen.ui \
    dialognewmap.ui \
    forminternationalization.ui \
    wizardobject.ui \
    dialogobject.ui \
    dialogwaypointgenerate.ui
