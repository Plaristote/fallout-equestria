#include "enlightenedobjectwidget.h"

using namespace std;

EnlightenedObjectsWidget::EnlightenedObjectsWidget(QWidget* parent) : QTableWidget(parent)
{
  connect(this, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(ChangeCurrentRow(int,int,int,int)));
}

void EnlightenedObjectsWidget::ChangeCurrentRow(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
  if (currentRow != previousRow)
  {
    SetRowEnabled(previousRow, false);
    SetRowEnabled(currentRow,  true);
  }
}

void EnlightenedObjectsWidget::SetRowEnabled(int row, bool enabled)
{
  QWidget*   spinbox  = cellWidget(row, 1);
  QWidget*   checkbox = cellWidget(row, 2);

  if (spinbox)
    spinbox->setEnabled(enabled);
  if (checkbox)
    checkbox->setEnabled(enabled);

}

void EnlightenedObjectsWidget::AddEnlightenedObject(const QString& name, unsigned short priority, bool propagates)
{
  QTableWidgetItem* name_item       = new QTableWidgetItem;
  QSpinBox*         priority_item   = new QSpinBox;
  QCheckBox*        propagates_item = new QCheckBox;

  name_item->setText(name);
  name_item->setFlags(name_item->flags() ^ Qt::ItemIsEditable);
  priority_item->setValue(priority);
  propagates_item->setChecked(propagates);
  insertRow(rowCount());
  setItem(rowCount() - 1, 0, name_item);
  setCellWidget(rowCount() - 1, 1, priority_item);
  setCellWidget(rowCount() - 1, 2, propagates_item);
  setCurrentCell(rowCount() - 1, 0);
  connect(priority_item,   SIGNAL(valueChanged(int)), this, SIGNAL(updatedPriority()));
  connect(propagates_item, SIGNAL(toggled(bool)),     this, SIGNAL(updatedPropagation()));
}

unsigned short EnlightenedObjectsWidget::GetPriority(int row)
{
  QWidget*  widget  = cellWidget(row, 1);
  QSpinBox* spinbox = reinterpret_cast<QSpinBox*>(widget);

  return (spinbox ? spinbox->value() : 1);
}

bool EnlightenedObjectsWidget::PropagatesToChildren(int row)
{
  QWidget*   widget   = cellWidget(row, 2);
  QCheckBox* checkbox = reinterpret_cast<QCheckBox*>(widget);

  return (checkbox ? checkbox->isChecked() : false);
}

QString EnlightenedObjectsWidget::Name(int row)
{
  QTableWidgetItem* item = this->item(row, 0);

  return (item ? item->text() : "");
}

void EnlightenedObjectsWidget::clear(void)
{
  while (rowCount())
    removeRow(0);
}

void EnlightenedObjectsWidget::removeRow(int row)
{
  QTableWidgetItem* item1 = item(row, 0);
  QWidget*          item2 = cellWidget(row, 1);
  QWidget*          item3 = cellWidget(row, 2);

  if (item1)
    delete item1;
  if (item2)
    delete item2;
  if (item3)
    delete item3;
  QTableWidget::removeRow(row);
}
