#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpandaapplication.h"
#include <QInputDialog>
#include <panda3d/collisionRay.h>
#include "world/scene_camera.hpp"
#include "mouse.h"
#include <QElapsedTimer>
#include "functorthread.h"
#include "selectableresource.h"

#define PANDA_TAB 1

QString objectTypes[] = { "NPC", "Shelf", "Locker", "Door" };
QString strScriptCategories[N_SCRIPT_CAT] = {
    "Artificial Intelligence", "Buffs", "Dialogues", "Levels", "Objects", "Pipbuck Apps", "Quests"
};
QString pathScriptCategories[N_SCRIPT_CAT] = {
    "ai", "buffs", "dialogs", "level", "objects", "pipbuck", "quests"
};

struct PandaTask : public AsyncTask
{
  MainWindow*      main_window;
  QWidget*         panda_widget;
  WindowFramework* window;
  SceneCamera*     camera;
  Mouse*           mouse;
  QElapsedTimer    timer;
  bool             camera_locked;

  PandaTask() : window(0), camera(0), mouse(0), camera_locked(false) { }

  DoneStatus do_task(void)
  {
    float elapsedTime = timer.elapsed();

    elapsedTime /= 1000;
    camera_locked = camera_locked || !(dynamic_cast<MouseWatcher*>(window->get_mouse().node())->has_mouse());
    camera->SetCameraLocked(camera_locked);
    camera->Run(elapsedTime);
    mouse->Run();
    timer.start();
    //main_window->ShowWaypointZone();
    return (AsyncTask::DS_cont);
  }

  LPoint3 CollidingAt(MapObject* object)
  {
    NodePath                  camera                  = window->get_camera_group();
    PT(CollisionRay)          picker_ray              = new CollisionRay();
    PT(CollisionNode)         picker_node             = new CollisionNode("mouse_ray");
    NodePath                  picker_path             = camera.attach_new_node(picker_node);
    CollisionTraverser        traverser;
    PT(CollisionHandlerQueue) collision_handler_queue = new CollisionHandlerQueue();
    LPoint2f                  cursor_pos              = mouse->GetPositionRatio();

    picker_ray->set_from_lens(window->get_camera(0), cursor_pos.get_x(), cursor_pos.get_y());
    picker_node->add_solid(picker_ray);
    traverser.add_collider(picker_path, collision_handler_queue);
    traverser.traverse(object->nodePath);
    collision_handler_queue->sort_entries();
    for (int i = 0 ; i < collision_handler_queue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry = collision_handler_queue->get_entry(i);
      NodePath        np    = entry->get_into_node_path();
      LPoint3         pos;

      if (object->nodePath.is_ancestor_of(np))
        return (entry->get_surface_point(object->nodePath));
    }
    return (LPoint3(0, 0, 0));
  }
};

