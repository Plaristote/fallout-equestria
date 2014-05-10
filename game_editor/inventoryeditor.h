#ifndef INVENTORYEDITOR_H
#define INVENTORYEDITOR_H

#include <QWidget>

namespace Ui {
  class InventoryEditor;
}

class InventoryEditor : public QWidget
{
  Q_OBJECT

public:
  explicit InventoryEditor(QWidget *parent = 0);
  ~InventoryEditor();

private:
  Ui::InventoryEditor *ui;
};

#endif // INVENTORYEDITOR_H
