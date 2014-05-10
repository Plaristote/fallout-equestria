#ifndef INVENTORYEDITOR_H
#define INVENTORYEDITOR_H

#include <QWidget>
#include <string>
#include <QStringList>
#include "datatree.hpp"

namespace Ui {
  class InventoryEditor;
}

class InventoryEditor : public QWidget
{
  Q_OBJECT

public:
  explicit InventoryEditor(QWidget *parent = 0);
  ~InventoryEditor();

  void SetInventory(Data);

private slots:
  void AddItem(void);
  void DeleteItem(void);
  void SelectItem(void);
  void UpdateCurrentItem(void);

  void SetLeftEquippedItem();
  void SetRightEquippedItem();
  void SetArmourEquippedItem();
  void SetBattlesaddleEquippedItem();

private:
  void UnsetEquipedItem(const std::string& slot, unsigned short slot_number);
  void SetEquipedItem(const std::string item_name, const std::string& slot, unsigned short slot_number, unsigned char mode);

  void        UpdateEquippedItemUi();
  QStringList GetAmmunitionsForItem(const std::string& item_name);

  bool                disable_update;
  Data                inventory;
  Ui::InventoryEditor *ui;
};

#endif // INVENTORYEDITOR_H