PandaTask my_task;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), tabScript(this, ui), tabDialog(this, ui), tabL18n(this, ui), splashScreen(this), wizardObject(this), dialogObject(this)
{
    QIcon iconCharacter("icons/character.png");
    QIcon iconScript("icons/script.png");
    QIcon iconItems("icons/item.png");
    QIcon iconDialogs("icons/dialogs.png");
    QIcon iconLevel("icons/level.png");
    QIcon iconWorldmap("icons/worldmap.png");
    QIcon iconDelete("icons/delete.png");
    QIcon iconSave("icons/save.png");
    QIcon iconAdd("icons/add.png");
    QIcon iconUp("icons/up.png");
    QIcon iconDown("icons/down.png");
    QIcon iconLeft("icons/previous.png");
    QIcon iconRight("icons/next.png");
    QIcon iconClose("icons/close.png");
    QIcon iconLanguage("icons/internationalization.png");
    QIcon iconEdit("icons/edit.png");
    QIcon iconConnect("icons/connect.png");
    QIcon iconDisconnect("icons/disconnect.png");
    QIcon waypointGenerate("icons/waypoint-generate.png");

    clipboard         = 0;
    level_editor_lock = 0;
    world             = 0;
    objectFile        = 0;
    ui->setupUi(this);

    ui->charsheetAdd->setIcon(iconAdd);
    ui->charsheetDel->setIcon(iconDelete);
    ui->charsheetSave->setIcon(iconSave);

    ui->waypointConnect->setIcon(iconConnect);
    ui->waypointDisconnect->setIcon(iconDisconnect);
    ui->waypointSelDelete->setIcon(iconDelete);
    waypointSelX = waypointSelY = waypointSelZ = 0;

    ui->objectAddButton->setIcon(iconAdd);
    ui->objectDeleteButton->setIcon(iconDelete);

    ui->tabWidget->setTabIcon(0, iconLevel);
    ui->tabWidget->setTabIcon(1, iconWorldmap);
    ui->tabWidget->setTabIcon(2, iconCharacter);
    ui->tabWidget->setTabIcon(3, iconScript);
    ui->tabWidget->setTabIcon(4, iconItems);
    ui->tabWidget->setTabIcon(5, iconDialogs);
    ui->tabWidget->setTabIcon(6, iconLanguage);
    ui->scriptNew->setIcon(iconAdd);
    ui->dialogNew->setIcon(iconAdd);
    ui->mapNew->setIcon(iconAdd);
    ui->scriptDelete->setIcon(iconDelete);
    ui->dialogDelete->setIcon(iconDelete);
    ui->scriptNext->setIcon(iconRight);
    ui->scriptPrevious->setIcon(iconLeft);
    ui->scriptSave->setIcon(iconSave);
    ui->scriptClose->setIcon(iconClose);
    ui->languageNew->setIcon(iconAdd);
    ui->languageDelete->setIcon(iconDelete);
    ui->saveMap->setIcon(iconSave);

    ui->progressBar->hide();

    setWindowTitle("Fallout Equestria Editor");
    setWindowIcon(QIcon("icons/app-icon.png"));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(CurrentTabChanged(int)));

    /*connect(&splashScreen, SIGNAL(rejected()), app,  SLOT(Terminate()));
    connect(&splashScreen, SIGNAL(rejected()), app,  SLOT(quit()));*/
    connect(&splashScreen, SIGNAL(accepted()), this, SLOT(LoadProject()));

    connect(ui->widget, SIGNAL(Initialized()), this, SLOT(PandaInitialized()));
    //connect(this,       SIGNAL(Closed()),      app,  SLOT(Terminate()));

    for (short i = 0 ; i < N_SCRIPT_CAT ; ++i)
    {
        tabScript.scriptCategories[i].setText(0, strScriptCategories[i]);
        ui->scriptList->addTopLevelItem(&(tabScript.scriptCategories[i]));
    }

    connect(ui->scriptList,   SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), &tabScript, SLOT(SwapScript(QTreeWidgetItem*)));
    connect(ui->scriptList,   SIGNAL(itemClicked(QTreeWidgetItem*,int)), &tabScript, SLOT(SwapScript(QTreeWidgetItem*)));
    connect(ui->scriptNew,    SIGNAL(clicked()),  &tabScript.dialogNewScript, SLOT(open()));
    connect(&tabScript.dialogNewScript, SIGNAL(accepted()), &tabScript, SLOT(NewScript()));

    connect(ui->scriptClose,    SIGNAL(clicked()), &tabScript, SLOT(CloseScript()));
    connect(ui->scriptSave,     SIGNAL(clicked()), &tabScript, SLOT(SaveScript()));
    connect(ui->scriptPrevious, SIGNAL(clicked()), &tabScript, SLOT(PreviousScript()));
    connect(ui->scriptNext,     SIGNAL(clicked()), &tabScript, SLOT(NextScript()));
    connect(ui->scriptSearch,   SIGNAL(textChanged(QString)), &tabScript, SLOT(FilterScript(QString)));
    connect(ui->scriptDelete,   SIGNAL(clicked()), &tabScript, SLOT(RemoveScript()));
    connect(ui->scriptSearch,   SIGNAL(cursorPositionChanged(int,int)), this, SLOT(FilterInit()));

    connect(&tabDialog, SIGNAL(RequestLocale()),                this,       SLOT(SetDefaultLocalization()));
    connect(&tabDialog, SIGNAL(RequestScript(QString,QRegExp)), &tabScript, SLOT(GoTo(QString,QRegExp)));

    connect(ui->dialogDelete, SIGNAL(clicked()), &tabDialog, SLOT(RemoveDialog()));
    connect(ui->dialogNew,    SIGNAL(clicked()), &tabDialog, SLOT(NewDialog()));
    connect(ui->dialogList,   SIGNAL(currentIndexChanged(QString)), &tabDialog, SLOT(SwapDialog(QString)));
	//connect(ui->dialogEditor, SIGNAL(UpdateLocale()), &tabL18n, SLOT(UpdateLocale()));
	connect(ui->dialogList,   SIGNAL(currentIndexChanged(QString)), &tabDialog, SLOT(SwapDialog(QString)));
    //connect(ui->dialogSearch, SIGNAL(textChanged(QString)), &tabDialog, SLOT(FilterDialog(QString)));

    connect(ui->languageDelete, SIGNAL(clicked()), &tabL18n, SLOT(RemoveLanguage()));
    connect(ui->languageNew, SIGNAL(clicked()), &tabL18n, SLOT(NewLanguage()));
    connect(ui->languageList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), &tabL18n, SLOT(SwapLanguage(QListWidgetItem*)));

    connect(ui->listMap, SIGNAL(currentIndexChanged(QString)), this, SLOT(LoadMap(QString)));
    connect(ui->saveMap, SIGNAL(clicked()),                    &dialogSaveMap, SLOT(open()));
    connect(&dialogSaveMap, SIGNAL(accepted()),                this, SLOT(SaveMap()));
    connect(ui->mapNew,  SIGNAL(clicked()),                    &dialogNewMap, SLOT(open()));

    connect(&dialogNewMap, SIGNAL(CreateMap()), this, SLOT(CreateMap()));

    connect(this, SIGNAL(SigDisplayError(QString,QString)),     this, SLOT(DisplayError(QString,QString)),     Qt::QueuedConnection);
    connect(this, SIGNAL(SigUpdateProgressBar(QString, float)), this, SLOT(UpdateProgressBar(QString, float)), Qt::QueuedConnection);
    save_map_use_thread = true;

    connect(&this->waypointGenerate, SIGNAL(Generate()), this, SLOT(MapObjectGenerateWaypoints()));
    connect(ui->waypointGenerateObject, SIGNAL(clicked()), &this->waypointGenerate, SLOT(show()));

    connect(ui->characterSearch, SIGNAL(textChanged(QString)), this, SLOT(FilterCharacters(QString)));
    connect(ui->charsheetList, SIGNAL(currentTextChanged(QString)), ui->charsheetEditor, SLOT(Load(QString)));
    connect(ui->charsheetAdd,  SIGNAL(clicked()),                   this,                SLOT(AddCharsheet()));
    connect(ui->charsheetDel,  SIGNAL(clicked()),                   this,                SLOT(DeleteCharsheet()));
    connect(ui->charsheetSave, SIGNAL(clicked()),                   ui->charsheetEditor, SLOT(Save()));

    connect(ui->treeWidget, SIGNAL(FocusObject(MapObject*)),            this, SLOT(MapObjectFocus(MapObject*)));
    connect(ui->treeWidget, SIGNAL(FocusDynamicObject(DynamicObject*)), this, SLOT(DynamicObjectFocus(DynamicObject*)));
    connect(ui->treeWidget, SIGNAL(FocusLight(WorldLight*)),            this, SLOT(LightFocus(WorldLight*)));

    connect(ui->freeCamera, SIGNAL(toggled(bool)), this, SLOT(SetFreeCamera(bool)));
    connect(ui->lockCamera, SIGNAL(toggled(bool)), this, SLOT(SetCameraLocked(bool)));

    connect(ui->worldObjectWidget, SIGNAL(CopyRequested()),  this, SLOT(CopyClicked()));
    connect(ui->worldObjectWidget, SIGNAL(PasteRequested()), this, SLOT(PasteClicked()));
    connect(ui->worldObjectWidget, SIGNAL(SelectWaypointFromObject(DynamicObject*)), this, SLOT(SelectWaypointFromObject(DynamicObject*)));
    connect(ui->worldObjectWidget, SIGNAL(WaypointSetOnObjectRequested(DynamicObject*)), this, SLOT(DynamicObjectSetWaypoint(DynamicObject*)));

    ui->treeWidget->header()->hide();
    ui->scriptList->header()->hide();

    connect(ui->waypointPicker,       SIGNAL(clicked(bool)),                               SLOT(TerrainPickerPicked(bool)));
    connect(ui->waypointZoneSelector, SIGNAL(clicked(bool)),                               SLOT(TerrainSelectorPicked(bool)));
    connect(ui->itemEditor,           SIGNAL(ItemListChanged(QStringList)), &dialogObject, SLOT(SetObjectList(QStringList)));

    connect(ui->dialogList, SIGNAL(currentIndexChanged(QString)), &tabDialog, SLOT(LoadDialog(QString)));

    ui->worldObjectWidget->SetDialogObject(&dialogObject);
    connect(ui->worldObjectWidget, SIGNAL(RenameObject(QString,QString)), ui->treeWidget, SLOT(Rename(QString,QString)));
}

MainWindow::~MainWindow()
{
    AsyncTaskManager::get_global_ptr()->remove(&my_task);
    delete ui;
}

void MainWindow::FilterCharacters(QString string)
{
  QRegExp regexp(string);

  for (short i = 0 ; i < ui->charsheetList->count() ; ++i)
  {
    QListWidgetItem* item = ui->charsheetList->item(i);

    item->setHidden(!(string == "" || item->text().contains(regexp)));
  }
}

void MainWindow::TerrainPickerPicked(bool set)
{
  ui->waypointZoneSelector->setChecked(!set);
}

void MainWindow::TerrainSelectorPicked(bool set)
{
  ui->waypointPicker->setChecked(!set);
}

void MainWindow::SetFreeCamera(bool value)
{
  if (my_task.camera != 0)
    my_task.camera->SetEnabledTrackball(value);
}

