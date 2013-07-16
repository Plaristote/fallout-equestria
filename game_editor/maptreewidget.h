#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include "globals.hpp"
#include <QTreeWidget>

class World;
class MapObject;
class DynamicObject;
class WorldLight;

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    enum ItemType
    {
      ItemMapObject,
      ItemDynamicObject,
      ItemLight,
      ItemUnknown
    };

    explicit MapTreeWidget(QWidget *parent = 0);

    void             SetWorld(World* world);
    ItemType         GetType(QTreeWidgetItem*) const;

signals:
    void             FocusObject(MapObject*);
    void             FocusDynamicObject(DynamicObject*);
    void             FocusLight(WorldLight*);
    
public slots:
    void             ItemFocused();
protected:
    virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);

private:
    World* world;
    QIcon icon_map_object, icon_dyn_object, icon_light;

    void             ProbeWorld(const std::string& parent_name, QTreeWidgetItem* item);
    void             SetItemIcon(QTreeWidgetItem*, ItemType t);
};

#endif // MAPTREEWIDGET_H
