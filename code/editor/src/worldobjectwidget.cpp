#include "worldobjectwidget.h"
#include "ui_worldobjectwidget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QKeySequence>
#include "selectableresource.h"

WorldObjectWidget::WorldObjectWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WorldObjectWidget)
{
  ui->setupUi(this);
  selection.object = 0;
  selection_type   = 0;
  UnsetSelection();

  ui->actionsButton->setMenu(&action_menu);

  connect(ui->objectName, SIGNAL(textChanged(QString)), this, SLOT(UpdateName(QString)));

  // Geometry
  connect(ui->objectPosX,        SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectPosY,        SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectPosZ,        SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectRotationX,   SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectRotationY,   SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectRotationZ,   SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectScaleX,      SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectScaleY,      SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectScaleZ,      SIGNAL(valueChanged(double)), this, SLOT(UpdateGeometry()));
  connect(ui->objectFloor,       SIGNAL(valueChanged(int)),    this, SLOT(UpdateFloor()));
  connect(ui->inheritsFloor,     SIGNAL(toggled(bool)),        this, SLOT(UpdateFloor()));

  // Collider
  connect(ui->displayColliders,  SIGNAL(toggled(bool)),            this, SLOT(UpdateColliderDisplay()));
  connect(ui->collider_type,     SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateColliderType()));
  connect(ui->collider_pos_x,    SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_pos_y,    SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_pos_z,    SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_hpr_x,    SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_hpr_y,    SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_hpr_z,    SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_scale_x,  SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_scale_y,  SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));
  connect(ui->collider_scale_z,  SIGNAL(valueChanged(double)),     this, SLOT(UpdateColliderGeometry()));

  // Light
  connect(ui->lightSetEnabled,   SIGNAL(toggled(bool)),         this, SLOT(LightSetEnabled(bool)));
  connect(ui->lightSetDisabled,  SIGNAL(toggled(bool)),         this, SLOT(LightSetDisabled(bool)));
  connect(ui->lightColorR,       SIGNAL(valueChanged(double)),  this, SLOT(UpdateLightColor()));
  connect(ui->lightColorG,       SIGNAL(valueChanged(double)),  this, SLOT(UpdateLightColor()));
  connect(ui->lightColorB,       SIGNAL(valueChanged(double)),  this, SLOT(UpdateLightColor()));
  connect(ui->lightAttenuationA, SIGNAL(valueChanged(double)),  this, SLOT(UpdateLightAttenuation()));
  connect(ui->lightAttenuationB, SIGNAL(valueChanged(double)),  this, SLOT(UpdateLightAttenuation()));
  connect(ui->lightAttenuationC, SIGNAL(valueChanged(double)),  this, SLOT(UpdateLightAttenuation()));
  connect(ui->lightTypesList,    SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateLightType()));
  connect(ui->lightRadius,       SIGNAL(valueChanged(double)),  this, SLOT(UpdateLightZoneSize()));
  connect(ui->lightCompile,      SIGNAL(clicked()),             this, SLOT(LightCompile()));
  connect(ui->lightPriority,     SIGNAL(valueChanged(int)),     this, SLOT(UpdateLightPriority()));

  // Light -> Shadow caster
  connect(ui->shadowFilmSize,    SIGNAL(valueChanged(int)),     this, SLOT(UpdateShadowCaster()));
  connect(ui->shadowNear,        SIGNAL(valueChanged(int)),     this, SLOT(UpdateShadowCaster()));
  connect(ui->shadowFar,         SIGNAL(valueChanged(int)),     this, SLOT(UpdateShadowCaster()));
  connect(ui->shadowBufferSizeX, SIGNAL(valueChanged(int)),     this, SLOT(UpdateShadowCaster()));
  connect(ui->shadowBufferSizeY, SIGNAL(valueChanged(int)),     this, SLOT(UpdateShadowCaster()));

  // Render
  connect(ui->selectModel,   SIGNAL(clicked()), this, SLOT(PickModel()));
  connect(ui->selectTexture, SIGNAL(clicked()), this, SLOT(PickTexture()));
  connect(ui->objectModel,   SIGNAL(textChanged(QString)), this, SLOT(UpdateRender()));
  connect(ui->objectTexture, SIGNAL(textChanged(QString)), this, SLOT(UpdateRender()));
  connect(ui->objectFocus,   SIGNAL(clicked()), this, SLOT(FocusCurrentObject()));

  // Waypoint
  connect(ui->setCurrentWaypoint, SIGNAL(clicked()), this, SLOT(SetCurrentWaypoint()));
  connect(ui->selectCurrentWaypoint, SIGNAL(clicked()), this, SLOT(SelectCurrentWaypoint()));

  // Behaviour
  connect(ui->objectTypeList,       SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateBehaviour()));
  connect(ui->character,            SIGNAL(textChanged(QString)),     this, SLOT(UpdateBehaviour()));
  connect(ui->dialog,               SIGNAL(textChanged(QString)),     this, SLOT(UpdateBehaviour()));
  connect(ui->script,               SIGNAL(textChanged(QString)),     this, SLOT(UpdateBehaviour()));
  connect(ui->key,                  SIGNAL(textChanged(QString)),     this, SLOT(UpdateBehaviour()));
  connect(ui->interactionUse,       SIGNAL(toggled(bool)),            this, SLOT(UpdateBehaviour()));
  connect(ui->interactionUseSkill,  SIGNAL(toggled(bool)),            this, SLOT(UpdateBehaviour()));
  connect(ui->interactionUseSpell,  SIGNAL(toggled(bool)),            this, SLOT(UpdateBehaviour()));
  connect(ui->interactionUseObject, SIGNAL(toggled(bool)),            this, SLOT(UpdateBehaviour()));
  connect(ui->interactionLookAt,    SIGNAL(toggled(bool)),            this, SLOT(UpdateBehaviour()));
  connect(ui->interactionTalkTo,    SIGNAL(toggled(bool)),            this, SLOT(UpdateBehaviour()));
  connect(ui->selectCharacter,      SIGNAL(clicked()),                this, SLOT(SelectCharacter()));
  connect(ui->selectItem,           SIGNAL(clicked()),                this, SLOT(SelectItem()));
  connect(ui->selectKey,            SIGNAL(clicked()),                this, SLOT(SelectKey()));
  connect(ui->selectScript,         SIGNAL(clicked()),                this, SLOT(SelectScript()));
  connect(ui->selectDialog,         SIGNAL(clicked()),                this, SLOT(SeletDialog()));

  ui->actionsButton->setMenu(&action_menu);
  action_menu.addAction("Copy",  this, SIGNAL(CopyRequested()),  QKeySequence::Copy);
  action_menu.addAction("Paste", this, SIGNAL(PasteRequested()), QKeySequence::Paste);
}