void MainWindow::SetCameraLocked(bool value)
{
  my_task.camera_locked = value;
}

void MainWindow::CurrentTabChanged(int ntab)
{
    //_app.SetPandaEnabled(ntab == PANDA_TAB); // The first tab is the only one using Panda3D
}

void MainWindow::CopyClicked()
{
  if (clipboard)
    delete clipboard;
  clipboard = new Utils::Packet;
  Copy(*clipboard);
}

void MainWindow::PasteClicked()
{
  if (clipboard)
  {
    Utils::Packet dup(clipboard->raw(), clipboard->size());

    Paste(dup);
  }
}

void MainWindow::Copy(Utils::Packet& packet)
{
  if (mapobjectSelected)
  {
    std::function<void (std::list<MapObject*>&, const std::string&)> get_children;
    std::list<MapObject*>                                            objects;

    objects.push_back(mapobjectSelected);
    // Recursively look for each children of the current object and
    // duplicate them as well.
    get_children = [this, &get_children](std::list<MapObject*>& objects, const std::string& parent)
    {
      auto                  it  = world->objects.begin();
      auto                  end = world->objects.end();

      for (; it != end ; ++it)
      {
         if (it->parent == parent)
         {
           objects.push_back(&(*it));
           get_children(objects, it->nodePath.get_name());
         }
      }
    };
    get_children(objects, mapobjectSelected->nodePath.get_name());
    // Now we record the amount of objects serialized, and we serialize them.
    // The waypoints must not be part of the copy/paste, thus we temporarily remove them
    // from the duplicated object when serializing.
    packet << (int)objects.size();
    std::for_each(objects.begin(), objects.end(), [&packet](MapObject* object)
    {
      MapObject::Waypoints waypoints = object->waypoints;

      object->waypoints.clear();
      object->Serialize(packet);
      object->waypoints = waypoints;
    });
  }
}

void MainWindow::Paste(Utils::Packet& packet)
{
   int                   object_count;
   QMap<QString,QString> name_map;

   World::LoadingWorld = world; // Sets a context for unserialized objects to plug into
   packet >> object_count;
   for (int i = 0 ; i < object_count ; ++i)
   {
       MapObject object;

       packet >> object;
       object.nodePath.set_collide_mask(CollideMask(ColMask::Object));

       QString old_name  = QString::fromStdString(object.nodePath.get_name());
       QString base_name = old_name;
       QString name;
       unsigned short ii = 0;

       // remove_number_from_name
       {
         QRegExp regexp("#[0-9]+$");

         if (base_name.contains(regexp))
           base_name = base_name.replace(regexp, "");
       }

       // Change the object's name,
       // Change the object's parent name if relevant
       do
       {
         ++ii;
         name = base_name + '#' + QString::number(ii);
       } while ((world->GetMapObjectFromName(name.toStdString()) != 0) ||
                (world->GetDynamicObjectFromName(name.toStdString()) != 0) ||
                (world->GetLightByName(name.toStdString()) != 0));
       object.name = name.toStdString();
       object.nodePath.set_name(object.name);
       name_map.insert(old_name, name);

       {
         auto parent_it = name_map.find(QString::fromStdString(object.parent));

         if (parent_it != name_map.end())
           object.parent = parent_it->toStdString();
         else
         {
           MapObject* selection = ui->worldObjectWidget->GetSelectedObject();

           if (selection)
             object.parent = selection->name;
         }
       }

       world->objects.push_back(object);
   }
   world->UpdateMapTree();
   ui->treeWidget->SetWorld(world);
   World::LoadingWorld = 0;
}

void MainWindow::AddCharsheet()
{
  QString name = QInputDialog::getText(this, "New statistic sheet", "Name");

  if (name != "")
  {
    ui->charsheetEditor->New(name);
    ui->charsheetList->addItem(name);
    ui->charsheetDel->setEnabled(true);
  }
}

void MainWindow::DeleteCharsheet()
{
  QListWidgetItem* item = ui->charsheetList->currentItem();

  if (item)
  {
    int ret = QMessageBox::warning(this, "Deleting a stat sheet", "Are you sure you want to delete " + item->text() + " ?", QMessageBox::Yes, QMessageBox::No);

    if (ret == QMessageBox::No)
      return ;
    ui->charsheetEditor->Delete();
    ui->charsheetList->removeItemWidget(item);
    delete item;
  }
  if (ui->charsheetList->count() == 0)
    ui->charsheetDel->setEnabled(false);
}

void MainWindow::CreateMap(void)
{
    QString name = dialogNewMap.GetMapName();

    if (world)
      delete world;
    world               = new World(_window);
    levelName           = name;
    save_map_use_thread = false;
    SaveMap();
    save_map_use_thread = true;
    ui->listMap->addItem(name);
    SelectableResource::MapsResource().AddResource(name);
    LoadMap(name);
    LevelListUpdated();
}

void MainWindow::LevelListUpdated()
{
  QStringList level_list;

  for (unsigned short i = 0 ; i < ui->listMap->count() ; ++i)
    level_list << ui->listMap->itemText(i);
  tabScript.dialogNewScript.UpdateLevelList(level_list);
}

void MainWindow::LoadProject()
{
    bool    success = true;
    QString path    = splashScreen.GetProjectPath();
    QDir    root(path);

    if (!(QDir::setCurrent(path)))
    {
        QMessageBox::warning(this, "Error", "Couldn't set the path properly");
        success = false;
    }
    else // Check Existing paths
    {
        QStringList pathToCheck;
        bool        creatingPath = false;

        pathToCheck << "maps/" << "data/" << "scripts/" << "data/i18n" << "data/dialogs";
        for (short i = 0 ; i < 3 ; ++i)
          pathToCheck << ("scripts/" + pathScriptCategories[i]);
        foreach (QString path, pathToCheck)
        {
            QDir dir(path);

            if (!(dir.exists()))
            {
                if (creatingPath == false)
                {
                    int ret = QMessageBox::warning(this, "Missing directory " + path, "This project doesn't exist yet. Do you want to create it ?", QMessageBox::Yes, QMessageBox::No);

                    if (ret == QMessageBox::No)
                    {
                      success = false;
                      break ;
                    }
                    else
                      creatingPath = true;
                }
                if (creatingPath)
                    root.mkdir(path);
            }
        }
    }

    if (success)
    {
        tabScript.LoadAllScript();
		tabL18n.LoadAllLanguages();
        tabDialog.LoadAllDialogs();
        ui->itemEditor->LoadAllItems();
        ui->worldmapEditor->Load();
        LoadAllMaps();
        LoadAllStatsheets();
    }
    else
        splashScreen.open();
    LevelListUpdated();
}

void MainWindow::LoadAllStatsheets()
{
    QDir        dir("data/charsheets");
    QStringList fileList = dir.entryList();
    QRegExp     regexp("\\.json$");

    foreach (QString string, fileList)
    {
      if ((string.contains(regexp)))
      {
        QString name = string.replace(regexp, "");

        ui->charsheetList->addItem(name);
        SelectableResource::Charsheets().AddResource(name);
      }
    }
    if (ui->charsheetList->count() == 0)
      ui->charsheetDel->setEnabled(false);
}

