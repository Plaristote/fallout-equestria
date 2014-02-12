#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include "globals.hpp"
#include <QTreeWidget>
#include <QMap>

struct World;
struct MapObject;
struct DynamicObject;
struct WorldLight;

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
    void             AddMapObject(MapObject*);
    void             AddDynamicObject(DynamicObject*);
    void             AddWorldLight(WorldLight*);
    void             DelObject(MapObject*);
    void             DelLight(WorldLight*);
    void             DelItem(QString);
    void             SetItemFocused(MapObject*);
    void             Rename(QString old_name, QString new_name);

protected:
    virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);

private:
    World* world;
    QIcon icon_map_object, icon_dyn_object, icon_light;

    void             ProbeWorld(const std::string& parent_name, QTreeWidgetItem* item);
    void             SetItemIcon(QTreeWidgetItem*, ItemType t);
    void             ReparentTo(QTreeWidgetItem* to_reparent, QTreeWidgetItem* parent);
    void             InsertPair(const QString& key, QTreeWidgetItem* value)
    {
      map.insert(key, value);
    }

public:
    QMap<QString, QTreeWidgetItem*> map;
};

#endif // MAPTREEWIDGET_H
