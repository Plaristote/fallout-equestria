#ifndef DIALOGINVENTORYITEM_H
#define DIALOGINVENTORYITEM_H

#include <QDialog>
#include "datatree.hpp"

namespace Ui {
class DialogInventoryItem;
}

class DialogInventoryItem : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogInventoryItem(QWidget *parent = 0);
    ~DialogInventoryItem();

    void                    SetDataTree(DataTree* p);
    void                    SetItemType(QString);
    
private slots:
    void                    SaveChanges(void);

private:
    Ui::DialogInventoryItem *ui;
    DataTree*                dataTree;
};

#endif // DIALOGINVENTORYITEM_H