void MainWindow::LoadAllMaps()
{
  QDir        dir("maps/");
  QStringList fileList = dir.entryList();
  QRegExp     regexp("\\.blob$");

  foreach (QString string, fileList)
  {
    if (string.contains(regexp))
    {
      QString name = string.replace(regexp, "");

      ui->listMap->addItem(name);
      SelectableResource::MapsResource().AddResource(name);
    }
  }
}

void MainWindow::FilterInit()
{
    if (ui->scriptSearch->text() == "Search...")
        ui->scriptSearch->setText("");
    //if (ui->dialogSearch->text() == "Search...")
    //    ui->dialogSearch->setText("");
}

void MainWindow::CameraMoveBottom()
{
    my_task.camera->MoveV(-30.f);
}

void MainWindow::CameraMoveTop()
{
    my_task.camera->MoveV(30.f);
}

void MainWindow::CameraMoveLeft()
{
    my_task.camera->MoveH(-30.f);
}

void MainWindow::CameraMoveRight()
{
    my_task.camera->MoveH(30.f);
}

void MainWindow::PandaButtonPressed(QMouseEvent*)
{
  if (ui->waypointZoneSelector->isChecked() && mapobjectSelected)
  {
    LPoint3 pos = my_task.CollidingAt(mapobjectSelected);

    wp_select_x = pos.get_x();
    wp_select_y = pos.get_y();
  }
}

void MainWindow::SelectWaypointZone(float from_x, float from_y, float to_x, float to_y)
{
  if (mapobjectSelected)
  {
    auto it  = mapobjectSelected->waypoints.begin();
    auto end = mapobjectSelected->waypoints.end();

    for (; it != end ; ++it)
    {
      LPoint3 pos = (*it)->nodePath.get_pos();

      if ((!(*it)->IsSelected()) &&
          (pos.get_x() >= from_x && pos.get_x() <= to_x) &&
          (pos.get_y() >= from_y && pos.get_y() <= to_y))
      {
        (*it)->SetSelected(true);
        waypointsSelection.push_back(*it);
      }
    }
  }
}

void MainWindow::ShowWaypointZone(void)
{
  if (mapobjectSelected && !(mapobjectSelected->waypoints_root.is_hidden()))
  {
    static LPoint2f last_position    = LPoint2f(0, 0);
    static bool     needs_update     = true;
    LPoint2f        current_position = my_task.mouse->GetPosition();

    if (last_position != current_position)
      needs_update = true;
    if (needs_update == false || last_position != current_position)
    {
      last_position = current_position;
      return ;
    }
    last_position = current_position;

    LPoint3                   position       = my_task.CollidingAt(mapobjectSelected);
    PT(CollisionSphere)       collision_box  = new CollisionSphere(position, 50);
    PT(CollisionNode)         collision_node = new CollisionNode("waypoint_box");
    NodePath                  collision_path = mapobjectSelected->nodePath.attach_new_node(collision_node);
    CollisionTraverser        traverser;
    PT(CollisionHandlerQueue) collision_handler_queue = new CollisionHandlerQueue();

    collision_node->set_from_collide_mask(CollideMask(ColMask::Waypoint));
    collision_node->add_solid(collision_box);
    traverser.add_collider(collision_path, collision_handler_queue);
    traverser.traverse(mapobjectSelected->nodePath);
    std::for_each(mapobjectSelected->waypoints.begin(), mapobjectSelected->waypoints.end(), [](Waypoint* wp)
    {
      if (wp->IsSelected())
        wp->nodePath.show();
      else
        wp->nodePath.hide();
    });
    for (int i = 0 ; i < collision_handler_queue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry  = collision_handler_queue->get_entry(i);
      NodePath        into   = entry->get_into_node_path();
      NodePath        parent = into.get_parent();

      while (!(parent.is_empty()) && parent != mapobjectSelected->waypoints_root)
      {
        into   = parent;
        parent = into.get_parent();
      }
      if (parent == mapobjectSelected->waypoints_root)
        into.show();
    }
    collision_path.remove_node();
    needs_update = false;
  }
}

#ifdef _WIN32
# define MIN min
# define MAX max
#else
# define MIN std::min
# define MAX std::max
#endif

void MainWindow::PandaButtonRelease(QMouseEvent*)
{
    bool selected_waypoint = false;

    waypointHovered      = 0;
    mapobjectHovered     = 0;
    dynamicObjectHovered = 0;
    my_task.mouse->GetHoveredAt(my_task.mouse->GetPositionRatio());
    if (mapobjectSelected && !(mapobjectSelected->waypoints_root.is_hidden()))
    {
      if (ui->waypointPicker->isChecked())
      {
        my_task.mouse->GetWaypointHoveredAt(my_task.mouse->GetPositionRatio(), mapobjectSelected->waypoints_root);
        if (waypointHovered)
        {
          WaypointSelect(waypointHovered);
          selected_waypoint = true;
        }
      }
      else if (ui->waypointZoneSelector->isChecked())
      {
        LPoint3 pos   = my_task.CollidingAt(mapobjectSelected);
        float   min_x = MIN(pos.get_x(), wp_select_x);
        float   min_y = MIN(pos.get_y(), wp_select_y);
        float   max_x = MAX(pos.get_x(), wp_select_x);
        float   max_y = MAX(pos.get_y(), wp_select_y);

        SelectWaypointZone(min_x, min_y, max_x, max_y);
        selected_waypoint = true;
      }
    }
    if (!selected_waypoint)
    {
      if (mapobjectHovered)
        MapObjectSelect();
      if (dynamicObjectHovered)
        DynamicObjectSelect();
    }
}

#define MAP_TABS_OBJECTS            2
#define OBJECT_TABS_MAP_OBJECTS     0
#define OBJECT_TABS_DYNAMIC_OBJECTS 1
#define OBJECT_TABS_LIGHTS          2

void MainWindow::LightFocus(WorldLight* light)
{
  if (light)
  {
    LPoint3f relative_pos = light->symbol.get_pos(_window->get_render());

    my_task.camera->CenterCameraInstant(relative_pos);
    my_task.camera->SlideToHeight(relative_pos.get_z());
    ui->worldObjectWidget->SetSelection(light);
  }
}

void MainWindow::MapObjectFocus(MapObject* mapobject)
{
  if (mapobject)
  {
    LPoint3f relative_pos = mapobject->nodePath.get_pos(_window->get_render());

    mapobjectHovered = mapobject;
    MapObjectSelect();
    my_task.camera->CenterCameraInstant(relative_pos);
    my_task.camera->SlideToHeight(relative_pos.get_z());
  }
}

void MainWindow::DynamicObjectFocus(DynamicObject* dynamic_object)
{
  if (dynamic_object)
  {
    LPoint3f relative_pos = dynamic_object->nodePath.get_pos(_window->get_render());

    dynamicObjectHovered = dynamic_object;
    DynamicObjectSelect();
    my_task.camera->CenterCameraInstant(relative_pos);
    my_task.camera->SlideToHeight(relative_pos.get_z());
  }
}

//Pass the locale data from the localization manager to the dialog editor
void MainWindow::SetDefaultLocalization() {
	tabDialog.LoadLocale( tabL18n.GetDefaultLanguage() );
};

