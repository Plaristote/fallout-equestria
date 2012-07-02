#include "dialoginventoryitem.h"
#include "ui_dialoginventoryitem.h"

DialogInventoryItem::DialogInventoryItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInventoryItem)
{
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(SaveChanges()));
}

DialogInventoryItem::~DialogInventoryItem()
{
    delete ui;
}

void DialogInventoryItem::SetDataTree(DataTree *p)
{
    Data data(p);

    dataTree = p;
    ui->itemName->setText(data["type"].Value().c_str());
    ui->quantity->setValue(data["quantity"]);

    ui->ammoInBarrel->setValue(-1);
    ui->ammoType->setText("");

    if (!data["ammo"].Nil())
    {
      if (!(data["ammo"]["ammount"].Nil()))
        ui->ammoInBarrel->setValue(data["ammo"]["ammount"]);
      if (!(data["ammo"]["current"].Nil()))
        ui->ammoType->setText(data["ammo"]["current"].Value().c_str());
    }
}

void DialogInventoryItem::SaveChanges()
{
    Data data(dataTree);

    data["quantity"] = ui->quantity->value();
    if (ui->ammoInBarrel->value() > -1)
      data["ammo"]["ammount"] = ui->ammoInBarrel->value();
    if (ui->ammoType->text().size() > 0)
      data["ammo"]["current"] = ui->ammoType->text().toStdString();
}
