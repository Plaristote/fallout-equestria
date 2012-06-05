#ifndef DIALOGWAYPOINTGENERATE_H
#define DIALOGWAYPOINTGENERATE_H

#include <QDialog>
#include "world.h"

namespace Ui {
class DialogWaypointGenerate;
}

class DialogWaypointGenerate : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWaypointGenerate(QWidget *parent = 0);
    ~DialogWaypointGenerate();

    void SetWorld(World* world);

private slots:
    void Generate(void);

private:
    Ui::DialogWaypointGenerate* ui;

    World*                      world;
};

#endif // DIALOGWAYPOINTGENERATE_H