extern float editor_camera_height;

void MainWindow::UpdateCameraZoom(int value)
{
  editor_camera_height -= (value / 2);
  my_task.camera->RefreshCameraHeight();
}

void MainWindow::PandaInitialized()
{
    waypointHovered = 0;
    WindowFramework* window = ui->widget->Window();

    _window                 = window;
    my_task.panda_widget    = ui->widget;
    my_task.main_window     = this;
    my_task.window          = window;
    my_task.camera          = new SceneCamera(window, window->get_camera_group());
    my_task.mouse           = new Mouse(window);

    connect(ui->widget, SIGNAL(MousePressed(QMouseEvent*)), this, SLOT(PandaButtonPressed(QMouseEvent*)));
    connect(ui->widget, SIGNAL(MouseRelease(QMouseEvent*)), this, SLOT(PandaButtonRelease(QMouseEvent*)));
    connect(ui->widget, SIGNAL(Scroll(int)),                this, SLOT(UpdateCameraZoom(int)));

    connect(ui->mapMoveLeft,   SIGNAL(clicked()), this, SLOT(CameraMoveLeft()));
    connect(ui->mapMoveBottom, SIGNAL(clicked()), this, SLOT(CameraMoveBottom()));
    connect(ui->mapMoveTop,    SIGNAL(clicked()), this, SLOT(CameraMoveTop()));
    connect(ui->mapMoveRight,  SIGNAL(clicked()), this, SLOT(CameraMoveRight()));

// WAYPOINTS
     connect(ui->waypointVisible,    SIGNAL(toggled(bool)),        this, SLOT(WaypointVisible()));

     connect(ui->waypointConnect,    SIGNAL(clicked()),            this, SLOT(WaypointConnect()));
     connect(ui->waypointDisconnect, SIGNAL(clicked()),            this, SLOT(WaypointDisconnect()));
     connect(ui->waypointSelX,       SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateSelX()));
     connect(ui->waypointSelY,       SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateSelY()));
     connect(ui->waypointSelZ,       SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateSelZ()));
     connect(ui->waypointSelDelete,  SIGNAL(clicked()),            this, SLOT(WaypointSelDelete()));
     connect(ui->waypointClone,      SIGNAL(clicked()),            this, SLOT(WaypointClone()));
     connect(ui->forceFloorAbove,    SIGNAL(clicked()),            this, SLOT(WaypointForceFloorAbove()));

     connect(ui->waypointDiscardSelection, SIGNAL(clicked()), this, SLOT(WaypointDiscardSelection()));

     connect(&wizardObject, SIGNAL(accepted()), this, SLOT(ObjectAdd()));

// ENTRY/EXIT ZONES
     connect(ui->zoneAdd,               SIGNAL(clicked()),                    this, SLOT(ZoneAdd()));
     connect(ui->zoneDelete,            SIGNAL(clicked()),                    this, SLOT(ZoneDelete()));
     connect(ui->zoneSetSelection,      SIGNAL(clicked()),                    this, SLOT(ZoneSetAsSelection()));
     connect(ui->zoneList,              SIGNAL(currentIndexChanged(QString)), this, SLOT(ZoneChanged(QString)));
     connect(ui->zoneAddDestination,    SIGNAL(clicked()),                    this, SLOT(ZoneDestinationAdd()));
     connect(ui->zoneDeleteDestination, SIGNAL(clicked()),                    this, SLOT(ZoneDestinationDelete()));

// MAPOBJECTS
     mapobjectSelected = 0;
     mapobjectHovered  = 0;

// DYNAMICOBJECTS
     ui->objectAddButton->setMenu(&level_add_object_menu);
     level_add_object_menu.addAction(ui->treeWidget->icon_map_object, "Map Object",     this, SLOT(MapObjectWizard()));
     level_add_object_menu.addAction(ui->treeWidget->icon_dyn_object, "Dynamic Object", this, SLOT(DynamicObjectWizard()));
     level_add_object_menu.addAction(ui->treeWidget->icon_light,      "Light",          this, SLOT(LightAdd()));
     level_add_object_menu.addAction(                                 "Character", this, SLOT(CharacterAdd()));

     connect(ui->objectDeleteButton, SIGNAL(clicked()), this, SLOT(DeleteSelection()));

     dynamicObjectSelected = 0;
     dynamicObjectHovered  = 0;

// LIGHTS
     lightSelected      = 0;
     lightIgnoreChanges = false;

     connect(my_task.mouse,      SIGNAL(WaypointHovered(NodePath)), this, SLOT(WaypointHovered(NodePath)));
     connect(my_task.mouse,      SIGNAL(ObjectHovered(NodePath)),   this, SLOT(MapObjectHovered(NodePath)));
     connect(my_task.mouse,      SIGNAL(UnitHovered(NodePath)),     this, SLOT(DynamicObjectHovered(NodePath)));

    waypointSelected = 0;
    waypointHovered  = 0;

    window->enable_keyboard();
    my_task.timer.start();
    AsyncTaskManager::get_global_ptr()->add(&my_task);

    wizardDynObject = false;
    wizardMapObject = false;
    {
      QString level = ui->listMap->currentText();

      if (level != "")
        LoadMap(level);
    }
}

void MainWindow::ObjectAdd()
{
    if (wizardDynObject)
      DynamicObjectAdd();
    else if (wizardMapObject)
      MapObjectAdd();
}

void MainWindow::CharacterAdd()
{
  SelectableResource::Charsheets().SelectResource([this](QString name)
  {
    DataTree* charsheet = DataTree::Factory::JSON("data/charsheets/" + name.toStdString() + ".json");

    if (charsheet)
    {
      {
        Data    data(charsheet);
        Data    appearance   = data["Appearance"];
        QString unique_name  = QInputDialog::getText(this, "Add Character", "Unique name");

        dynamicObjectHovered = world->AddDynamicObject(unique_name.toStdString(),
                                                       DynamicObject::Character,
                                                       appearance["model"].Value(),
                                                       appearance["texture"].Value());
        dynamicObjectHovered->charsheet    = name.toStdString();
        dynamicObjectHovered->script       = data["Behaviour"]["script"].Value();
        dynamicObjectHovered->dialog       = data["Behaviour"]["dialog"].Value();
        dynamicObjectHovered->interactions = 0;
        ui->treeWidget->AddDynamicObject(dynamicObjectHovered);
        DynamicObjectSelect();
      }
      delete charsheet;
    }
  });
}

void MainWindow::DynamicObjectSetWaypoint(DynamicObject* object)
{
  if (object && waypointSelected)
    world->DynamicObjectSetWaypoint(*object, *waypointSelected);
}

void MainWindow::SelectWaypointFromObject(DynamicObject* object)
{
  if (object->waypoint != 0)
    WaypointSelect(object->waypoint);
}

void MainWindow::DynamicObjectWizard()
{
    wizardDynObject = true;
    wizardObject.open();
}

