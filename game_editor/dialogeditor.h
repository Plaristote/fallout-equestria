#ifndef DIALOGEDITOR_H
#define DIALOGEDITOR_H

#include <QWidget>
#include "datatree.hpp"
#include <QTreeWidgetItem>

namespace Ui {
class DialogEditor;
}

class DialogEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit DialogEditor(QWidget *parent = 0);
    ~DialogEditor();

    void LoadDialog(DataTree* tree);

    void AddNode(QString);
    void SaveData(void);

private slots:
    void NewNode(void);
    void DeleteNode(void);
    void SwapNode(QString);
    void NewSuccessor(void);
    void DeleteSuccessor(void);
    void MoveUp(void);
    void MoveDown(void);
    void ChangedCurrentSuccessor(void);
    void NpcLineChanged(void);

    void SuccessorChanged(QTreeWidgetItem*, int);

private:
    Ui::DialogEditor *ui;

    DataTree*         tree;
    QString           currentSuccessorName;
};

#endif // DIALOGEDITOR_H
