#include <QApplication>
#include "mainwindow.h"
#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>
#include "qpandawidget.h"
#include <iostream>
#include <QSettings>

#include "qpandaapplication.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Shinygami");
    QCoreApplication::setOrganizationDomain("plaristote.franceserv.fr");
    QCoreApplication::setApplicationName("Fallout Equestria Editor");

    QPandaApplication app(argc, argv);
    QSettings         settings;
    MainWindow        w(&app);
    int               ret;

    // Loading window parameters
    settings.beginGroup("MainWindow");
    w.resize(settings.value("size", QSize(400, 400)).toSize());
    w.move(settings.value("pos", QPoint(50, 50)).toPoint());
    settings.endGroup();
    // Executing app
    w.show();
    ret = app.exec();
    // Savinf window parameters
    settings.beginGroup("MainWindow");
    settings.setValue("size", w.size());
    settings.setValue("pos", w.pos());
    settings.endGroup();
    return (ret);
}