WorldObjectWidget::~WorldObjectWidget()
{
  delete ui;
}

void WorldObjectWidget::LightCompile(void)
{
  if (selection_type == 3)
    world->CompileLight(selection.light, ColMask::Object | ColMask::DynObject);
}

MapObject* WorldObjectWidget::GetSelectedObject(void) const
{
  return (selection_type > 0 && selection_type < 3 ? selection.object : 0);
}

void WorldObjectWidget::SetDialogObject(DialogObject* dialog)
{
  this->dialog_object = dialog;
  connect(ui->objectAdvancedSettings, SIGNAL(clicked()), dialog, SLOT(open()));
}

void WorldObjectWidget::DeleteSelection()
{
  switch (selection_type)
  {
    case 1:
      world->DeleteMapObject(selection.object);
      break ;
    case 2:
      world->DeleteDynamicObject(selection.dynamic_object);
      break ;
    case 3:
      world->DeleteLight(selection.light->name);
      break ;
    default:
      return ;
  }
  selection_type = 0;
  UnsetSelection();
}

void WorldObjectWidget::UnsetSelection()
{
  if (selection_type > 0 && selection_type < 3)
    selection.object->collider.node.hide();
  else if (selection_type == 3)
    selection.light->collider.node.hide();
  selection_type = 0;
  while (ui->tabWidget->count())
    ui->tabWidget->removeTab(0);
  ui->objectName->setText("No objects currently selected");
  ui->objectName->setEnabled(false);
}

