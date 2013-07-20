#ifndef DIALOGWAYPOINTGENERATE_H
#define DIALOGWAYPOINTGENERATE_H

# include <panda3d/cmath.h>
#include <QDialog>
#include "world.h"

namespace Ui {
class DialogWaypointGenerate;
}

typedef QList<Waypoint*> WaypointList;

#include "functorthread.h"

class WaypointGenerator : public QObject
{
  Q_OBJECT

  typedef std::vector<std::vector<Waypoint*> > WaypointGrid;
public:
  WaypointGenerator(World* world, MapObject* object, LPoint4 margin, LPoint2 spacing);

  void Run(void);

private:
  void ClearObject(void);
  void Generating(WaypointGrid&);
  void Linking(WaypointGrid&);
  void Leveling(void);

  bool LevelWaypoint(Waypoint*);

signals:
  void UpdateProgress(QString,float);
  void Started(void);
  void Done(void);

private:
  World*         world;
  MapObject*     object;
  unsigned int   sizex, sizey;
  float          initPosX, initPosY, initPosZ;
  float          spacingx, spacingy;
  FunctorThread* thread;
};

class DialogWaypointGenerate : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWaypointGenerate(QWidget *parent = 0);
    ~DialogWaypointGenerate();

    LPoint4 GetMargin(void) const;
    LPoint2 GetSpacing(void) const;

signals:
    void Generate(void);

private:
    Ui::DialogWaypointGenerate* ui;
};

#endif // DIALOGWAYPOINTGENERATE_H
