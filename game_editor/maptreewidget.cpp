#include "maptreewidget.h"
#include <QMessageBox>
#include <QMimeData>
#include "world.h"

MapTreeWidget::MapTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
  connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ItemFocused()));
  icon_map_object = QIcon("icons/tree-map-object.png");
  icon_dyn_object = QIcon("icons/tree-dyn-object.png");
  icon_light      = QIcon("icons/tree-light.png");
}

void MapTreeWidget::ItemFocused()
{
  QTreeWidgetItem* item = this->currentItem();

  if (item)
  {
    std::string name = item->text(0).toStdString();

    cout << "Focused item " << name << endl;
    switch (GetType(item))
    {
    case ItemMapObject:
      FocusObject(world->GetMapObjectFromName(name));
      break ;
    case ItemDynamicObject:
      FocusDynamicObject(world->GetDynamicObjectFromName(name));
      break ;
    case ItemLight:
      FocusLight(world->GetLightByName(name));
      break ;
    case ItemUnknown:
      break ;
    }
  }
}

MapTreeWidget::ItemType MapTreeWidget::GetType(QTreeWidgetItem* item) const
{
  if (item != 0)
  {
    std::string    name       = item->text(0).toStdString();

    if (world->GetMapObjectFromName(name) != 0)
      return (ItemMapObject);
    if (world->GetDynamicObjectFromName(name) != 0)
      return (ItemDynamicObject);
    if (world->GetLightByName(name) != 0)
      return (ItemLight);
  }
  return (ItemUnknown);
}

bool MapTreeWidget::dropMimeData(QTreeWidgetItem *parent, int, const QMimeData *data, Qt::DropAction)
{
  ItemType parent_type = GetType(parent);

  switch (parent_type)
  {
  // Cannot drop on lights objects
  case ItemLight:
    return (false);
  default:
    break ;
  }

  if (data->hasFormat("application/x-qabstractitemmodeldatalist"))
  {
    QTreeWidgetItem* current = this->currentItem();
    QTreeWidgetItem* clone;

    /*
     * Aborting if dropping in a sub-branch
     */
    for (int i = 0 ; i < current->childCount() ; ++i)
    {
      QTreeWidgetItem* item = current->child(i);

      if (item == parent)
        return (false);
    }

    /*
     * Reparenting the objects in the TreeWidget
     */
    if (parent != 0)
      clone = new QTreeWidgetItem(parent);
    else
      clone = new QTreeWidgetItem(this);
    clone->setText(0, current->text(0));
    while (current->childCount())
    {
      QTreeWidgetItem* item = current->child(0);

      current->removeChild(item);
      clone->addChild(item);
    }

    SetItemIcon(clone, GetType(clone));
    delete current;

    /*
     * Reparenting the objects in World
     */
    {
      ItemType    type           = GetType(clone);
      std::string name           = clone->text(0).toStdString();

      if (type == ItemMapObject || type == ItemDynamicObject)
      {
         MapObject*  tmp_map_object = 0;

         if (type == ItemMapObject)
           tmp_map_object = world->GetMapObjectFromName(name);
         else
           tmp_map_object = world->GetDynamicObjectFromName(name);
         if (parent == 0)
           world->ReparentObject(tmp_map_object, 0);
         else
           world->ReparentObject(tmp_map_object, parent->text(0).toStdString());
      }
      else if (type == ItemLight)
      {
        WorldLight* light = world->GetLightByName(name);

        switch (parent_type)
        {
        case ItemUnknown:
        case ItemLight:
          light->parent_type = WorldLight::Type_None;
          break ;
        case ItemMapObject:
          light->ReparentTo(world->GetMapObjectFromName(parent->text(0).toStdString()));
          break ;
        case ItemDynamicObject:
          light->ReparentTo(world->GetDynamicObjectFromName(parent->text(0).toStdString()));
          break ;
        }
      }
    }
    return (true);
  }
  return (false);
}

void MapTreeWidget::SetItemIcon(QTreeWidgetItem* item, ItemType type)
{
  switch (type)
  {
  case ItemMapObject:
    item->setIcon(0, icon_map_object);
    break ;
  case ItemDynamicObject:
    item->setIcon(0, icon_dyn_object);
    break ;
  case ItemLight:
    item->setIcon(0, icon_light);
    break ;
  case ItemUnknown:
    break ;
  }
}

void MapTreeWidget::ProbeWorld(const std::string& parent_name, QTreeWidgetItem* parent)
{
  ItemType                        type;
  std::function<void(MapObject&)> functor = [this, parent_name, parent, &type](MapObject& object)
  {
    if (object.parent == parent_name)
    {
      std::string      name = object.nodePath.get_name();
      QTreeWidgetItem* new_item;

      if (parent != 0)
        new_item = new QTreeWidgetItem(parent);
      else
        new_item = new QTreeWidgetItem(this);
      new_item->setText(0, QString::fromStdString(name));
      SetItemIcon(new_item, type);
      ProbeWorld(name, new_item);
    }
  };

  type = ItemMapObject;
  std::for_each(world->objects.begin(),        world->objects.end(),        functor);
  type = ItemDynamicObject;
  std::for_each(world->dynamicObjects.begin(), world->dynamicObjects.end(), functor);
  std::for_each(world->lights.begin(), world->lights.end(), [this, parent_name, parent](WorldLight& light)
  {
    if ((light.Parent() == 0 && parent_name == "") ||
        (light.Parent() != 0 && light.Parent()->nodePath.get_name() == parent_name))
    {
      QTreeWidgetItem* new_item;

      if (parent != 0)
          new_item = new QTreeWidgetItem(parent);
      else
          new_item = new QTreeWidgetItem(this);
      new_item->setText(0, light.name.c_str());
      SetItemIcon(new_item, ItemLight);
    }
  });
}

void MapTreeWidget::SetWorld(World* world)
{
  this->world = world;
  this->clear();
  ProbeWorld("", 0);
}