void MainWindow::DynamicObjectAdd()
{
    QString name = wizardObject.GetName();
    QString mod  = wizardObject.GetModel();
    QString tex  = wizardObject.GetTexture();
    float   posx = wizardObject.GetPosX();
    float   posy = wizardObject.GetPosY();
    float   posz = wizardObject.GetPosZ();
    float   scale = wizardObject.GetScale();

    dynamicObjectHovered = world->AddDynamicObject(name.toStdString(), DynamicObject::Character, mod.toStdString(), tex.toStdString());
    dynamicObjectHovered->nodePath.set_pos(posx, posy, posz);
    dynamicObjectHovered->nodePath.set_scale(scale);
    ui->treeWidget->AddDynamicObject(dynamicObjectHovered);
    DynamicObjectSelect();
}

void MainWindow::DynamicObjectHovered(NodePath np)
{
  dynamicObjectHovered = world->GetDynamicObjectFromNodePath(np);
}

void MainWindow::DynamicObjectSelect()
{
    dynamicObjectSelected = dynamicObjectHovered;
    if (dynamicObjectHovered)
      ui->worldObjectWidget->SetSelection(dynamicObjectSelected);
}

void MainWindow::MapObjectWizard()
{
    wizardMapObject = true;
    wizardObject.open();
}

void MainWindow::MapObjectAdd()
{
    wizardMapObject = false;
    QString name  = wizardObject.GetName();
    QString model = wizardObject.GetModel();
    QString text  = wizardObject.GetTexture();
    float   posx  = wizardObject.GetPosX();
    float   posy  = wizardObject.GetPosY();
    float   posz  = wizardObject.GetPosZ();
    float   scale = wizardObject.GetScale();

    mapobjectHovered = world->AddMapObject(name.toStdString(), model.toStdString(), text.toStdString(), posx, posy, posz);
    mapobjectHovered->nodePath.set_scale(scale);
    mapobjectHovered->nodePath.show();
    ui->treeWidget->AddMapObject(mapobjectHovered);
    MapObjectSelect();
}

void MainWindow::DeleteSelection()
{
  dynamicObjectSelected = dynamicObjectHovered = 0;
  mapobjectSelected = mapobjectHovered = 0;
  lightSelected = 0;
  ui->worldObjectWidget->DeleteSelection();
  ui->treeWidget->SetWorld(world);
}

void MainWindow::MapObjectHovered(NodePath path)
{
  mapobjectHovered     = world->GetMapObjectFromNodePath(path);
}

void MainWindow::MapObjectGenerateWaypoints(void)
{
  if (mapobjectSelected)
  {
    wp_generator = new WaypointGenerator(world,
                                         mapobjectSelected,
                                         waypointGenerate.GetMargin(),
                                         waypointGenerate.GetSpacing(),
                                         waypointGenerate.IsOriginCorner());
    connect(wp_generator, SIGNAL(Started()),                     this,            SLOT(DisableLevelEditor()),             Qt::QueuedConnection);
    connect(wp_generator, SIGNAL(Started()),                     ui->progressBar, SLOT(show()),                           Qt::QueuedConnection);
    connect(wp_generator, SIGNAL(UpdateProgress(QString,float)), this,            SLOT(UpdateProgressBar(QString,float)), Qt::QueuedConnection);
    connect(wp_generator, SIGNAL(Done()),                        this,            SLOT(EnableLevelEditor()),              Qt::QueuedConnection);
    connect(wp_generator, SIGNAL(Done()),                        ui->progressBar, SLOT(hide()),                           Qt::QueuedConnection);

    wp_generator->Run();
  }
}

void MainWindow::MapObjectSelect()
{
  if (mapobjectSelected && mapobjectSelected->collider.type != Collider::NONE)
    mapobjectSelected->collider.node.hide();
  mapobjectSelected = 0;
  if (mapobjectHovered)
  {
    ui->treeWidget->SetItemFocused(mapobjectHovered);
    ui->waypointVisible->setChecked(!(mapobjectHovered->waypoints_root.is_hidden()));
    ui->worldObjectWidget->SetSelection(mapobjectHovered);
  }
  mapobjectSelected = mapobjectHovered;
}

void MainWindow::WaypointConnect()
{
    std::list<Waypoint*>::iterator it  = waypointsSelection.begin();
    std::list<Waypoint*>::iterator end = waypointsSelection.end();

    for (; it != end ; ++it)
    {
      std::list<Waypoint*>::iterator subIt  = waypointsSelection.begin();
      std::list<Waypoint*>::iterator subEnd = waypointsSelection.end();

      for (; subIt != subEnd ; ++subIt)
      {
        if (it == subIt)
          continue ;
        (*it)->Connect(*subIt);
        (*it)->SetSelected(false);
        (*it)->SetSelected(true);
      }
    }
}

void MainWindow::WaypointDisconnect()
{
    std::list<Waypoint*>::iterator it  = waypointsSelection.begin();
    std::list<Waypoint*>::iterator end = waypointsSelection.end();

    for (; it != end ; ++it)
    {
      std::list<Waypoint*>::iterator subIt  = waypointsSelection.begin();
      std::list<Waypoint*>::iterator subEnd = waypointsSelection.end();

      for (; subIt != subEnd ; ++subIt)
      {
        if (it == subIt)
          continue ;
        (*it)->Disconnect(*subIt);
        (*it)->SetSelected(false);
        (*it)->SetSelected(true);
      }
    }
}

void MainWindow::WaypointHovered(NodePath path)
{
    if (ui->waypointVisible->isChecked())
    {
      waypointHovered      = world->GetWaypointFromNodePath(path);
    }
}

void MainWindow::WaypointVisible()
{
  if (mapobjectSelected)
  {
    if (ui->waypointVisible->isChecked())
      mapobjectSelected->waypoints_root.show();
    else
      mapobjectSelected->waypoints_root.hide();
  }
}

void MainWindow::UpdateSelection()
{
    QString str;
    bool    enabled;

    str  = QString::number(waypointsSelection.size());
    str += " waypoints selected";
    ui->waypointSelection->setText(str);
    enabled = waypointsSelection.size() > 0;
    ui->waypointEditor->setEnabled(enabled);

    waypointSelX = waypointSelY = waypointSelZ = 0;
    ui->waypointSelX->setValue(waypointSelX);
    ui->waypointSelY->setValue(waypointSelY);
    ui->waypointSelZ->setValue(waypointSelZ);
}

void MainWindow::WaypointSelDelete()
{
    std::list<Waypoint*>::iterator it  = waypointsSelection.begin();
    std::list<Waypoint*>::iterator end = waypointsSelection.end();

    for (; it != end ; ++it)
      world->DeleteWayPoint(*it);
    waypointsSelection.clear();
    UpdateSelection();
}

void MainWindow::WaypointSelect(Waypoint* waypoint)
{
    std::list<Waypoint*>::iterator it = std::find(waypointsSelection.begin(), waypointsSelection.end(), waypoint);

    if (it != waypointsSelection.end())
    {
      (*it)->SetSelected(false);
      waypointsSelection.erase(it);
      if (waypoint == waypointSelected)
      {
          if (waypointsSelection.size() > 0)
            waypointSelected = (*(--(waypointsSelection.end())));
          else
            waypointSelected = 0;
      }
    }
    else
    {
      waypoint->SetSelected(true);
      waypointSelected = waypoint;
      waypointsSelection.push_back(waypoint);
    }
    UpdateSelection();
}

