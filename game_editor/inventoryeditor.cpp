#include "inventoryeditor.h"
#include "ui_inventoryeditor.h"

InventoryEditor::InventoryEditor(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::InventoryEditor)
{
  ui->setupUi(this);
}

InventoryEditor::~InventoryEditor()
{
  delete ui;
}