void WorldObjectWidget::SetSelection(MapObject* object)
{
  UnsetSelection();
  InitializeMapObject(object);
  selection.object = object;
  selection_type   = 1;
}

void WorldObjectWidget::SetSelection(DynamicObject* object)
{
  UnsetSelection();
  InitializeMapObject(object);
  InitializeDynamicObject(object);
  selection.dynamic_object = object;
  selection_type           = 2;
  dialog_object->SetCurrentObject(object);
}

void WorldObjectWidget::SetSelection(WorldLight* light)
{
  UnsetSelection();
  InitializeGeometry(light->nodePath);
  InitializeCollider(light->collider);

  {
    LColor color = light->GetColor();
    ui->lightColorR->setValue(color.get_x());
    ui->lightColorG->setValue(color.get_y());
    ui->lightColorB->setValue(color.get_z());
  }

  ui->lightTypesList->setCurrentIndex((int)light->type);
  InitializeShadowCaster(light);
  InitializeLightAttenuation(light);
  ui->lightRadius->setValue(light->zoneSize);
  ui->lightPriority->setValue(light->priority);
  ui->objectName->setEnabled(true);
  ui->objectName->setText(QString::fromStdString(light->name));
  selection.light = light;
  selection_type  = 3;
  ui->tabWidget->addTab(ui->lightTab, "Light");
}

void WorldObjectWidget::UpdateLightType(void)
{
  if (selection_type == 3)
  {
    WorldLight::Type type  = (WorldLight::Type)ui->lightTypesList->currentIndex();
    WorldLight*      light = selection.light;

    if (type != light->type)
    {
      cout << "Updating light type" << endl;
      std::string name = light->name;

      light->Destroy();
      light->type = type;
      light->Initialize();
      UpdateLightColor();
      UpdateLightAttenuation();
      InitializeLightAttenuation(light);
      InitializeShadowCaster(light);
    }
    else
      cout << "Light type is already " << (int)type << endl;
  }
}

void WorldObjectWidget::InitializeShadowCaster(WorldLight* light)
{
  if (light->type == WorldLight::Point || light->type == WorldLight::Spot)
  {
    ui->shadowCasting->show();
    ui->shadowFilmSize->setValue(light->shadow_settings.film_size);
    ui->shadowBufferSizeX->setValue(light->shadow_settings.buffer_size[0]);
    ui->shadowBufferSizeX->setValue(light->shadow_settings.buffer_size[1]);
    ui->shadowNear->setValue(light->shadow_settings.distance_near);
    ui->shadowFar->setValue(light->shadow_settings.distance_far);
  }
  else
    ui->shadowCasting->hide();
}

void WorldObjectWidget::UpdateShadowCaster()
{
  if (selection_type == 3)
  {
    WorldLight* light = selection.light;

    light->shadow_settings.film_size      = ui->shadowFilmSize->value();
    light->shadow_settings.buffer_size[0] = ui->shadowBufferSizeX->value();
    light->shadow_settings.buffer_size[1] = ui->shadowBufferSizeX->value();
    light->shadow_settings.distance_near  = ui->shadowNear->value();
    light->shadow_settings.distance_far   = ui->shadowFar->value();
    light->InitializeShadowCaster();
  }
}

void WorldObjectWidget::UpdateLightPriority()
{
  if (selection_type == 3)
    selection.light->priority = ui->lightPriority->value();
}

void WorldObjectWidget::InitializeLightAttenuation(WorldLight* light)
{
  if (light->type == WorldLight::Point || light->type == WorldLight::Spot)
  {
    ui->lightAttenuationA->setValue(light->GetAttenuation().get_x());
    ui->lightAttenuationB->setValue(light->GetAttenuation().get_y());
    ui->lightAttenuationC->setValue(light->GetAttenuation().get_z());
    ui->labelAttenuation->show();
    ui->lightAttenuationA->show();
    ui->lightAttenuationB->show();
    ui->lightAttenuationC->show();
  }
  else
  {
    ui->labelAttenuation->hide();
    ui->lightAttenuationA->hide();
    ui->lightAttenuationB->hide();
    ui->lightAttenuationC->hide();
  }
}

