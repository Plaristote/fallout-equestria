#include "dialogobject.h"
#include "ui_dialogobject.h"
#include "selectableresource.h"
#include <QFileDialog>
#include <QMessageBox>

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

    connect(ui->toolSelectCharsheet, SIGNAL(clicked()), this, SLOT(SelectCharsheet()));
    connect(ui->toolSelectScript,    SIGNAL(clicked()), this, SLOT(SelectScript()));
    connect(ui->toolSelectDialog,    SIGNAL(clicked()), this, SLOT(SelectDialog()));

    connect(ui->modelPick,   SIGNAL(clicked()), this, SLOT(SelectModel()));
    connect(ui->texturePick, SIGNAL(clicked()), this, SLOT(SelectTexture()));
}

DialogObject::~DialogObject()
{
    delete ui;
}

void DialogObject::SetWaypointSelection(std::list<Waypoint*> waypoints)
{
    auto    it  = waypoints.begin();
    auto    end = waypoints.end();
    QString str;

    for (; it != end ; ++it)
    {
      if (str.size() > 0)
        str += ',';
      str += QString::number((*it)->id);
    }
    ui->doorWaypointsLine->setText(str);
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
        ui->passageway->setEnabled(true);
        break ;
    case DynamicObject::Item:
        ui->type->setCurrentIndex(4);
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
    ui->toolSelectCharsheet->setEnabled(str == objectTypes[0]);
    ui->toolSelectScript->setEnabled   (str == objectTypes[0]);
    ui->tabInventory->setEnabled       (str == objectTypes[0] || str == objectTypes[1] || str == objectTypes[2]);
    ui->tabDoor->setEnabled            (str == objectTypes[2] || str == objectTypes[3]);
    ui->lock->setEnabled               (str == objectTypes[1] || str == objectTypes[2] || str == objectTypes[3]);
    ui->passageway->setEnabled         (str == objectTypes[3]);
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

    if (object->strModel != ui->model->text().toStdString())
    {
        std::string name         = object->nodePath.get_name();
        std::string model_path   = ui->model->text().toStdString();
        LPoint3   pos   = object->nodePath.get_pos();
        LVecBase3 hpr   = object->nodePath.get_hpr();
        LVecBase3 scale = object->nodePath.get_scale();

        object->nodePath.remove_node();
        object->nodePath = world->window->load_model(world->window->get_panda_framework()->get_models(), MODEL_ROOT + model_path);
        object->nodePath.set_name(name);
        object->nodePath.set_pos(pos);
        object->nodePath.set_hpr(hpr);
        object->nodePath.set_scale(scale);
        object->nodePath.set_collide_mask(CollideMask(ColMask::DynObject));
        object->nodePath.reparent_to(world->rootDynamicObjects);
        world->DynamicObjectSetWaypoint(*object, *object->waypoint);
    }
    if (object->strTexture != ui->texture->text().toStdString())
    {
        std::string texture_path = ui->texture->text().toStdString();

        if (texture_path != "")
        {
          object->texture = TexturePool::load_texture(TEXT_ROOT + texture_path);
          object->nodePath.set_texture(object->texture);
        }
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

void DialogObject::SelectCharsheet()
{
  SelectableResource::Charsheets().SelectResource([this](QString name)
  {
    ui->statistsics->setText(name);
  });
}

void DialogObject::SelectScript()
{
  SelectableResource::AIs().SelectResource([this](QString name)
  {
    ui->script->setText(name);
  });
}

void DialogObject::SelectDialog()
{
  SelectableResource::Dialogs().SelectResource([this](QString name)
  {
    ui->dialog->setText(name);
  });
}

void DialogObject::SelectModel(void)
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
    ui->model->setText(relative_path);
}

void DialogObject::SelectTexture(void)
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
    ui->texture->setText(relative_path);
}