void MainWindow::WaypointForceFloorAbove(void)
{
  unsigned int suggested_floor_above = QInputDialog::getInt(this, "Suggest above floor", "Floor");

  for_each(waypointsSelection.begin(), waypointsSelection.end(), [suggested_floor_above](Waypoint* waypoint)
  {
    waypoint->suggested_floor_above = suggested_floor_above;
  });
}

void MainWindow::WaypointClone(void)
{
  if (waypointSelected != 0)
  {
    NodePath   waypoint_root = waypointSelected->nodePath.get_parent();
    MapObject* map_object    = world->GetMapObjectFromNodePath(waypoint_root.get_parent());

    if (map_object)
    {
      LPoint3f  position = waypointSelected->nodePath.get_pos(world->window->get_render());
      Waypoint* waypoint = world->AddWayPoint(position.get_x(), position.get_y(), position.get_z());

      waypoint->nodePath.reparent_to(map_object->waypoints_root);
      waypoint->nodePath.set_pos(world->window->get_render(), position);
      map_object->waypoints.push_back(waypoint);
    }
  }
}

void MainWindow::WaypointUpdateSelX()
{
    std::list<Waypoint*>::iterator it;
    std::list<Waypoint*>::iterator end    = waypointsSelection.end();
    float                          toMove = ui->waypointSelX->value() - waypointSelX;

    for (it = waypointsSelection.begin() ; it != end ; ++it)
    {
      float posx = (*it)->nodePath.get_x();

      (*it)->nodePath.set_x(posx + toMove);
    }
    for (it = waypointsSelection.begin() ; it != end ; ++it)
      (*it)->PositionChanged();

    waypointSelX = ui->waypointSelX->value();
}

void MainWindow::WaypointUpdateSelY()
{
    std::list<Waypoint*>::iterator it;
    std::list<Waypoint*>::iterator end    = waypointsSelection.end();
    float                          toMove = ui->waypointSelY->value() - waypointSelY;

    for (it = waypointsSelection.begin() ; it != end ; ++it)
    {
      float posy = (*it)->nodePath.get_y();

      (*it)->nodePath.set_y(posy + toMove);
    }
    for (it = waypointsSelection.begin() ; it != end ; ++it)
      (*it)->PositionChanged();

    waypointSelY = ui->waypointSelY->value();
}

void MainWindow::WaypointUpdateSelZ()
{
    std::list<Waypoint*>::iterator it;
    std::list<Waypoint*>::iterator end    = waypointsSelection.end();
    float                          toMove = ui->waypointSelZ->value() - waypointSelZ;

    for (it = waypointsSelection.begin() ; it != end ; ++it)
    {
      float posz = (*it)->nodePath.get_z();

      (*it)->nodePath.set_z(posz + toMove);
    }
    for (it = waypointsSelection.begin() ; it != end ; ++it)
      (*it)->PositionChanged();

    waypointSelZ = ui->waypointSelZ->value();
}

void MainWindow::WaypointDiscardSelection(void)
{
    while (waypointsSelection.size())
      WaypointSelect(waypointsSelection.front());
}

void MainWindow::WaypointSyncTerrain(void)
{
  ui->tabLevelDesigner->setEnabled(false);
  ui->progressBar->show();

  FunctorThread& thread = *FunctorThread::Create([this](void)
  {
    unsigned short                 attempt = 0;
    float                          i       = 0;
    std::list<Waypoint*>::iterator it;

    for (it = waypointsSelection.begin() ; it != waypointsSelection.end() ; ++it, ++i)
    {
      NodePath wp = (*it)->nodePath;
      LPoint3  min_pos;
      float    new_height = wp.get_z();

      CollisionTraverser col_traverser;
      PT(CollisionHandlerQueue) col_queue = new CollisionHandlerQueue;

      PT(CollisionNode) cnode = new CollisionNode("waypointSyncTerrainNode");
      cnode->set_from_collide_mask(CollideMask(ColMask::Object));

      PT(CollisionSegment) segment = new CollisionSegment;
      cnode->add_solid(segment);

      NodePath np = world->window->get_render().attach_new_node(cnode);

      segment->set_point_a(wp.get_x(), wp.get_y(), wp.get_z());
      segment->set_point_b(wp.get_x(), wp.get_y(), wp.get_z() - 100000.f);

      cout << "Looking for collisions" << endl;
      col_traverser.add_collider(np, col_queue);
      col_traverser.traverse(world->window->get_render());
      if (col_queue->get_num_entries())
      {
        col_queue->sort_entries();
        min_pos = col_queue->get_entry(0)->get_surface_point(world->window->get_render());
        std::cout << "Found a collision" << std::endl;
        std::cout << min_pos.get_x() << ", " << min_pos.get_y() << ", " << min_pos.get_z() << std::endl;

        // ALMOST DONE !
        LPoint3 min_point, max_point;
        wp.calc_tight_bounds(min_point, max_point);
        float height = max_point.get_y() - min_point.get_y();

        new_height = min_pos.get_z() + (height / 2);
      }
      if (new_height != wp.get_z()) // Success
      {
        wp.set_z(new_height);
        attempt = 0;
      }
      else if (attempt < 20) // Failure: let's try with a slightly different position
      {
        if (attempt % 2)
          wp.set_x(wp.get_x() + 0.01);
        else
          wp.set_y(wp.get_y() + 0.01);
        --it; --i;
        ++attempt;
      }
      else // Failure: 10 consecutive failures, let's give up
        attempt = 0;
      np.show();
      np.remove_node();
      SigUpdateProgressBar("Waypoints Level Terrain: %p%", i / waypointsSelection.size() * 100.f);
    }
  });
  connect(&thread, SIGNAL(Done()), this,            SLOT(EnableLevelEditor()), Qt::QueuedConnection);
  connect(&thread, SIGNAL(Done()), ui->progressBar, SLOT(hide()),              Qt::QueuedConnection);
  thread.start();
}

void MainWindow::MapFocused(void)
{
    if (ui->tabWidget->currentIndex() != PANDA_TAB)
    {
      disconnect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(MapFocused()));
      LoadMap(ui->listMap->currentText());
    }
}

