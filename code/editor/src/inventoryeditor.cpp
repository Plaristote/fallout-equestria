#include "globals.hpp"
#include "inventoryeditor.h"
#include "ui_inventoryeditor.h"
#include "selectableresource.h"
#include <iostream>

using namespace std;

InventoryEditor::InventoryEditor(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::InventoryEditor)
{
  QIcon iconAdd("icons/add.png");
  QIcon iconDelete("icons/delete.png");

  ui->setupUi(this);

  ui->addItem->setIcon(iconAdd);
  ui->deleteItem->setIcon(iconDelete);

  disable_update = false;
  connect(ui->addItem,              SIGNAL(clicked()),                this, SLOT(AddItem()));
  connect(ui->deleteItem,           SIGNAL(clicked()),                this, SLOT(DeleteItem()));
  connect(ui->itemList,             SIGNAL(currentRowChanged(int)), this, SLOT(SelectItem()));
  connect(ui->equippedLeftMode,     SIGNAL(currentIndexChanged(int)), this, SLOT(SetLeftEquippedItem()));
  connect(ui->equippedLeft,         SIGNAL(currentIndexChanged(int)), this, SLOT(SetLeftEquippedItem()));
  connect(ui->equippedRightMode,    SIGNAL(currentIndexChanged(int)), this, SLOT(SetRightEquippedItem()));
  connect(ui->equippedRight,        SIGNAL(currentIndexChanged(int)), this, SLOT(SetRightEquippedItem()));
  connect(ui->equippedArmour,       SIGNAL(currentIndexChanged(int)), this, SLOT(SetArmourEquippedItem()));
  connect(ui->equippedBattlesaddle, SIGNAL(currentIndexChanged(int)), this, SLOT(SetBattlesaddleEquippedItem()));

  connect(ui->ammoCount,         SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentItem()));
  connect(ui->itemQuantity,      SIGNAL(valueChanged(int)), this, SLOT(UpdateCurrentItem()));
  connect(ui->currentAmmunition, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateCurrentItem()));
}

InventoryEditor::~InventoryEditor()
{
  delete ui;
}

QStringList InventoryEditor::GetAmmunitionsForItem(const std::string& item_name)
{
  DataTree*   objects_tree = DataTree::Factory::JSON("data/objects.json");
  QStringList list;

  if (objects_tree)
  {
    {
      Data objects(objects_tree);
      Data item = objects[item_name];

      if (item.NotNil())
      {
        std::for_each(item["ammo"]["types"].begin(), item["ammo"]["types"].end(), [&list](Data ammo_type)
        { list << QString::fromStdString(ammo_type.Value()); });
      }
    }
    delete objects_tree;
  }
  return (list);
}

QString InventoryEditor::GetIconForItem(const std::string& item_name)
{
  DataTree*   objects_tree = DataTree::Factory::JSON("data/objects.json");
  QStringList list;

  if (objects_tree)
  {
    {
      Data objects(objects_tree);
      Data item = objects[item_name];

      if (item.NotNil())
        return (item["icon"].Value().c_str());
    }
    delete objects_tree;
  }
  return ("default.png");
}

void InventoryEditor::SetInventory(Data data)
{
  disable_update = true;
  inventory = data;
  {
    auto it  = data.begin();
    auto end = data.end();

    ui->itemList->clear();
    for (; it != end ; ++it)
      ui->itemList->addItem(QString::fromStdString((*it)["Name"].Value()));
    UpdateEquippedItemUi();
  }
  disable_update = false;
}

void InventoryEditor::UpdateEquippedItemUi()
{
  disable_update = true;
  QComboBox* equippedComboBoxes[] = { ui->equippedLeft, ui->equippedRight, ui->equippedArmour, ui->equippedBattlesaddle };
  for (int i = 0 ; i < 4 ; ++i)
  {
    equippedComboBoxes[i]->clear();
    equippedComboBoxes[i]->addItem("");
  }
  if (inventory.NotNil())
  {
    auto it  = inventory.begin();
    auto end = inventory.end();

    for (; it != end ; ++it)
    {
      Data item = *it;

      if (item["equiped"].NotNil())
      {
        std::string  target        = item["equiped"]["target"].Value();
        unsigned int slot          = item["equiped"]["slot"];
        unsigned int mode          = item["equiped"]["mode"];
        QComboBox*   equipped      = 0;
        QComboBox*   equipped_mode = 0;

        if (target == "equiped")
        {
          equipped      = slot == 0 ? ui->equippedLeft     : ui->equippedRight;
          equipped_mode = slot == 0 ? ui->equippedLeftMode : ui->equippedRightMode;
        }
        else if (target == "armour")
          equipped = ui->equippedArmour;
        else if (target == "battlesaddle")
          equipped = ui->equippedBattlesaddle;
        if (equipped != 0)
        {
          equipped->addItem(item["Name"].Value().c_str());
          equipped->setCurrentIndex(equipped->count() - 1);
        }
        if (equipped_mode != 0)
          equipped_mode->setCurrentIndex(mode);
      }
      else
      {
        ui->equippedLeft->addItem(item["Name"].Value().c_str());
        ui->equippedRight->addItem(item["Name"].Value().c_str());
        ui->equippedArmour->addItem(item["Name"].Value().c_str());
        ui->equippedBattlesaddle->addItem(item["Name"].Value().c_str());
      }
    }
  }
  disable_update = false;
}

