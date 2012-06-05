#ifndef DIALOGNEWDYNOBJECT_H
#define DIALOGNEWDYNOBJECT_H

#include <QDialog>

namespace Ui {
class DialogNewDynObject;
}

class DialogNewDynObject : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogNewDynObject(QWidget *parent = 0);
    ~DialogNewDynObject();
    
private:
    Ui::DialogNewDynObject *ui;
};

#endif // DIALOGNEWDYNOBJECT_H
