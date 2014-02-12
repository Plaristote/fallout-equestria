#-------------------------------------------------
#
# Project created by QtCreator 2012-05-23T11:36:14
#
#-------------------------------------------------

win32:RC_FILE=ressource.rc

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = FoE-Editor
TEMPLATE = app

unix:QMAKE_CXXFLAGS += -std=c++0x

win32:QMAKE_CFLAGS_RELEASE  = -O2 -MD -zi
win32:QMAKE_LFLAGS_RELEASE  = /INCREMENTAL:NO /DEBUG

win32:LIBS +=  ../3rd_parties/lib/libp3framework.lib \
                ../3rd_parties/lib/libpanda.lib \
                ../3rd_parties/lib/libpandafx.lib \
                ../3rd_parties/lib/libpandaexpress.lib \
                ../3rd_parties/lib/libp3dtoolconfig.lib \
                ../3rd_parties/lib/libp3dtool.lib \
                ../3rd_parties/lib/libp3direct.lib \
                ../3rd_parties/lib/python27.lib
unix:LIBS   += -L/usr/lib/panda3d -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3direct -lX11

INCLUDEPATH += ../3rd_parties/include ../code/editor/include

VPATH += ../code/editor/src ../code/editor/include ../code/editor/ui
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
           serializer.cpp \
           itemeditor.cpp \
           dialoginventoryitem.cpp \
           worldmapeditor.cpp \
           functorthread.cpp \
           dialogsavemap.cpp \
           selectableresource.cpp \
           charsheeteditor.cpp \
           formkeyvalue.cpp \
           maptreewidget.cpp \
           citysplashdialog.cpp

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
            serializer.hpp \
            itemeditor.h \
            dialoginventoryitem.h \
            worldmapeditor.h \
            functorthread.h \
            dialogsavemap.h \
            selectableresource.h \
            charsheeteditor.h \
            formkeyvalue.h \
            maptreewidget.h \
            citysplashdialog.h

FORMS    += mainwindow.ui \
            dialognewscript.ui \
            dialogeditor.ui \
            dialogsplashscreen.ui \
            dialognewmap.ui \
            forminternationalization.ui \
            wizardobject.ui \
            dialogobject.ui \
            dialogwaypointgenerate.ui \
            itemeditor.ui \
            dialoginventoryitem.ui \
            worldmapeditor.ui \
            dialogsavemap.ui \
            selectableresource.ui \
            charsheeteditor.ui \
            formkeyvalue.ui \
            citysplashdialog.ui
