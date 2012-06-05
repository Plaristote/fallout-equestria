#ifndef DIALOGNEWMAP_H
#define DIALOGNEWMAP_H

#include <QDialog>

namespace Ui {
class DialogNewMap;
}

class DialogNewMap : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogNewMap(QWidget *parent = 0);
    ~DialogNewMap();
    
private:
    Ui::DialogNewMap *ui;
};

#endif // DIALOGNEWMAP_H