void WorldObjectWidget::UpdateLightAttenuation()
{
  if (selection_type == 3)
  {
      selection.light->SetAttenuation(ui->lightAttenuationA->value(),
                                      ui->lightAttenuationB->value(),
                                      ui->lightAttenuationC->value());
  }
}

void WorldObjectWidget::UpdateLightZoneSize()
{
  if (selection_type == 3)
    selection.light->zoneSize = ui->lightRadius->value();
}

void WorldObjectWidget::UpdateLightColor()
{
  if (selection_type == 3)
  {
    selection.light->SetColor(ui->lightColorR->value(),
                              ui->lightColorG->value(),
                              ui->lightColorB->value(),
                              0.f);
  }
}

void WorldObjectWidget::InitializeMapObject(MapObject* object)
{
  ui->objectName->setEnabled(true);
  ui->objectName->setText(QString::fromStdString(object->name));
  ui->inheritsFloor->setChecked(object->inherits_floor);
  InitializeGeometry(object->nodePath);
  InitializeCollider(object->collider);
  InitializeRender(object);
  ui->objectFloor->setValue(object->floor);
}

void WorldObjectWidget::InitializeDynamicObject(DynamicObject* object)
{
  ui->tabWidget->addTab(ui->behaviourTab, "Behaviour");
  ui->selectCurrentWaypoint->setEnabled(object->waypoint != 0);
  InitializeBehaviour(object);
}

void WorldObjectWidget::InitializeBehaviour(DynamicObject* object)
{
  ui->objectTypeList->setCurrentIndex(object->type);
  ui->character->setText(object->charsheet.c_str());
  ui->item->setText(object->charsheet.c_str());
  ui->script->setText(object->script.c_str());
  ui->dialog->setText(object->dialog.c_str());
  ui->interactionUse->setChecked      (object->interactions & Interactions::Use);
  ui->interactionUseObject->setChecked(object->interactions & Interactions::UseObject);
  ui->interactionUseSkill->setChecked (object->interactions & Interactions::UseSkill);
  ui->interactionUseSpell->setChecked (object->interactions & Interactions::UseSpell);
  ui->interactionTalkTo->setChecked   (object->interactions & Interactions::TalkTo);
  ui->interactionLookAt->setChecked   (object->interactions & Interactions::LookAt);
}

void WorldObjectWidget::UpdateBehaviour()
{
  QList<QWidget*> key_widgets;       key_widgets << ui->key << ui->labelKey << ui->selectKey;
  QList<QWidget*> character_widgets; character_widgets << ui->labelCharacter << ui->character << ui->selectCharacter;
  QList<QWidget*> item_widgets;      item_widgets << ui->labelItem << ui->item << ui->selectItem;
  QList<QWidget*> inventory_widgets; inventory_widgets << ui->labelInventory << ui->inventory << ui->selectInventory;
  auto            show_widgets = [this](QList<QWidget*> widgets, bool show)
  { foreach (QWidget* widget, widgets) { widget->setVisible(show); } };

  show_widgets(key_widgets, false);
  show_widgets(character_widgets, false);
  show_widgets(item_widgets, false);
  show_widgets(inventory_widgets, true);
  switch (ui->objectTypeList->currentIndex())
  {
    case DynamicObject::Door:
      show_widgets(inventory_widgets, false);
    case DynamicObject::Locker:
      show_widgets(key_widgets, true);
    case DynamicObject::Shelf:
      ui->dynamicObject->show();
      break ;
    case DynamicObject::Item:
      show_widgets(item_widgets, true);
      ui->dynamicObject->hide();
      break ;
    case DynamicObject::Character:
      show_widgets(character_widgets, true);
      ui->dynamicObject->hide();
      break ;
  }
  if (selection_type == 2)
  {
    DynamicObject* object = selection.dynamic_object;

    object->type = (DynamicObject::Type)ui->objectTypeList->currentIndex();
    switch (object->type)
    {
      case DynamicObject::Character:
        object->charsheet    = ui->character->text().toStdString();
        object->interactions = 0;
        break ;
      case DynamicObject::Item:
        object->charsheet    = ui->item->text().toStdString();
        object->interactions = Interactions::Use;
        break ;
      default:
        object->key          = ui->key->text().toStdString();
        object->script       = ui->script->text().toStdString();
        object->dialog       = ui->dialog->text().toStdString();
        object->interactions = (ui->interactionUse->isChecked() ? Interactions::Use : 0) +
            (ui->interactionUseObject->isChecked() ? Interactions::UseObject : 0) +
            (ui->interactionUseSkill->isChecked()  ? Interactions::UseSkill  : 0) +
            (ui->interactionUseSpell->isChecked()  ? Interactions::UseSpell  : 0) +
            (ui->interactionTalkTo->isChecked()    ? Interactions::TalkTo    : 0) +
            (ui->interactionLookAt->isChecked()    ? Interactions::LookAt    : 0);
        break ;
    }
  }
}

