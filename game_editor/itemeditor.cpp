#include "itemeditor.h"
#include "ui_itemeditor.h"
#include <QDir>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "selectableresource.h"

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

    connect(ui->itemHidden,              SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->itemIcon,                SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->itemModel,               SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->itemTexture,             SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->itemScale,               SIGNAL(valueChanged(double)),SLOT(UpdateData()));
    connect(ui->itemScript,              SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->itemAmmoMaximum,         SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->itemAmmo,                SIGNAL(valueChanged(int)),   SLOT(UpdateData()));
    connect(ui->actionHookCanUse,        SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->actionHookHitChances,    SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->actionSkill,             SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->actionApCost,            SIGNAL(valueChanged(int)),   SLOT(UpdateData()));
    connect(ui->actionCombat,            SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->actionTargeted,          SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->actionDamage,            SIGNAL(valueChanged(int)),   SLOT(UpdateData()));
    connect(ui->actionDamageMax,         SIGNAL(valueChanged(int)),   SLOT(UpdateData()));
    connect(ui->actionHasHookCharacters, SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->actionHasHookDoors,      SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->actionHasHookOthers,     SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->actionHasHookUse,        SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->actionHasHookWeapon,     SIGNAL(clicked()),           SLOT(UpdateData()));
    connect(ui->actionHookCharacters,    SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->actionHookOthers,        SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->actionHookUse,           SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->actionHookWeapon,        SIGNAL(textEdited(QString)), SLOT(UpdateData()));
    connect(ui->selectScript,            SIGNAL(clicked()),           SLOT(SelectScript()));
    connect(ui->selectModel,             SIGNAL(clicked()),           SLOT(SelectModel()));
    connect(ui->selectTexture,           SIGNAL(clicked()),           SLOT(SelectTexture()));
}

ItemEditor::~ItemEditor()
{
    delete ui;
    if (dataTree)
      delete dataTree;
}

void ItemEditor::SelectModel(void)
{
  QString filter    = "Panda3D Models (*.egg *.bam *.egg.pz *.bam.pz *.obj)";
  QString base_path = QDir::currentPath() + "/models/";
  QString path      = QFileDialog::getOpenFileName(this, "Select a model", base_path, filter);
  QFileInfo info(path);
  QString   relative_path;

  if (!(info.exists()))
    return ;
  if (!(path.startsWith(base_path))) // Needs to be moved
  {
    if (!(QFile::copy(path, base_path + info.fileName())))
    {
      QMessageBox::warning(this, "Error", "Couldn't copy file to the project directory.");
      return ;
    }
    path = base_path + info.fileName();
  }
  relative_path = path.remove(0, base_path.length());
  ui->itemModel->setText(relative_path);
}

void ItemEditor::SelectTexture(void)
{
  QString   filter    = "Images (*.png *.jpg *.bmp)";
  QString   base_path = QDir::currentPath() + "/textures/";
  QString   path = QFileDialog::getOpenFileName(this, "Select a texture", base_path, filter);
  QFileInfo info(path);
  QString   relative_path;

  if (!(info.exists()))
    return ;
  if (!(path.startsWith(base_path))) // Needs to be moved
  {
    if (!(QFile::copy(path, base_path + info.fileName())))
    {
      QMessageBox::warning(this, "Error", "Couldn't copy file to the project directory.");
      return ;
    }
    path = base_path + info.fileName();
  }
  relative_path = path.remove(0, base_path.length());
  ui->itemTexture->setText(relative_path);
}

void ItemEditor::SelectScript(void)
{
  SelectableResource::ObjectScript().SelectResource([this](QString name)
  {
    ui->itemScript->setText(name + ".as");
  });
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
    item["icon"]            = ui->itemIcon->text().toStdString();
    item["model"]           = ui->itemModel->text().toStdString();
    item["texture"]         = ui->itemTexture->text().toStdString();
    item["scale"]           = ui->itemScale->value();
    item["weight"]          = ui->itemWeight->value();
    item["ammo"]["maximum"] = ui->itemAmmoMaximum->value();
    item["script"]["file"]  = ui->itemScript->text().toStdString();

    if (ui->itemHidden->isChecked())
      item["hidden"].Remove();
    else
      item["hidden"] = "1";

    // Actions
    QString actionName = ui->actionList->currentText();

    if (actionName != "")
    {
      Data action = item["actions"][actionName.toStdString()];

      action["type"]           = ui->actionSkill->text().toStdString();
      action["ap-cost"]        = ui->actionApCost->value();
      action["range"]          = ui->actionRange->value();
      action["combat"]         = (ui->actionCombat->isChecked()   ? "1" : "0");
      action["targeted"]       = (ui->actionTargeted->isChecked() ? "1" : "0");
      action["damage"]         = ui->actionDamage->value();
      action["damage-max"]     = ui->actionDamageMax->value();
      action["hookCanUse"]     = ui->actionHookCanUse->text().toStdString();
      action["hookHitChances"] = ui->actionHookHitChances->text().toStdString();

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

        ui->itemAmmoMaximum->setValue(item["ammo"]["maximum"]);
        //ui->itemAmmo->setText();

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

      ui->actionCombat->setChecked  (currentAction["combat"].Value() == "1");
      ui->actionTargeted->setChecked(currentAction["targeted"].Value() != "0");
      ui->actionApCost->setValue    (currentAction["ap-cost"]    || 0);
      ui->actionRange->setValue     (currentAction["range"]      || 0);
      ui->actionDamage->setValue    (currentAction["damage"]     || 0);
      ui->actionDamageMax->setValue (currentAction["damage-max"] || 0);
      ui->actionSkill->setText(currentAction["type"].Value().c_str());
      ui->actionHookCanUse->setText(currentAction["hookCanUse"].Value().c_str());
      ui->actionHookHitChances->setText(currentAction["hookHitChances"].Value().c_str());

      ui->actionHasHookUse->setChecked(!(currentAction["hookUse"].Nil()));
      if (ui->actionHasHookUse->isChecked())
        ui->actionHookUse->setText(currentAction["hookUse"].Value().c_str());
      else
        ui->actionHookUse->clear();

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
