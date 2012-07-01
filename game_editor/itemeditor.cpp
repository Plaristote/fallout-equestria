#include "itemeditor.h"
#include "ui_itemeditor.h"
#include <QDir>

ItemEditor::ItemEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemEditor)
{
    dataTree = 0;

    ui->setupUi(this);

    QIcon iconDelete("icons/delete.png");
    QIcon iconSave("icons/save.png");
    QIcon iconAdd("icons/add.png");

    ui->actionAdd->setIcon(iconAdd);
    ui->actionDelete->setIcon(iconDelete);
    ui->itemAdd->setIcon(iconAdd);
    ui->itemDelete->setIcon(iconDelete);
    ui->itemSave->setIcon(iconSave);

    connect(ui->itemList, SIGNAL(currentTextChanged(QString)), this, SLOT(SelectItem(QString)));
    connect(ui->actionList, SIGNAL(currentIndexChanged(QString)), this, SLOT(SelectAction(QString)));
}

ItemEditor::~ItemEditor()
{
    delete ui;
    if (dataTree)
      delete dataTree;
}

void ItemEditor::LoadAllItems()
{
    if (dataTree)
      delete dataTree;
    QString path = QDir::currentPath() + "/data/objects.json";
    dataTree = DataTree::Factory::JSON(path.toStdString());

    Data data(dataTree);
    Data::iterator it  = data.begin();
    Data::iterator end = data.end();

    ui->itemList->clear();
    for (; it != end ; ++it)
      ui->itemList->addItem((*it).Key().c_str());
    ui->itemList->sortItems();
}

void ItemEditor::SelectItem(QString key)
{
    if (key != "")
    {
        Data item = Data(dataTree)[key.toStdString()];

        ui->frame->setEnabled(true);
        ui->frameAction->setEnabled(true);
        ui->labelCurrentItem->setText(key);
        ui->itemIcon->setText(item["icon"].Value().c_str());
        ui->itemModel->setText(item["model"].Value().c_str());
        ui->itemTexture->setText(item["texture"].Value().c_str());

        if (item["scale"].Nil())
          ui->itemScale->setValue(0);
        else
          ui->itemScale->setValue(item["scale"]);

        if (item["weight"].Nil())
          ui->itemWeight->setValue(0);
        else
          ui->itemWeight->setValue(item["weight"]);

        ui->itemHidden->setChecked((item["hidden"].Value() != "1"));

        ui->actionList->clear();
        Data::iterator actionIt  = item["actions"].begin();
        Data::iterator actionEnd = item["actions"].end();

        for (; actionIt != actionEnd ; ++actionIt)
          ui->actionList->addItem((*actionIt).Key().c_str());

        //ui->itemScript->setText(item["script"]["file"].Value().c_str());
    }
    else
    {
        ui->frame->setEnabled(false);
        ui->frameAction->setEnabled(false);
    }
}

void ItemEditor::SelectAction(QString key)
{
    Data currentItem = Data(dataTree)[ui->itemList->currentItem()->text().toStdString()];

    if (!(currentItem.Nil()) && key != "")
    {
      Data currentAction = currentItem["actions"][key.toStdString()];

      ui->actionWidget->setEnabled(true);

      std::cout << "Loading action " << key.toStdString() << " for item " << currentItem.Key() << std::endl;

      ui->actionApCost->setValue(currentAction["ap-cost"]);
      ui->actionCombat->setChecked(currentAction["combat"].Value() == "1");
      ui->actionTargeted->setChecked(currentAction["targeted"].Value() != "0");
      ui->actionRange->setValue(currentAction["range"]);
      ui->actionDamage->setValue(currentAction["damage"]);
      ui->actionDamageMax->setValue(currentAction["damage-max"]);

      ui->actionHasHookWeapon->setChecked(!(currentAction["hookWeapon"].Nil()));
      if (ui->actionHasHookWeapon->isChecked())
        ui->actionHookWeapon->setText(currentAction["hookWeapon"].Value().c_str());
      else
        ui->actionHookWeapon->clear();

      ui->actionHasHookCharacters->setChecked(!(currentAction["hookCharacters"].Nil()));
      if (ui->actionHasHookCharacters->isChecked())
        ui->actionHookCharacters->setText(currentAction["hookCharacters"].Value().c_str());
      else
        ui->actionHookCharacters->clear();

      ui->actionHasHookDoors->setChecked(!(currentAction["hookDoors"].Nil()));
      if (ui->actionHasHookDoors->isChecked())
        ui->actionHookDoors->setText(currentAction["hookDoors"].Value().c_str());
      else
        ui->actionHookDoors->clear();

      ui->actionHasHookOthers->setChecked(!(currentAction["hookOthers"].Nil()));
      if (ui->actionHasHookOthers->isChecked())
        ui->actionHookOthers->setText(currentAction["hookOthers"].Value().c_str());
      else
        ui->actionHookOthers->clear();
    }
    else
      ui->actionWidget->setEnabled(false);
}