void WorldObjectWidget::SelectCharacter()
{
  SelectableResource::Charsheets().SelectResource([this](QString name)
  {
    DataTree* data_tree = DataTree::Factory::JSON("data/charsheets/" + name.toStdString() + ".json");

    if (data_tree)
    {
      {
        Data data(data_tree);

        ui->character->setText(name);
        if (data["Appearance"].NotNil())
        {
          ui->objectModel->setText  (data["Appearance"]["model"].Value().c_str());
          ui->objectTexture->setText(data["Appearance"]["texture"].Value().c_str());
        }
      }
      delete data_tree;
    }
  });
}

void WorldObjectWidget::SelectItem()
{
  SelectableResource::Items().SelectResource([this](QString name)
  {
    DataTree* data_tree = DataTree::Factory::JSON("data/objects.json");

    if (data_tree)
    {
      {
        Data data = Data(data_tree)[name.toStdString()];

        if (data.NotNil())
        {
          std::string model   = data["model"].Value();
          std::string texture = data["texture"].Value();

          if (model != "")
            ui->objectModel->setText(model.c_str());
          ui->objectTexture->setText(texture.c_str());
          ui->objectScaleX->setValue(data["scale"].Or(1.f));
          ui->objectScaleY->setValue(data["scale"].Or(1.f));
          ui->objectScaleZ->setValue(data["scale"].Or(1.f));
          ui->item->setText(name);
        }
      }
      delete data_tree;
    }
    // Update Render according to the selected item
  });
}

void WorldObjectWidget::SelectKey()
{
  SelectableResource::Items().SelectResource([this](QString name)
  {
    ui->key->setText(name);
  });
}

void WorldObjectWidget::SelectDialog()
{
  SelectableResource::Dialogs().SelectResource([this](QString name)
  {
    ui->dialog->setText(name);
  });
}

void WorldObjectWidget::SelectScript()
{
  SelectableResource::AIs().SelectResource([this](QString name)
  {
    ui->script->setText(name);
  });
}

void WorldObjectWidget::InitializeRender(MapObject* object)
{
  ui->tabWidget->addTab(ui->renderTab, "Render");
  ui->objectModel->setText(QString::fromStdString(object->strModel));
  ui->objectTexture->setText(QString::fromStdString(object->strTexture));
}

void WorldObjectWidget::InitializeGeometry(NodePath nodePath)
{
  ui->tabWidget->addTab(ui->geometryTab, "Geometry");
  ui->objectPosX->setValue(nodePath.get_x());
  ui->objectPosY->setValue(nodePath.get_y());
  ui->objectPosZ->setValue(nodePath.get_z());
  ui->objectRotationX->setValue(nodePath.get_hpr().get_x());
  ui->objectRotationY->setValue(nodePath.get_hpr().get_y());
  ui->objectRotationZ->setValue(nodePath.get_hpr().get_z());
  ui->objectScaleX->setValue(nodePath.get_scale().get_x());
  ui->objectScaleY->setValue(nodePath.get_scale().get_y());
  ui->objectScaleZ->setValue(nodePath.get_scale().get_z());
}