void MainWindow::LoadMap(const QString& path)
{
    if (ui->tabWidget->currentIndex() != PANDA_TAB)
    {
      connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(MapFocused()));
      return ;
    }
    if (my_task.camera == 0 || (world != 0 && levelName == path))
      return ;
    my_task.camera->SetPosition(0, 0, 75);
    levelName             = path;
    mapobjectSelected     = 0;
    mapobjectHovered      = 0;
    dynamicObjectSelected = 0;
    dynamicObjectHovered  = 0;
    waypointsSelection.clear();
    ui->worldObjectWidget->UnsetSelection();

    FunctorThread&  thread   = *FunctorThread::Create([this](void)
    {
      std::string   fullpath = (QDir::currentPath() + "/maps/" + levelName + ".blob").toStdString();
      std::ifstream file;

      file.open(fullpath.c_str(),ios::binary);
      if (file.is_open())
      {
        Utils::Packet* packet;
        long           begin, end;
        long           size;
        char*          raw;

        begin     = file.tellg();
        file.seekg (0, ios::end);
        end       = file.tellg();
        file.seekg(0, ios::beg);
        size      = end - begin;
        raw       = new char[size + 1];
        file.read(raw, size);
        file.close();
        raw[size] = 0;

        packet = new Utils::Packet(raw, size);
        if (world)
          delete world;
        try
        {
          world = new World(_window);
          world->UnSerialize(*packet);
          std::cout << "World unserialized" << std::endl;
        }
        catch (const Utils::Packet::Exception& exception)
        {
          world = 0;
          QMessageBox::warning(this, "Fatal Error", QString("Map file seems corrupted. Unserializer said: ") + exception.what());
        }
        delete   packet;
        delete[] raw;
      }
      else
        QMessageBox::warning(this, "Fatal Error", "Can't load map file '" + QString::fromStdString(fullpath) + "'");

      std::cout << "Post loading map" << std::endl;
      if (world)
      {
        ui->treeWidget->SetWorld(world);
        dialogSaveMap.SetEnabledSunlight(world->sunlight_enabled);

        Ui::MainWindow* _ui = ui;

        ui->zoneList->clear();
        ForEach(world->zones,  [_ui](Zone& zone)  { _ui->zoneList->addItem(zone.name.c_str());  });

        for (int i = 0 ; i < ui->listMap->count() ; ++i)
        {
          if (ui->listMap->itemText(i) == levelName)
          {
            ui->listMap->setCurrentIndex(i);
            break ;
          }
        }

        my_task.mouse->SetWorld(world);
        dialogObject.SetWorld(world);
        ui->worldObjectWidget->SetWorld(world);
        std::cout << "Finished Loading Map" << std::endl;
      }
    });

    QPandaApplication::SetPandaEnabled(false);
    DisableLevelEditor();
    connect(&thread, SIGNAL(Done()), ui->progressBar, SLOT(hide()),              Qt::QueuedConnection);
    connect(&thread, SIGNAL(Done()), this,            SLOT(EnableLevelEditor()), Qt::QueuedConnection);
    thread.start_sync();
    //thread.start();
}

void MainWindow::ZoneAdd()
{
    QString name = QInputDialog::getText(this, "Create Zone", "Name");

    if (name != "")
    {
        world->AddZone(name.toStdString());
        ui->zoneList->addItem(name);
    }
}

void MainWindow::ZoneDelete()
{
    QString name = ui->zoneList->currentText();

    if (name != "")
    {
        ui->zoneList->removeItem(ui->zoneList->currentIndex());
        world->DeleteZone(name.toStdString());
    }
}

void MainWindow::ZoneSetAsSelection()
{
  QString name = ui->zoneList->currentText();

  if (name != "")
  {
    Zone* zone = world->GetZoneByName(name.toStdString());

    if (zone)
      zone->waypoints = waypointsSelection;
  }
}

void MainWindow::ZoneChanged(QString string)
{
  if (string != "")
  {
    Zone* zone = world->GetZoneByName(string.toStdString());

    if (zone)
    {
      ui->zoneDestinations->clear();
      ForEach(zone->destinations, [this](std::string name) { ui->zoneDestinations->addItem(name.c_str()); });

      std::list<Waypoint*> tmp = waypointsSelection;
      ForEach(tmp,             [this](Waypoint* wp) { this->WaypointSelect(wp); });
      ForEach(zone->waypoints, [this](Waypoint* wp) { this->WaypointSelect(wp); });
    }
  }
}

void MainWindow::LightAdd(void)
{
    QString name = QInputDialog::getText(this, "Add light", "Light name");

    if (name != "")
    {
        world->AddLight(WorldLight::Directional, name.toStdString());
        ui->treeWidget->AddWorldLight(world->GetLightByName(name.toStdString()));
    }
}

void MainWindow::ZoneDestinationAdd()
{
  QString name = ui->zoneList->currentText();
  Zone*   zone = world->GetZoneByName(name.toStdString());

  if (zone)
  {
    QString newDest = QInputDialog::getText(this, "Add destination", "Map name");

    zone->destinations.push_back(newDest.toStdString());
    ui->zoneDestinations->addItem(newDest);
  }
}

void MainWindow::ZoneDestinationDelete()
{
  QString name = ui->zoneList->currentText();
  Zone*   zone = world->GetZoneByName(name.toStdString());

  if (zone)
  {
    QListWidgetItem* item = ui->zoneDestinations->currentItem();

    if (item)
    {
      QString destName = item->text();

      std::remove(zone->destinations.begin(), zone->destinations.end(), destName.toStdString());
      ui->zoneDestinations->removeItemWidget(item);
      delete item;
    }
  }
}

void MainWindow::EnableLevelEditor(void)
{
  if (level_editor_lock > 0)
    level_editor_lock--;
  if (level_editor_lock == 0)
  {
    ui->tabLevelDesigner->setEnabled(true);
    QPandaApplication::SetPandaEnabled(true);
  }
}

void MainWindow::DisableLevelEditor(void)
{
    if (level_editor_lock == 0)
      ui->tabLevelDesigner->setEnabled(false);
    level_editor_lock++;
}

void MainWindow::UpdateProgressBar(QString fmt, float percentage)
{
    ui->progressBar->setFormat(fmt);
    ui->progressBar->setValue(percentage);
}

void MainWindow::DisplayError(QString title, QString message)
{
  QMessageBox::warning(this, title, message);
}

void MainWindow::CallbackUpdateProgressBar(const std::string& label, float percentage)
{
  QString format = QString::fromStdString(label) + "%p%";

  SigUpdateProgressBar(format, percentage);
}

void MainWindow::SaveMap()
{
  if (world)
  {
    ui->tabLevelDesigner->setEnabled(false);
    world->do_compile_doors     = dialogSaveMap.DoCompileDoors();
    world->do_compile_waypoints = dialogSaveMap.DoCompileWaypoints();
    world->sunlight_enabled     = dialogSaveMap.DoEnableSunlight();

    FunctorThread& thread = *FunctorThread::Create([this, &thread](void)
    {
      std::ofstream file;
      std::string   path = (QDir::currentPath() + "/maps/" + levelName + ".blob").toStdString();

      if (world)
      {
        Utils::Packet packet;
        MainWindow*   self = this;

        world->Serialize(packet, [self](const std::string& label, float percentage)
        { self->CallbackUpdateProgressBar(label, percentage); });
        file.open(path.c_str(),ios::binary);
        file.write(packet.raw(), packet.size());
        file.close();
      }
      else
        SigDisplayError("Fatal Error", "Cannot save map");
    });
    ui->progressBar->show();
    connect(&thread, SIGNAL(Done()), ui->progressBar, SLOT(hide()), Qt::QueuedConnection);
    connect(&thread, SIGNAL(Done()), this, SLOT(EnableLevelEditor()), Qt::QueuedConnection);
    if (save_map_use_thread)
      thread.start();
    else
      thread.start_sync();
  }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    QMainWindow::closeEvent(e);
    emit Closed();
}
