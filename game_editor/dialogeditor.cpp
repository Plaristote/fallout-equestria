#include "dialogeditor.h"
#include "ui_dialogeditor.h"
#include <QInputDialog>
#include <QMessageBox>

DialogEditor::DialogEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogEditor)
{
    QIcon iconDelete("icons/delete.png");
    QIcon iconAdd("icons/add.png");
    QIcon iconUp("icons/up.png");
    QIcon iconDown("icons/down.png");

    tree = 0;
    ui->setupUi(this);

    ui->nodeRemove->setIcon(iconDelete);
    ui->nodeNew->setIcon(iconAdd);
    ui->successorAdd->setIcon(iconAdd);
    ui->successorRemove->setIcon(iconDelete);
    ui->successorUp->setIcon(iconUp);
    ui->successorDown->setIcon(iconDown);

    connect(ui->nodeRemove,      SIGNAL(clicked()), this, SLOT(DeleteNode()));
    connect(ui->nodeNew,         SIGNAL(clicked()), this, SLOT(NewNode()));
    connect(ui->successorAdd,    SIGNAL(clicked()), this, SLOT(NewSuccessor()));
    connect(ui->successorRemove, SIGNAL(clicked()), this, SLOT(DeleteSuccessor()));
    connect(ui->successorDown,   SIGNAL(clicked()), this, SLOT(MoveDown()));
    connect(ui->successorUp,     SIGNAL(clicked()), this, SLOT(MoveUp()));
    connect(ui->nodeList,        SIGNAL(currentIndexChanged(QString)), this, SLOT(SwapNode(QString)));
    connect(ui->successorList,   SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(SuccessorChanged(QTreeWidgetItem*,int)));
    connect(ui->successorList,   SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(ChangedCurrentSuccessor()));
    connect(ui->npcLine,         SIGNAL(textEdited(QString)), this, SLOT(NpcLineChanged()));
}

DialogEditor::~DialogEditor()
{
    delete ui;
}

void DialogEditor::ChangedCurrentSuccessor()
{
    if (ui->successorList->currentItem() != 0)
      currentSuccessorName = ui->successorList->currentItem()->text(0);
}

void DialogEditor::NpcLineChanged()
{
    QString newLine = ui->npcLine->text();
    Data    data(tree);
    Data    node    = data[ui->nodeList->currentText().toStdString()];

    node["npcLine"] = newLine.toStdString();
    SaveData();
}

void DialogEditor::SuccessorChanged(QTreeWidgetItem* item, int column)
{
    {
        Data        data(tree);
        Data        node          = data[ui->nodeList->currentText().toStdString()];
        std::string successorName = item->text(0).toStdString();
        Data        successor     = node[successorName];
        Data        tmp;

        switch (column)
        {
        case 0:
            tmp = node[currentSuccessorName.toStdString()];
            tmp.SetKey(successorName);
            break ;
        case 1:
            successor.SetKey(successorName);
            successor["HookAvailable"] = item->text(column).toStdString();
            break ;
        case 2:
            successor["HookExecute"]   = item->text(column).toStdString();
            break ;
        case 3:
            successor["DefaultAnswer"] = item->text(column).toStdString();
            break ;
        }
    }
    SaveData();
}

void DialogEditor::SwapNode(QString str)
{
    Data           data(tree);
    Data           node(data[str.toStdString()]);
    Data::iterator it  = node.begin();
    Data::iterator end = node.end();

    ui->successorList->clear();
    for (; it != end ; ++it)
    {
        if ((*it).Key() == "npcLine")
          continue ;
        QString name          = QString::fromStdString((*it).Key());
        QString hookAvailable = QString::fromStdString((*it)["HookAvailable"]);
        QString hookExecute   = QString::fromStdString((*it)["HookExecute"]);
        QString defaultAnswer = QString::fromStdString((*it)["DefaultAnswer"]);

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->successorList);
        item->setText(0, name);
        item->setText(1, hookAvailable);
        item->setText(2, hookExecute);
        item->setText(3, defaultAnswer);
        item->setFlags (item->flags () | Qt::ItemIsEditable);
    }
    ui->npcLine->setText(QString::fromStdString(node["npcLine"].Value()));
}

void DialogEditor::LoadDialog(DataTree* tree)
{
    Data           data(tree);
    Data::iterator it  = data.begin();
    Data::iterator end = data.end();

    this->tree = tree;
    ui->nodeList->clear();
    ui->successorList->clear();
    for (; it != end ; ++it)
    {
        AddNode(QString::fromStdString((*it).Key()));
    }
    this->setEnabled(true);
}

void DialogEditor::AddNode(QString name)
{
    Data data(tree);

    data[name.toStdString()] = "";
    ui->nodeList->addItem(name);

    SaveData();
}

void DialogEditor::DeleteNode(void)
{
    if (ui->nodeList->count() > 0)
    {
        Data data(tree);

        ui->nodeList->removeItem(ui->nodeList->currentIndex());
        data[ui->nodeList->currentText().toStdString()].Remove();
        SaveData();
    }
}

void DialogEditor::SaveData(void)
{
    DataTree::Writers::JSON(tree, tree->GetSourceFile());
}

void DialogEditor::NewNode(void)
{
    QString name = QInputDialog::getText(this, "New dialog node", "Name");

    if (name != "")
    {
        Data data(tree);

        if (data[name.toStdString()].Nil())
          AddNode(name);
        else
          QMessageBox::warning(this, "Error", "A dialog node with that name already exist");
    }
}

void DialogEditor::NewSuccessor(void)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->successorList);

    item->setFlags(item->flags () | Qt::ItemIsEditable);
}

void DialogEditor::DeleteSuccessor(void)
{
    QTreeWidgetItem* item = ui->successorList->currentItem();

    if (item)
    {
      Data           data(tree);
      QString        itemName = item->text(0);

      delete item;
      data[ui->nodeList->currentText().toStdString()][itemName.toStdString()].Remove();
      SaveData();
    }
}

void DialogEditor::MoveUp(void)
{
    QTreeWidgetItem* item = ui->successorList->currentItem();
    int              row  = ui->successorList->currentIndex().row();

    if (item && row > 0)
    {
        Data data(tree);

        ui->successorList->takeTopLevelItem(row);
        ui->successorList->insertTopLevelItem(row - 1, item);
        ui->successorList->setCurrentItem(item);
        data[ui->nodeList->currentText().toStdString()][item->text(0).toStdString()].MoveUp();
        SaveData();
    }
}

void DialogEditor::MoveDown(void)
{
    QTreeWidgetItem* item = ui->successorList->currentItem();
    QTreeWidgetItem* below = ui->successorList->itemBelow(item);

    if (item && below)
    {
        Data data(tree);

        int row = ui->successorList->currentIndex().row();
        ui->successorList->takeTopLevelItem(row);
        ui->successorList->insertTopLevelItem(row + 1, item);
        ui->successorList->setCurrentItem(item);
        data[ui->nodeList->currentText().toStdString()][item->text(0).toStdString()].MoveDown();
        SaveData();
    }
}