void WorldObjectWidget::InitializeCollider(Collider& collider)
{
  NodePath node = collider.node;

  ui->tabWidget->addTab(ui->colliderTab, "Collider");
  ui->collider_type->setCurrentIndex((int)collider.type);
  ui->collider_position->setEnabled(collider.type != Collider::NONE);
  if (collider.type != Collider::NONE)
  {
    ui->collider_pos_x->setValue(node.get_x());
    ui->collider_pos_y->setValue(node.get_y());
    ui->collider_pos_z->setValue(node.get_z());
    ui->collider_hpr_x->setValue(node.get_hpr().get_x());
    ui->collider_hpr_y->setValue(node.get_hpr().get_y());
    ui->collider_hpr_z->setValue(node.get_hpr().get_z());
    ui->collider_scale_x->setValue(node.get_scale().get_x());
    ui->collider_scale_y->setValue(node.get_scale().get_y());
    ui->collider_scale_z->setValue(node.get_scale().get_z());
    if (ui->displayColliders->isChecked())
      node.show();
  }
}

void WorldObjectWidget::UpdateName(QString name)
{
  if (selection_type != 0)
  {
    if (selection_type < 3)
    {
      MapObject* object = selection.object;

      RenameObject(QString::fromStdString(object->name), name);
      object->name = name.toStdString();
      object->nodePath.set_name(object->name);
    }
    else if (selection_type == 3)
    {
      RenameObject(QString::fromStdString(selection.light->name), name);
      selection.light->name = name.toStdString();
    }
  }
}

void WorldObjectWidget::UpdateColliderGeometry()
{
  Collider* collider = 0;

  if (selection_type > 0 && selection_type < 3)
    collider = &selection.object->collider;
  else if (selection_type == 3)
    collider = &selection.light->collider;
  if (collider)
  {
    LPoint3f   position(ui->collider_pos_x->value(), ui->collider_pos_y->value(), ui->collider_pos_z->value());
    LPoint3f   hpr     (ui->collider_hpr_x->value(), ui->collider_hpr_y->value(), ui->collider_hpr_z->value());
    LPoint3f   scale   (ui->collider_scale_x->value(), ui->collider_scale_y->value(), ui->collider_scale_z->value());

    collider->node.set_pos(position);
    collider->node.set_hpr(hpr);
    collider->node.set_scale(scale);
    ui->displayColliders->setChecked(true);
  }
  UpdateColliderDisplay();
}

void WorldObjectWidget::UpdateColliderType()
{
  Collider* collider = 0;
  NodePath  parent;

  if (selection_type > 0 && selection_type < 3)
  {
    collider = &selection.object->collider;
    parent   = selection.object->nodePath;
  }
  else if (selection_type == 3)
  {
    collider = &selection.light->collider;
    parent   = selection.light->symbol;
  }
  if (collider)
  {
    if (collider->type != Collider::NONE)
      collider->node.remove_node();
    collider->type = (Collider::Type)ui->collider_type->currentIndex();
    ui->collider_position->setEnabled(collider->type != Collider::NONE);
    collider->parent = parent;
    collider->InitializeCollider(collider->type, LPoint3f(0, 0, 0), LPoint3f(1, 1, 1), LPoint3f(0, 0, 0));
    if (selection_type > 0 && selection_type < 3)
    {
      LPoint3f scale = NodePathSize(selection.object->render) / 2;
      LPoint3f min_point, max_point;

      selection.object->render.calc_tight_bounds(min_point, max_point);
      ui->collider_pos_x->setValue((std::abs(max_point.get_x()) - std::abs(min_point.get_x())) / 2 * (max_point.get_x() < 0 ? -1 : 1));
      ui->collider_pos_y->setValue((std::abs(max_point.get_y()) - std::abs(min_point.get_y())) / 2 * (max_point.get_y() < 0 ? -1 : 1));
      ui->collider_pos_z->setValue((std::abs(max_point.get_z()) - std::abs(min_point.get_z())) / 2 * (max_point.get_z() < 0 ? -1 : 1));
      ui->collider_scale_x->setValue(scale.get_x());
      ui->collider_scale_y->setValue(scale.get_y());
      ui->collider_scale_z->setValue(scale.get_z());
    }
    UpdateColliderGeometry();
  }
}

