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
    QStringList GetItemList(void) const;

signals:
    void ItemListChanged(QStringList);

private slots:
    void SaveData(void);
    void UpdateData(void);
    void SelectItem(QString);
    void SelectAction(QString);

    void ItemAdd(void);
    void ItemDelete(void);
    void ActionAdd(void);
    void ActionDelete(void);

    void SelectScript(void);
    void SelectModel(void);
    void SelectTexture(void);

private:
    Ui::ItemEditor *ui;

    DataTree*      dataTree;
    bool           loading;
};

#endif // ITEMEDITOR_H
