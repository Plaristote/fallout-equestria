#include <panda3d/cmath.h>
#include <QApplication>
#include "mainwindow.h"
#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>
#include "qpandawidget.h"
#include <iostream>
#include <QSettings>

#include "qpandaapplication.h"
#include "selectableresource.h"

int main(int argc, char *argv[])
{
    qDebug("Fallout Equestria Game Editor");
    QPandaApplication panda(argc, argv);
    QCoreApplication::setOrganizationName("Shinygami");
    QCoreApplication::setOrganizationDomain("plaristote.franceserv.fr");
    QCoreApplication::setApplicationName("Fallout Equestria Editor");
    QApplication      app(argc, argv);
    int               ret;

    {
      QSettings       settings;
      MainWindow      window;

      // Loading window parameters
      settings.beginGroup("MainWindow");
      window.resize(settings.value("size", QSize(400, 400)).toSize());
      window.move(settings.value("pos", QPoint(50, 50)).toPoint());
      settings.endGroup();
      // Executing app
      window.show();
      window.splashScreen.open();
      ret = app.exec();
      // Savinf window parameters
      settings.beginGroup("MainWindow");
      settings.setValue("size", window.size());
      settings.setValue("pos",  window.pos());
      settings.endGroup();
    }
    return (ret);
}
