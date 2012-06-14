#ifndef DIALOGOBJECT_H
#define DIALOGOBJECT_H

# include <panda3d/cmath.h>
#include <QDialog>
#include "world.h"

namespace Ui {
class DialogObject;
}

class WindowFramework;

class DialogObject : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogObject(QWidget *parent = 0);
    ~DialogObject();

    void SetCurrentObject(DynamicObject* object);

private slots:
    void UpdateType(QString);
    void Reset(void);
    void Apply(void);

private:
    Ui::DialogObject *ui;
    DynamicObject*    object;
};

#endif // DIALOGOBJECT_H
