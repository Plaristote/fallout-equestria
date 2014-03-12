#include "maptreewidget.h"
#include <QMessageBox>
#include <QMimeData>
#include "world/world.h"
#include <QTreeWidgetItemIterator>

using namespace std;

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

void MapTreeWidget::SetItemFocused(MapObject* object)
{
  QTreeWidgetItemIterator it(this);

  while (*it)
  {
    if ((*it)->text(0).toStdString() == object->nodePath.get_name())
    {
      this->setCurrentItem(*it);
      break ;
    }
    ++it;
  }
}

void MapTreeWidget::Rename(QString old_name, QString new_name)
{
  QTreeWidgetItemIterator it(this);

  while (*it)
  {
    if ((*it)->text(0) == old_name)
    {
      (*it)->setText(0, new_name);
      break ;
    }
    ++it;
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

void MapTreeWidget::ReparentTo(QTreeWidgetItem* current, QTreeWidgetItem* parent)
{
  if (current == parent) return ;
  ItemType         parent_type = GetType(parent);
  QTreeWidgetItem* clone       = 0;

  /*
   * Aborting if dropping in a sub-branch
   */
  if (!current)
    return ;
  for (int i = 0 ; i < current->childCount() ; ++i)
  {
    QTreeWidgetItem* item = current->child(i);

    if (item == parent)
      return ;
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
  *(map.find(current->text(0))) = clone;
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
        light->ReparentTo(world);
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
    ReparentTo(this->currentItem(), parent);
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

void MapTreeWidget::AddMapObject(MapObject* object)
{
  QTreeWidgetItem* new_item = new QTreeWidgetItem(this);
  QString          name     = QString::fromStdString(object->nodePath.get_name());

  new_item->setText(0, name);
  SetItemIcon(new_item, ItemMapObject);
  map.insert(name, new_item);
}

void MapTreeWidget::AddDynamicObject(DynamicObject* object)
{
  QTreeWidgetItem* new_item = new QTreeWidgetItem(this);
  QString          name     = QString::fromStdString(object->nodePath.get_name());

  new_item->setText(0, name);
  SetItemIcon(new_item, ItemDynamicObject);
  map.insert(name, new_item);
}

void MapTreeWidget::AddWorldLight(WorldLight* light)
{
  QTreeWidgetItem* new_item = new QTreeWidgetItem(this);
  QString          name     = QString::fromStdString(light->name);

  new_item->setText(0, name);
  SetItemIcon(new_item, ItemLight);
  map.insert(name, new_item);
}

void MapTreeWidget::DelLight(WorldLight* light)
{
  DelItem(QString::fromStdString(light->name));
}

void MapTreeWidget::DelObject(MapObject* object)
{
  DelItem(QString::fromStdString(object->nodePath.get_name()));
}

void MapTreeWidget::DelItem(QString name)
{
  auto it =  map.find(name);

  if (it != map.end())
  {
    QTreeWidgetItem* item = *it;

    // Reparenting children to the parent node
    if (item->childCount() > 0)
    {
      MapObject*     object     = world->GetMapObjectFromName(name.toStdString());
      DynamicObject* dyn_object = 0;

      if (!object)
      {
        dyn_object = world->GetDynamicObjectFromName(name.toStdString());
        object     = dyn_object;
      }
      if (object != 0)
      {
        auto parent_widget = map.find(QString::fromStdString(object->parent));

        // Reparenting Objects
        {
          auto it            = world->objects.begin();

          for (; it != world->objects.end() ; ++it)
          {
            if (it->parent == object->name)
            {
              auto widget = map.find(QString::fromStdString(it->name));

              if (widget != map.end())
                ReparentTo(*widget, parent_widget == map.end() ? nullptr : *parent_widget);
              it->parent = object->parent;
            }
          }
        }
        // Reparenting Lights
        {
          auto it = world->lights.begin();

          for (; it != world->lights.end() ; ++it)
          {
            auto widget = map.find(QString::fromStdString(it->name));

            if (widget != map.end())
              ReparentTo(*widget, parent_widget == map.end() ? nullptr : *parent_widget);
            if (parent_widget == map.end())
              it->ReparentTo(world);
          }
        }
      }
    }
    delete item;
    map.erase(it);
  }
}

void MapTreeWidget::ProbeWorld(const std::string& parent_name, QTreeWidgetItem* parent)
{
  ItemType                        type;

  std::function<void(MapObject&)> functor = [this, parent_name, parent, &type](MapObject& object)
  {
    if (!object.nodePath.is_empty() && object.parent == parent_name)
    {
      std::string      name = object.name;
      QTreeWidgetItem* new_item;

      if (parent != 0)
        new_item = new QTreeWidgetItem(parent);
      else
        new_item = new QTreeWidgetItem(this);
      new_item->setText(0, QString::fromStdString(name));
      SetItemIcon(new_item, type);
      InsertPair(QString::fromStdString(name), new_item);
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
        (light.Parent() != 0 && light.Parent()->name == parent_name))
    {
      QTreeWidgetItem* new_item;

      if (parent != 0)
          new_item = new QTreeWidgetItem(parent);
      else
          new_item = new QTreeWidgetItem(this);
      new_item->setText(0, light.name.c_str());
      SetItemIcon(new_item, ItemLight);
      InsertPair(QString::fromStdString(light.name), new_item);
    }
  });
}

void MapTreeWidget::SetWorld(World* world)
{
  this->world = world;
  this->clear();
  map.clear();
  std::for_each(world->objects.begin(), world->objects.end(), [this, world](MapObject& object)
  {
    if ((world->GetMapObjectFromName(object.parent)) == 0)
    {
      object.parent = "";
    }
  });
  ProbeWorld("", 0);
}
