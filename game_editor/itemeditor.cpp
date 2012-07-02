#include "itemeditor.h"
#include "ui_itemeditor.h"
#include <QDir>
#include <QInputDialog>

ItemEditor::ItemEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemEditor)
{
    dataTree = 0;
    loading  = false;

    ui->setupUi(this);

    QIcon iconDelete("icons/delete.png");
    QIcon iconSave("icons/save.png");
    QIcon iconAdd("icons/add.png");

    ui->actionAdd->setIcon(iconAdd);
    ui->actionDelete->setIcon(iconDelete);
    ui->itemAdd->setIcon(iconAdd);
    ui->itemDelete->setIcon(iconDelete);
    ui->itemSave->setIcon(iconSave);

    connect(ui->itemAdd, SIGNAL(clicked()), this, SLOT(ItemAdd()));
    connect(ui->itemDelete, SIGNAL(clicked()), this, SLOT(ItemDelete()));
    connect(ui->actionAdd, SIGNAL(clicked()), this, SLOT(ActionAdd()));
    connect(ui->actionDelete, SIGNAL(clicked()), this, SLOT(ActionDelete()));
    connect(ui->itemSave, SIGNAL(clicked()), this, SLOT(SaveData()));

    connect(ui->itemList, SIGNAL(currentTextChanged(QString)), this, SLOT(SelectItem(QString)));
    connect(ui->actionList, SIGNAL(currentIndexChanged(QString)), this, SLOT(SelectAction(QString)));

    connect(ui->itemHidden, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->itemIcon,   SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
    connect(ui->itemModel,  SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
    connect(ui->itemTexture, SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
    connect(ui->itemScale,  SIGNAL(valueChanged(double)), this, SLOT(UpdateData()));
    connect(ui->itemScript, SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
    connect(ui->itemModeBattleSaddle, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->itemModeMagic, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->itemModeMouth, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->scriptBattleSaddleWeild, SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->scriptMagicWeild, SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->scriptMouthWeild, SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->actionApCost, SIGNAL(valueChanged(int)), this, SLOT(UpdateData()));
    connect(ui->actionCombat, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->actionTargeted, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->actionDamage, SIGNAL(valueChanged(int)), this, SLOT(UpdateData()));
    connect(ui->actionDamageMax, SIGNAL(valueChanged(int)), this, SLOT(UpdateData()));
    connect(ui->actionHasHookCharacters, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->actionHasHookDoors, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->actionHasHookOthers, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->actionHasHookUse, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->actionHasHookWeapon, SIGNAL(clicked()), this, SLOT(UpdateData()));
    connect(ui->actionHookCharacters, SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
    connect(ui->actionHookDoors, SIGNAL(textEdited()), this, SLOT(UpdateData()));
    connect(ui->actionHookOthers, SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
    connect(ui->actionHookUse, SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
    connect(ui->actionHookWeapon, SIGNAL(textEdited(QString)), this, SLOT(UpdateData()));
}

ItemEditor::~ItemEditor()
{
    delete ui;
    if (dataTree)
      delete dataTree;
}

QStringList ItemEditor::GetItemList() const
{
    QStringList ret;

    for (int i = 0 ; i < ui->itemList->count() ; ++i)
      ret << ui->itemList->item(i)->text();
    return (ret);
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
    ItemListChanged(GetItemList());
}

void ItemEditor::ItemAdd(void)
{
    QString name = QInputDialog::getText(this, "New item", "Name");

    if (name != "")
      ui->itemList->addItem(name);
    ui->itemList->sortItems();
    ItemListChanged(GetItemList());
}

void ItemEditor::ItemDelete(void)
{
    QListWidgetItem* listItem    = ui->itemList->currentItem();
    QString          currentItem = listItem->text();

    Data(dataTree)[currentItem.toStdString()].Remove();
    ui->itemList->removeItemWidget(listItem);
    delete listItem;
    ItemListChanged(GetItemList());
}

void ItemEditor::ActionAdd(void)
{
    QString name = QInputDialog::getText(this, "New item action", "Name");

    if (name != "")
    {
      ui->actionList->addItem(name);
      ui->actionList->setCurrentIndex(ui->actionList->count() - 1);
    }
}

void ItemEditor::ActionDelete(void)
{
    QString currentItem   = ui->itemList->currentItem()->text();
    QString currentAction = ui->actionList->currentText();

    if (currentAction != "" && currentItem != "")
    {
        Data(dataTree)[currentItem.toStdString()]["actions"][currentAction.toStdString()].Remove();
        ui->actionList->removeItem(ui->actionList->currentIndex());
    }
}

void ItemEditor::SaveData(void)
{
    QString path = QDir::currentPath() + "/data/objects.json";

    DataTree::Writers::JSON(Data(dataTree), path.toStdString());
}

void ItemEditor::UpdateData(void)
{
    if (loading)
      return ;

    QString itemName = ui->itemList->currentItem()->text();
    Data    item     = Data(dataTree)[itemName.toStdString()];

    // Generalties
    item["icon"]           = ui->itemIcon->text().toStdString();
    item["model"]          = ui->itemModel->text().toStdString();
    item["texture"]        = ui->itemTexture->text().toStdString();
    item["scale"]          = ui->itemScale->value();
    item["weight"]         = ui->itemWeight->value();
    item["script"]["file"] = ui->itemScript->text().toStdString();

    if (ui->itemHidden->isChecked())
      item["hidden"].Remove();
    else
      item["hidden"] = "1";

    // Weild Types
    auto weildFunctor = [](Data item, QCheckBox* checkbox, QLineEdit* lineEdit, const std::string& mode, const std::string& hook)
    {
      if (checkbox->isChecked())
      {
        item[mode] = "1";
        if (lineEdit->text().length() > 0)
          item["script"][hook] = lineEdit->text().toStdString();
        else
          item["script"][hook].Remove();
      }
      else
        item[mode] = "0";
    };
    weildFunctor(item, ui->itemModeMouth,        ui->scriptMouthWeild,        "mode-mouth",        "hookWeildMouth");
    weildFunctor(item, ui->itemModeMagic,        ui->scriptMagicWeild,        "mode-magic",        "hookWeildMagic");
    weildFunctor(item, ui->itemModeBattleSaddle, ui->scriptBattleSaddleWeild, "mode-battlesaddle", "hookWeildBattleSaddle");

    // Actions
    QString actionName = ui->actionList->currentText();

    if (actionName != "")
    {
      Data action = item["actions"][actionName.toStdString()];

      action["ap-cost"]    = ui->actionApCost->value();
      action["range"]      = ui->actionRange->value();
      action["combat"]     = (ui->actionCombat->isChecked()   ? "1" : "0");
      action["targeted"]   = (ui->actionTargeted->isChecked() ? "1" : "0");
      action["damage"]     = ui->actionDamage->value();
      action["damage-max"] = ui->actionDamageMax->value();

      auto hookFunctor = [](Data action, QCheckBox* checkbox, QLineEdit* edit, const std::string& hook)
      {
        if (checkbox->isChecked())
          action[hook] = edit->text().toStdString();
        else
          action[hook].Remove();
      };
      hookFunctor(action, ui->actionHasHookUse,        ui->actionHookUse,        "hookUse");
      hookFunctor(action, ui->actionHasHookCharacters, ui->actionHookCharacters, "hookCharacters");
      hookFunctor(action, ui->actionHasHookDoors,      ui->actionHookDoors,      "hookDoors");
      hookFunctor(action, ui->actionHasHookOthers,     ui->actionHookOthers,     "hookOthers");
      hookFunctor(action, ui->actionHasHookWeapon,     ui->actionHookWeapon,     "hookWeapon");
    }
}

void ItemEditor::SelectItem(QString key)
{
    loading = true;
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

        ui->itemModeMouth->setChecked(item["mode-mouth"].Nil() || item["mode-mouth"].Value() == "1");
        ui->itemModeMagic->setChecked(item["mode-magic"].Nil() || item["mode-magic"].Value() == "1");
        ui->itemModeBattleSaddle->setChecked(item["mode-battlesaddle"].Value() == "1");

        ui->scriptMouthWeild->setText(item["script"]["hookWeildMouth"].Value().c_str());
        ui->scriptMagicWeild->setText(item["script"]["hookWeildMagic"].Value().c_str());
        ui->scriptBattleSaddleWeild->setText(item["script"]["hookWeildBattleSaddle"].Value().c_str());

        ui->actionList->clear();
        Data::iterator actionIt  = item["actions"].begin();
        Data::iterator actionEnd = item["actions"].end();

        for (; actionIt != actionEnd ; ++actionIt)
          ui->actionList->addItem((*actionIt).Key().c_str());

        ui->itemScript->setText(item["script"]["file"].Value().c_str());
    }
    else
    {
        ui->frame->setEnabled(false);
        ui->frameAction->setEnabled(false);
    }
    loading = false;
}

void ItemEditor::SelectAction(QString key)
{
    Data currentItem = Data(dataTree)[ui->itemList->currentItem()->text().toStdString()];

    loading = true;
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
    loading = false;
}