void WorldObjectWidget::UpdateColliderDisplay()
{
  Collider* collider = 0;

  if (selection_type > 0 && selection_type < 3)
    collider = &selection.object->collider;
  else if (selection_type == 3)
    collider = &selection.light->collider;
  if (collider)
  {
    if (ui->displayColliders->isChecked())
      collider->node.show();
    else
      collider->node.hide();
  }
}

void WorldObjectWidget::UpdateGeometry()
{
  NodePath nodePath;

  if (selection_type > 0 && selection_type < 3)
    nodePath = selection.object->nodePath;
  else if (selection_type == 3)
    nodePath = selection.light->nodePath;
  if (!(nodePath.is_empty()))
  {
    nodePath.set_x(ui->objectPosX->value());
    nodePath.set_y(ui->objectPosY->value());
    nodePath.set_z(ui->objectPosZ->value());
    nodePath.set_hpr(ui->objectRotationX->value(),
                     ui->objectRotationY->value(),
                     ui->objectRotationZ->value());
    nodePath.set_scale(ui->objectScaleX->value(),
                       ui->objectScaleY->value(),
                       ui->objectScaleZ->value());
  }
}

void WorldObjectWidget::UpdateFloor()
{
  if (selection_type > 0 && selection_type < 3)
  {
    selection.object->inherits_floor = ui->inheritsFloor->isChecked();
    selection.object->floor          = ui->objectFloor->value();
  }
  ui->objectFloor->setEnabled(!ui->inheritsFloor->isChecked());
}

void WorldObjectWidget::LightSetDisabled(bool disabled)
{
  if (disabled)
    ui->lightSetEnabled->setChecked(false);
}

void WorldObjectWidget::LightSetEnabled(bool enabled)
{
  if (enabled)
    ui->lightSetDisabled->setChecked(false);
  selection.light->SetEnabled(enabled);
}

void WorldObjectWidget::PickModel()
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
  ui->objectModel->setText(relative_path);
}

void WorldObjectWidget::PickTexture()
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
  ui->objectTexture->setText(relative_path);
}

void WorldObjectWidget::UpdateRender()
{
  if (selection_type > 0 && selection_type < 3)
  {
    MapObject*  object      = selection.object;
    std::string new_model   = ui->objectModel->text().toStdString();
    std::string new_texture = ui->objectTexture->text().toStdString();

    if (object->strModel != new_model)
      selection.object->SetModel(new_model);
    if (object->strTexture != new_texture)
      selection.object->SetTexture(new_texture);
  }
}

void WorldObjectWidget::FocusCurrentObject()
{
  if (selection_type > 0 && selection_type < 3)
  {
    NodePath selected_nodepath = selection.object->nodePath;
    auto     functor = [this, selected_nodepath](MapObject& object)
    {
      NodePath nodePath = object.nodePath;

      if (nodePath != selected_nodepath &&
          !(selected_nodepath.is_ancestor_of(nodePath)))
        object.render.hide();
      else
        object.render.show();
    };

    for_each(world->objects.begin(), world->objects.end(), functor);
    for_each(world->dynamicObjects.begin(), world->dynamicObjects.end(), functor);
  }
}

void WorldObjectWidget::SetCurrentWaypoint()
{
  if (selection_type == 2)
  {
    DynamicObject* object = selection.dynamic_object;

    WaypointSetOnObjectRequested(object);
    ui->selectCurrentWaypoint->setEnabled(object->waypoint != 0);
  }
}

void WorldObjectWidget::SelectCurrentWaypoint()
{
  if (selection_type == 2)
    SelectWaypointFromObject(selection.dynamic_object);
}
