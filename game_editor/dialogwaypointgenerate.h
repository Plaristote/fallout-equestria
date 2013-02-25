#ifndef DIALOGWAYPOINTGENERATE_H
#define DIALOGWAYPOINTGENERATE_H

# include <panda3d/cmath.h>
#include <QDialog>
#include "world.h"

namespace Ui {
class DialogWaypointGenerate;
}

typedef QList<Waypoint*> WaypointList;

class DialogWaypointGenerate : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWaypointGenerate(QWidget *parent = 0);
    ~DialogWaypointGenerate();

    void SetWorld(World* world);
    WaypointList GetToSelect(void) const { return (to_select); }

private slots:
    void Generate(void);

signals:
    void SigUpdateProgressbar(QString, float);
    void StartedGeneration(void);
    void EndedGeneration(void);

private:
    Ui::DialogWaypointGenerate* ui;

    World*                      world;
    WaypointList                to_select;
};

#endif // DIALOGWAYPOINTGENERATE_H
