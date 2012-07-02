#ifndef DIALOGOBJECT_H
#define DIALOGOBJECT_H

# include <panda3d/cmath.h>
#include <QDialog>
#include <QVector>
#include "world.h"
#include "dialoginventoryitem.h"
#include "datatree.hpp"

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

public slots:
    void SetObjectList(QStringList);

private slots:
    void UpdateType(QString);
    void Reset(void);
    void Apply(void);

    void OpenItemDialog(void);
    void ItemAdd(void);
    void ItemSave(void);

private:
    Ui::DialogObject*   ui;
    DynamicObject*      object;
    DialogInventoryItem dialogItem;

    QVector<DataTree*>  itemsData;
};

#endif // DIALOGOBJECT_H
