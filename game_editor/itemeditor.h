#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include <QWidget>
#include "datatree.hpp"

namespace Ui {
class ItemEditor;
}

class ItemEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ItemEditor(QWidget *parent = 0);
    ~ItemEditor();

    void LoadAllItems(void);

private slots:
    void SelectItem(QString);
    void SelectAction(QString);

private:
    Ui::ItemEditor *ui;

    DataTree*      dataTree;
};

#endif // ITEMEDITOR_H
