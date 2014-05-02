#ifndef WORLDOBJECTWIDGET_H
# define WORLDOBJECTWIDGET_H

# include "globals.hpp"
# include <QWidget>
# include <QMenu>
# include "world/world.h"
# include "dialogobject.h"

namespace Ui {
  class WorldObjectWidget;
}

class WorldObjectWidget : public QWidget
{
  Q_OBJECT

  union Selection
  {
    MapObject*     object;
    DynamicObject* dynamic_object;
    WorldLight*    light;
  };

public:
  explicit WorldObjectWidget(QWidget *parent = 0);
  ~WorldObjectWidget();

  void SetWorld(World* world) { this->world = world; }
  void SetDialogObject(DialogObject* dialog_object);

  void SetSelection(MapObject*);
  void SetSelection(DynamicObject*);
  void SetSelection(WorldLight*);

  MapObject* GetSelectedObject(void) const;

public slots:
  void UnsetSelection();
  void DeleteSelection();

  void UpdateName(QString);
  void UpdateFloor();
  void UpdateGeometry();
  void UpdateRender();
  void PickModel();
  void PickTexture();

  void UpdateColliderType();
  void UpdateColliderGeometry();
  void UpdateColliderDisplay();

  void LightCompile();
  void LightSetEnabled(bool);
  void LightSetDisabled(bool);
  void UpdateLightType();
  void UpdateLightAttenuation();
  void UpdateLightColor();
  void UpdateLightZoneSize();
  void UpdateLightPriority();
  void UpdateShadowCaster();

  void FocusCurrentObject();

  void SelectCurrentWaypoint();
  void SetCurrentWaypoint();

signals:
  void RenameObject(QString old_name, QString new_name);
  void CopyRequested();
  void PasteRequested();
  void WaypointSetOnObjectRequested(DynamicObject*);
  void SelectWaypointFromObject(DynamicObject*);

private:
  void InitializeMapObject(MapObject*);
  void InitializeDynamicObject(DynamicObject*);
  void InitializeShadowCaster(WorldLight*);
  void InitializeLightAttenuation(WorldLight*);
  void InitializeGeometry(NodePath);
  void InitializeCollider(MapObject*);
  void InitializeRender(MapObject*);

  Ui::WorldObjectWidget *ui;
  QMenu                  action_menu;
  World*                 world;
  DialogObject*          dialog_object;
  Selection              selection;
  char                   selection_type;
};

#endif // WORLDOBJECTWIDGET_H
