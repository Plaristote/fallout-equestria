#include "dialogobject.h"
#include "ui_dialogobject.h"

extern QString objectTypes[];

DialogObject::DialogObject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogObject), dialogItem(this)
{
    ui->setupUi(this);
    ui->dialog->setEnabled(false);

    for (short i = 0 ; i < 4 ; ++i)
      ui->type->addItem(objectTypes[i]);

    connect(ui->interactionTalkTo, SIGNAL(toggled(bool)), ui->dialog, SLOT(setEnabled(bool)));
    connect(ui->type, SIGNAL(currentIndexChanged(QString)), this, SLOT(UpdateType(QString)));
    connect(this, SIGNAL(accepted()), this, SLOT(Apply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(Reset()));

    connect(ui->itemList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ItemAdd()));
    connect(ui->inventory, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OpenItemDialog()));
    connect(&dialogItem, SIGNAL(accepted()), this, SLOT(ItemSave()));
}

DialogObject::~DialogObject()
{
    delete ui;
}

void DialogObject::SetObjectList(QStringList list)
{
    ui->itemList->clear();
    foreach (QString str, list)
    {
      ui->itemList->addItem(str);
    }
}

void DialogObject::SetCurrentObject(DynamicObject* object)
{
    this->object = object;

    switch (object->type)
    {
    case DynamicObject::Character:
        ui->type->setCurrentIndex(0);
        break ;
    case DynamicObject::Shelf:
        ui->type->setCurrentIndex(1);
        break ;
    case DynamicObject::Locker:
        ui->type->setCurrentIndex(2);
        break ;
    case DynamicObject::Door:
        ui->type->setCurrentIndex(3);
        break ;
    }
    UpdateType(ui->type->currentText());

    ui->model->setText(QString::fromStdString(object->strModel));
    ui->texture->setText(QString::fromStdString(object->strTexture));
    ui->script->setText(QString::fromStdString(object->script));
    ui->dialog->setText(QString::fromStdString(object->dialog));
    ui->statistsics->setText(QString::fromStdString(object->charsheet));

    ui->interactionUse->setChecked(object->interactions & Interactions::Use);
    ui->interactionTalkTo->setChecked(object->interactions & Interactions::TalkTo);
    ui->interactionUseObject->setChecked(object->interactions & Interactions::UseObject);
    ui->interactionUseSkill->setChecked(object->interactions & Interactions::UseSkill);

    ui->doorKey->setText(QString::fromStdString(object->key));
    ui->doorLocked->setChecked(object->locked);

    ui->inventory->clear();
    foreach (DataTree* tmp, itemsData)
      delete tmp;
    itemsData.clear();
    std::for_each(object->inventory.begin(), object->inventory.end(), [this](std::pair<std::string, int> itemPair)
    {
       DataTree*               json;
       QStringList             strings;

       json = DataTree::Factory::StringJSON(itemPair.first);
       {
         Data itemData(json);

         itemData["quantity"] = itemPair.second;
         strings << itemData["type"].Value().c_str() << itemData["quantity"].Value().c_str();
         ui->inventory->addTopLevelItem(new QTreeWidgetItem(strings));
         itemsData << new DataTree();
         Data newData(itemsData.last());
         newData["type"]     = itemData["type"].Value();
         newData["quantity"] = itemData["quantity"].Value();
         if (!(itemData["ammo"].Nil()))
         {
           if (!(itemData["ammo"]["ammount"].Nil()))
             newData["ammo"]["ammount"] = itemData["ammo"]["ammount"].Value();
           if (!(itemData["ammo"]["current"].Nil()))
             newData["ammo"]["current"] = itemData["ammo"]["current"].Value();
         }
       }
       delete json;
    });
}

void DialogObject::UpdateType(QString str)
{
    // 0 -> NPC | 1 -> Shelf | 2 -> Locker | 3 -> Door
    ui->script->setEnabled      (str == objectTypes[0]);
    ui->statistsics->setEnabled (str == objectTypes[0]);
    ui->tabInventory->setEnabled(str == objectTypes[0] || str == objectTypes[1] || str == objectTypes[2]);
    ui->tabDoor->setEnabled     (str == objectTypes[2] || str == objectTypes[3]);
}

void DialogObject::Apply()
{
    switch (ui->type->currentIndex())
    {
    case 0:
        object->type = DynamicObject::Character;
        break ;
    case 1:
        object->type = DynamicObject::Shelf;
        break ;
    case 2:
        object->type = DynamicObject::Locker;
        break ;
    case 3:
        object->type = DynamicObject::Door;
        break ;
    }
    object->dialog     = ui->dialog->text().toStdString();
    object->script     = ui->script->text().toStdString();
    object->strModel   = ui->model->text().toStdString();
    object->strTexture = ui->texture->text().toStdString();
    object->key        = ui->doorKey->text().toStdString();
    object->charsheet  = ui->statistsics->text().toStdString();

    unsigned char interactionFlag = 0;
    interactionFlag |= (ui->interactionTalkTo->isChecked()    ? Interactions::TalkTo    : 0);
    interactionFlag |= (ui->interactionUse->isChecked()       ? Interactions::Use       : 0);
    interactionFlag |= (ui->interactionUseObject->isChecked() ? Interactions::UseObject : 0);
    interactionFlag |= (ui->interactionUseSkill->isChecked()  ? Interactions::UseSkill  : 0);

    object->interactions = interactionFlag;

    object->inventory.clear();
    for (int i = 0 ; i < itemsData.size() ; ++i)
    {
        DataTree*   dataTree = itemsData[i];
        Data        data(dataTree);
        int         quantity = data["quantity"];
        std::string json;

        data["quantity"].Remove();
        DataTree::Writers::StringJSON(data, json);
        if (quantity > 0)
          object->inventory.push_back(std::pair<std::string, int>(json, quantity));
    }
}

void DialogObject::Reset()
{
    SetCurrentObject(object);
}

// Inventory
void DialogObject::ItemAdd()
{
    QString                 itemName = ui->itemList->currentItem()->text();
    QStringList             strings;

    strings << itemName << "1";
    ui->inventory->addTopLevelItem(new QTreeWidgetItem(strings));
    itemsData << new DataTree();
    Data newData(itemsData.last());
    newData["type"]     = itemName.toStdString();
    newData["quantity"] = 1;
}

void DialogObject::OpenItemDialog()
{
    int       it       = ui->inventory->currentIndex().row();
    DataTree* dataTree = (itemsData[it]);

    dialogItem.SetDataTree(dataTree);
    dialogItem.open();
}

void DialogObject::ItemSave()
{
    int              it   = ui->inventory->currentIndex().row();
    QTreeWidgetItem* item = ui->inventory->itemAt(0, it);
    Data             data((itemsData[it]));

    item->setText(1, data["quantity"].Value().c_str());
}