void InventoryEditor::AddItem(void)
{
  SelectableResource::Items().SelectResource([this](QString name)
  {
    inventory["new_item"]["Name"] = name.toStdString();
    inventory["new_item"].SetKey("");
    inventory.Output();
    ui->itemList->addItem(name);
    UpdateEquippedItemUi();
  });
}

void InventoryEditor::DeleteItem(void)
{
  int              index = ui->itemList->currentRow();
  QListWidgetItem* item  = ui->itemList->currentItem();

  inventory[index].Remove();
  inventory.Output();
  ui->itemList->removeItemWidget(item);
  delete item;
  UpdateEquippedItemUi();
}

void InventoryEditor::SelectItem(void)
{
  if (ui->itemList->currentItem() != 0)
  {
    int  index = ui->itemList->currentRow();
    Data item  = inventory[index];

    disable_update = true;
    {
      QStringList available_ammo = GetAmmunitionsForItem(item["Name"].Value());

      ui->currentAmmunition->clear();
      foreach (QString ammo_type, available_ammo)
      {
        ui->currentAmmunition->addItem(ammo_type);
        if (ammo_type.toStdString() == item["ammo"]["current"].Value())
          ui->currentAmmunition->setCurrentIndex(ui->currentAmmunition->count() - 1);
      }
    }
    ui->iconPreview->setText("<div align='center'><img src='textures/itemIcons/" + GetIconForItem(item["Name"]) + "' /></div>");
    ui->itemQuantity->setValue(item["quantity"].Or(0));
    ui->ammoCount->setValue(item["ammo"]["amount"].Or(0));
    disable_update = false;
  }
}

void InventoryEditor::UpdateCurrentItem(void)
{
  if (disable_update == false)
  {
    int  index = ui->itemList->currentRow();
    Data item  = inventory[index];

    if (ui->itemQuantity->value() == 0)
      item["quantity"].Remove();
    else
      item["quantity"]       = ui->itemQuantity->value();
    if (ui->ammoCount->value() == 0)
      item["ammo"]["amount"].Remove();
    else
      item["ammo"]["amount"] = ui->ammoCount->value();
    item["ammo"]["current"] = ui->currentAmmunition->currentText().toStdString();
  }
}

void InventoryEditor::SetLeftEquippedItem()
{
  unsigned char mode = ui->equippedLeftMode->currentIndex();

  SetEquipedItem(ui->equippedLeft->currentText().toStdString(), "equiped", 0, mode);
}

void InventoryEditor::SetRightEquippedItem()
{
  unsigned char mode = ui->equippedRightMode->currentIndex();

  SetEquipedItem(ui->equippedRight->currentText().toStdString(), "equiped", 1, mode);
}

void InventoryEditor::SetArmourEquippedItem()
{
  SetEquipedItem(ui->equippedArmour->currentText().toStdString(), "armour", 0, 0);
}

void InventoryEditor::SetBattlesaddleEquippedItem()
{
  SetEquipedItem(ui->equippedBattlesaddle->currentText().toStdString(), "battlesaddle", 0, 0);
}

void InventoryEditor::UnsetEquipedItem(const std::string& slot, unsigned short slot_number)
{
  auto it  = inventory.begin();
  auto end = inventory.end();

  for (; it != end ; ++it)
  {
    Data item = *it;

    if (item["equiped"].NotNil())
    {
        if (item["equiped"]["target"].Value() == slot &&
            (item["equiped"]["slot"].Nil() || item["equiped"]["slot"] == slot_number))
          item["equiped"].Remove();
    }
  }
}

void InventoryEditor::SetEquipedItem(const std::string item_name, const std::string& slot, unsigned short slot_number, unsigned char mode)
{
  if (disable_update) return ;
  UnsetEquipedItem(slot, slot_number);
  if (item_name != "")
  {
    auto it  = inventory.begin();
    auto end = inventory.end();

    for (; it != end ; ++it)
    {
      Data item = *it;

      if (item["Name"].Value() == item_name)
      {
        item["equiped"]["target"] = slot;
        item["equiped"]["slot"]   = slot_number;
        item["equiped"]["mode"]   = (int)mode;
        break ;
      }
    }
  }
}
