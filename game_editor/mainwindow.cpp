#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpandaapplication.h"
#include <QInputDialog>

#include "scenecamera.h"
#include "mouse.h"
#include <QElapsedTimer>

extern PandaFramework framework;

QString objectTypes[] = { "NPC", "Shelf", "Locker", "Door" };

struct PandaTask : public AsyncTask
{
    WindowFramework* window;
    SceneCamera*     camera;
    Mouse*           mouse;
    QElapsedTimer    timer;

    int              brushTileId, brushTileX, brushTileY;

    PandaTask() { }

    DoneStatus do_task(void)
    {
        float elapsedTime = timer.elapsed();

        elapsedTime /= 1000;
        camera->Run(elapsedTime);
        mouse->Run();
        timer.start();
        return (AsyncTask::DS_cont);
    }
};

PandaTask my_task;

QString strScriptCategories[3] = {
    "Dialogues", "Quests", "Artificial Intelligence"
};

QString pathScriptCategories[3] = {
    "dialogs", "quests", "ai"
};

MainWindow::MainWindow(QPandaApplication* app, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), tabScript(this, ui), tabDialog(this, ui), tabL18n(this, ui), splashScreen(this), wizardObject(this), dialogObject(this)
{
    QIcon iconScript("icons/script.png");
    QIcon iconDialogs("icons/dialogs.png");
    QIcon iconLevel("icons/level.png");
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

    objectFile    = 0;
    ui->setupUi(this);

    ui->waypointAdd->setIcon(iconAdd);
    ui->waypointRemove->setIcon(iconDelete);
    ui->waypointConnect->setIcon(iconConnect);
    ui->waypointDisconnect->setIcon(iconDisconnect);
    ui->waypointGenerate->setIcon(waypointGenerate);
    ui->waypointSelDelete->setIcon(iconDelete);
    waypointSelX = waypointSelY = waypointSelZ = 0;

    ui->objectRemove->setIcon(iconDelete);

    ui->tabWidget->setTabIcon(0, iconLevel);
    ui->tabWidget->setTabIcon(1, iconScript);
    ui->tabWidget->setTabIcon(2, iconDialogs);
    ui->tabWidget->setTabIcon(3, iconLanguage);
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
    ui->mapNewObject->setIcon(iconAdd);
    ui->objectEdit->setIcon(iconEdit);
    ui->saveMap->setIcon(iconSave);

    setWindowTitle("Fallout Equestria Editor");
    setWindowIcon(QIcon("icons/app-icon.png"));

    splashScreen.open();

    connect(&splashScreen, SIGNAL(rejected()), app,  SLOT(Terminate()));
    connect(&splashScreen, SIGNAL(accepted()), this, SLOT(LoadProject()));

    connect(ui->widget, SIGNAL(Initialized()), this, SLOT(PandaInitialized()));
    connect(this,       SIGNAL(Closed()),      app,  SLOT(Terminate()));

    for (short i = 0 ; i < 4 ; ++i)
    {
    }

    for (short i = 0 ; i < 3 ; ++i)
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

	connect(&tabDialog, SIGNAL(RequestLocale()), this, SLOT(SetDefaultLocalization()) );

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
    connect(ui->saveMap, SIGNAL(clicked()),                    this, SLOT(SaveMap()));
    connect(ui->mapNew,  SIGNAL(clicked()),                    &dialogNewMap, SLOT(open()));

    ui->scriptList->header()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadProject()
{
    bool    success = true;
    QString path    = splashScreen.GetProjectPath();
    //QString path = "/home/plaristote/Work/fallout-equestria/build";
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

        pathToCheck << "maps/" << "data/" << "scripts/" << "data/l18n" << "data/dialogs";
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
        LoadAllMaps();
    }
    else
        splashScreen.open();
}

void MainWindow::LoadAllMaps()
{
    QDir        dir("maps/");
    QStringList fileList = dir.entryList();
    QRegExp     regexp("\.json$");

    foreach (QString string, fileList)
    {
        if (!(string.contains(regexp)))
          continue ;
        QString name = string.replace(regexp, "");

        ui->listMap->addItem(name);
    }
}

void MainWindow::FilterInit()
{
    if (ui->scriptSearch->text() == "Search...")
        ui->scriptSearch->setText("");
    /*if (ui->dialogSearch->text() == "Search...")
        ui->dialogSearch->setText("");*/
}

void MainWindow::CameraMoveBottom()
{
    my_task.camera->MoveV(-15.f);
}

void MainWindow::CameraMoveTop()
{
    my_task.camera->MoveV(15.f);
}

void MainWindow::CameraMoveLeft()
{
    my_task.camera->MoveH(-15.f);
}

void MainWindow::CameraMoveRight()
{
    my_task.camera->MoveH(15.f);
}

void MainWindow::DrawMap()
{
   if (currentHoveredCaseX != -1)
   {
      if (my_task.brushTileId == -1)
      {
        my_task.brushTileId = 0;
        my_task.brushTileX  = currentHoveredCaseX;
        my_task.brushTileY  = currentHoveredCaseY;
      }
      else
        my_task.brushTileId = -1;
   }
}

void MainWindow::PandaButtonPressed(QMouseEvent*)
{
}

void MainWindow::PandaButtonRelease(QMouseEvent*)
{
    waypointHovered      = 0;
    mapobjectHovered     = 0;
    dynamicObjectHovered = 0;
    my_task.mouse->GetHoveredAt(my_task.mouse->GetPosition());
    if      (waypointHovered)
      WaypointSelect(waypointHovered);
    if (mapobjectHovered)
      MapObjectSelect();
    if (dynamicObjectHovered)
      DynamicObjectSelect();
}

//Pass the locale data from the localization manager to the dialog editor
void MainWindow::SetDefaultLocalization() {
	tabDialog.LoadLocale( tabL18n.GetDefaultLanguage() );
};

void MainWindow::PandaInitialized()
{
    waypointHovered = 0;
    WindowFramework* window = ui->widget->Window();

    my_task.brushTileId = -1;
    my_task.window  = window;
    my_task.camera  = new SceneCamera(window, window->get_camera_group());
    my_task.mouse   = new Mouse(window);

    connect(ui->widget, SIGNAL(MousePressed(QMouseEvent*)), this, SLOT(PandaButtonPressed(QMouseEvent*)));
    connect(ui->widget, SIGNAL(MouseRelease(QMouseEvent*)), this, SLOT(PandaButtonRelease(QMouseEvent*)));

    connect(ui->mapMoveLeft,   SIGNAL(clicked()), this, SLOT(CameraMoveLeft()));
    connect(ui->mapMoveBottom, SIGNAL(clicked()), this, SLOT(CameraMoveBottom()));
    connect(ui->mapMoveTop,    SIGNAL(clicked()), this, SLOT(CameraMoveTop()));
    connect(ui->mapMoveRight,  SIGNAL(clicked()), this, SLOT(CameraMoveRight()));

    window->enable_keyboard();

     world = new World(window);

// WAYPOINTS
     connect(ui->waypointAdd, SIGNAL(clicked()), this, SLOT(WaypointAdd()));
     connect(ui->waypointRemove, SIGNAL(clicked()), this, SLOT(WaypointDelete()));
     connect(ui->waypointVisible, SIGNAL(toggled(bool)), this, SLOT(WaypointVisible()));
     connect(ui->waypointX, SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateX()));
     connect(ui->waypointY, SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateY()));
     connect(ui->waypointZ, SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateZ()));
     connect(ui->waypointConnect, SIGNAL(clicked()), this, SLOT(WaypointConnect()));
     connect(ui->waypointDisconnect, SIGNAL(clicked()), this, SLOT(WaypointDisconnect()));

     connect(ui->waypointSelX, SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateSelX()));
     connect(ui->waypointSelY, SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateSelY()));
     connect(ui->waypointSelZ, SIGNAL(valueChanged(double)), this, SLOT(WaypointUpdateSelZ()));
     connect(ui->waypointSelDelete, SIGNAL(clicked()), this, SLOT(WaypointSelDelete()));

     waypointGenerate.SetWorld(world);
     connect(ui->waypointGenerate, SIGNAL(clicked()), &waypointGenerate, SLOT(open()));

     connect(&wizardObject, SIGNAL(accepted()), this, SLOT(ObjectAdd()));

// MAPOBJECTS
     mapobjectSelected = 0;
     mapobjectHovered  = 0;
     connect(ui->mapNewObject, SIGNAL(clicked()), this, SLOT(MapObjectWizard()));
     //connect(ui->objectEdit, SIGNAL(clicked()), &dialogObject, SLOT(open()));
     //connect(&dialogObject, SIGNAL(accepted()), this, SLOT(EditObject()));
     connect(ui->mapObjectVisible, SIGNAL(toggled(bool)), this, SLOT(MapObjectVisible()));

     connect(ui->objectRemove, SIGNAL(clicked()), this, SLOT(MapObjectDelete()));
     connect(ui->objectPosX, SIGNAL(valueChanged(double)), this, SLOT(MapObjectUpdateX()));
     connect(ui->objectPosY, SIGNAL(valueChanged(double)), this, SLOT(MapObjectUpdateY()));
     connect(ui->objectPosZ, SIGNAL(valueChanged(double)), this, SLOT(MapObjectUpdateZ()));
     connect(ui->objectRotationX, SIGNAL(valueChanged(double)), this, SLOT(MapObjectRotationX()));
     connect(ui->objectRotationY, SIGNAL(valueChanged(double)), this, SLOT(MapObjectRotationY()));
     connect(ui->objectRotationZ, SIGNAL(valueChanged(double)), this, SLOT(MapObjectRotationZ()));
     connect(ui->objectScaleX, SIGNAL(valueChanged(double)), this, SLOT(MapObjectScaleX()));
     connect(ui->objectScaleY, SIGNAL(valueChanged(double)), this, SLOT(MapObjectScaleY()));
     connect(ui->objectScaleZ, SIGNAL(valueChanged(double)), this, SLOT(MapObjectScaleZ()));
     connect(ui->objectName, SIGNAL(textChanged(QString)), this, SLOT(MapObjectNameChanged(QString)));

// DYNAMICOBJECTS
     dynamicObjectSelected = 0;
     dynamicObjectHovered  = 0;
     connect(ui->interObjVisible, SIGNAL(toggled(bool)),     this, SLOT(DynamicObjectVisible()));
     connect(ui->interObjAdd,  SIGNAL(clicked()),            this, SLOT(DynamicObjectWizard()));
     connect(ui->interObjName, SIGNAL(textChanged(QString)), this, SLOT(DynamicObjectNameChanged(QString)));
     connect(ui->interObjEdit, SIGNAL(clicked()), &dialogObject,   SLOT(open()));
     connect(ui->interObjRemove, SIGNAL(clicked()),          this, SLOT(DynamicObjectDelete()));
     connect(ui->interObjWaypoint, SIGNAL(clicked()),        this, SLOT(DynamicObjectSetWaypoint()));

     connect(ui->interObjPosX, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectUpdateX()));
     connect(ui->interObjPosY, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectUpdateY()));
     connect(ui->interObjPosZ, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectUpdateZ()));
     connect(ui->interObjRotX, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectRotationX()));
     connect(ui->interObjRotY, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectRotationY()));
     connect(ui->interObjRotZ, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectRotationZ()));
     connect(ui->interObjScaleX, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectScaleX()));
     connect(ui->interObjScaleY, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectScaleY()));
     connect(ui->interObjScaleZ, SIGNAL(valueChanged(double)), this, SLOT(DynamicObjectScaleZ()));

     connect(my_task.mouse, SIGNAL(WaypointHovered(NodePath)), this, SLOT(WaypointHovered(NodePath)));
     connect(my_task.mouse, SIGNAL(ObjectHovered(NodePath)),   this, SLOT(MapObjectHovered(NodePath)));
     connect(my_task.mouse, SIGNAL(UnitHovered(NodePath)),     this, SLOT(DynamicObjectHovered(NodePath)));

     waypointSelected = 0;
     waypointHovered  = 0;

    my_task.timer.start();
    //AsyncTaskManager::get_global_ptr()->add(&my_task);

    wizardDynObject = false;
    wizardMapObject = false;
}

void MainWindow::ObjectAdd()
{
    if (wizardDynObject)
      DynamicObjectAdd();
    else if (wizardMapObject)
      MapObjectAdd();
}

void MainWindow::DynamicObjectSetWaypoint()
{
    if (dynamicObjectSelected)
      dynamicObjectSelected->waypoint = waypointSelected;
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

    dynamicObjectHovered = world->AddDynamicObject(name.toStdString(), DynamicObject::Character, mod.toStdString(), tex.toStdString());
    DynamicObjectSelect();
}

void MainWindow::DynamicObjectDelete()
{
    if (dynamicObjectSelected)
    {
      DynamicObject* toDel = dynamicObjectSelected;

      dynamicObjectHovered  = 0;
      DynamicObjectSelect();
      world->DeleteDynamicObject(toDel);
    }
}

void MainWindow::DynamicObjectHovered(NodePath np)
{
    if (ui->interObjVisible->isChecked())
    {
      dynamicObjectHovered = world->GetDynamicObjectFromNodePath(np);
      ui->labelStatus->setText(QString::fromStdString(dynamicObjectHovered->nodePath.get_name()));
    }
}

void MainWindow::DynamicObjectSelect()
{
    ui->interObjEditor->setEnabled(dynamicObjectHovered != 0);
    dynamicObjectSelected = dynamicObjectHovered;
    if (dynamicObjectHovered)
    {
        LVecBase3 pos, hpr, scale;
        NodePath  np = dynamicObjectHovered->nodePath;

        pos   = np.get_pos();
        hpr   = np.get_hpr();
        scale = np.get_scale();
        ui->interObjName->setText(QString::fromStdString(np.get_name()));
        ui->interObjPosX->setValue(pos.get_x());
        ui->interObjPosY->setValue(pos.get_y());
        ui->interObjPosZ->setValue(pos.get_z());
        ui->interObjRotX->setValue(hpr.get_x());
        ui->interObjRotY->setValue(hpr.get_y());
        ui->interObjRotZ->setValue(hpr.get_z());
        ui->interObjScaleX->setValue(scale.get_x());
        ui->interObjScaleY->setValue(scale.get_y());
        ui->interObjScaleZ->setValue(scale.get_z());
        dialogObject.SetCurrentObject(dynamicObjectSelected);
    }
}

void MainWindow::DynamicObjectNameChanged(QString name)
{
    if (dynamicObjectSelected)
      dynamicObjectSelected->nodePath.set_name(name.toStdString());
}

void MainWindow::DynamicObjectUpdateX()
{
    if (dynamicObjectSelected)
    {
        NodePath  np  = dynamicObjectSelected->nodePath;
        LVecBase3 pos = np.get_pos();

        pos.set_x(ui->interObjPosX->value());
        np.set_pos(pos);
    }
}

void MainWindow::DynamicObjectUpdateY()
{
    if (dynamicObjectSelected)
    {
        NodePath  np  = dynamicObjectSelected->nodePath;
        LVecBase3 pos = np.get_pos();

        pos.set_y(ui->interObjPosY->value());
        np.set_pos(pos);
    }
}

void MainWindow::DynamicObjectUpdateZ()
{
    if (dynamicObjectSelected)
    {
        NodePath  np  = dynamicObjectSelected->nodePath;
        LVecBase3 pos = np.get_pos();

        pos.set_z(ui->interObjPosZ->value());
        np.set_pos(pos);
    }
}

void MainWindow::DynamicObjectRotationX()
{
    if (dynamicObjectSelected)
    {
        NodePath  np  = dynamicObjectSelected->nodePath;
        LVecBase3 hpr = np.get_hpr();

        hpr.set_x(ui->interObjRotX->value());
        np.set_hpr(hpr);
    }
}

void MainWindow::DynamicObjectRotationY()
{
    if (dynamicObjectSelected)
    {
        NodePath  np  = dynamicObjectSelected->nodePath;
        LVecBase3 hpr = np.get_hpr();

        hpr.set_y(ui->interObjRotY->value());
        np.set_hpr(hpr);
    }
}

void MainWindow::DynamicObjectRotationZ()
{
    if (dynamicObjectSelected)
    {
        NodePath  np  = dynamicObjectSelected->nodePath;
        LVecBase3 hpr = np.get_hpr();

        hpr.set_z(ui->interObjRotZ->value());
        np.set_hpr(hpr);
    }
}

void MainWindow::DynamicObjectScaleX()
{
    if (dynamicObjectSelected)
    {
        NodePath  np    = dynamicObjectSelected->nodePath;
        LVecBase3 scale = np.get_scale();

        scale.set_x(ui->interObjScaleX->value());
        np.set_scale(scale);
    }
}

void MainWindow::DynamicObjectScaleY()
{
    if (dynamicObjectSelected)
    {
        NodePath  np    = dynamicObjectSelected->nodePath;
        LVecBase3 scale = np.get_scale();

        scale.set_y(ui->interObjScaleY->value());
        np.set_scale(scale);
    }
}

void MainWindow::DynamicObjectScaleZ()
{
    if (dynamicObjectSelected)
    {
        NodePath  np    = dynamicObjectSelected->nodePath;
        LVecBase3 scale = np.get_scale();

        scale.set_z(ui->interObjScaleZ->value());
        np.set_scale(scale);
    }
}

void MainWindow::DynamicObjectVisible()
{
    world->SetDynamicObjectsVisible(ui->interObjVisible->isChecked());
}

void MainWindow::MapObjectVisible()
{
    world->SetMapObjectsVisible(ui->mapObjectVisible->isChecked());
}

void MainWindow::MapObjectWizard()
{
    std::cout << "MAP OBJECT WIZARD" << std::endl;
    wizardMapObject = true;
    wizardObject.open();
}

void MainWindow::MapObjectAdd()
{
    std::cout << "POIL DE BITE" << std::endl;
    wizardMapObject = false;
    QString name  = wizardObject.GetName();
    QString model = wizardObject.GetModel();
    QString text  = wizardObject.GetTexture();
    float   posx  = wizardObject.GetPosX();
    float   posy  = wizardObject.GetPosY();
    float   scale = wizardObject.GetScale();

    mapobjectHovered = world->AddMapObject(name.toStdString(), model.toStdString(), text.toStdString(), posx, posy, 0);
    mapobjectHovered->nodePath.set_scale(scale);
    mapobjectHovered->nodePath.show();
    MapObjectSelect();
}

void MainWindow::MapObjectDelete()
{
    if (mapobjectSelected)
    {
      MapObject* toDel = mapobjectSelected;

      mapobjectHovered  = 0;
      MapObjectSelect();
      world->DeleteMapObject(toDel);
    }
}

void MainWindow::MapObjectHovered(NodePath path)
{
    if (ui->mapObjectVisible->isChecked())
    {
      mapobjectHovered     = world->GetMapObjectFromNodePath(path);
      ui->labelStatus->setText(QString::fromStdString(mapobjectHovered->nodePath.get_name()));
    }
}

void MainWindow::MapObjectNameChanged(QString name)
{
    if (mapobjectSelected)
      mapobjectSelected->nodePath.set_name(name.toStdString());
}

void MainWindow::MapObjectSelect()
{
    mapobjectSelected = mapobjectHovered;
    if (mapobjectSelected)
    {
        ui->objectScaleX->setValue(mapobjectSelected->nodePath.get_scale().get_x());
        ui->objectScaleY->setValue(mapobjectSelected->nodePath.get_scale().get_y());
        ui->objectScaleZ->setValue(mapobjectSelected->nodePath.get_scale().get_z());
        ui->objectPosX->setValue(mapobjectSelected->nodePath.get_x());
        ui->objectPosY->setValue(mapobjectSelected->nodePath.get_y());
        ui->objectPosZ->setValue(mapobjectSelected->nodePath.get_z());
        ui->objectRotationX->setValue(mapobjectSelected->nodePath.get_hpr().get_x());
        ui->objectRotationY->setValue(mapobjectSelected->nodePath.get_hpr().get_y());
        ui->objectRotationZ->setValue(mapobjectSelected->nodePath.get_hpr().get_z());
        ui->objectName->setText(QString::fromStdString(mapobjectSelected->nodePath.get_name()));
        ui->objectEditor->setEnabled(true);
        ui->objectName->setEnabled(true);
    }
    else
        ui->objectEditor->setEnabled(false);
}

void MainWindow::MapObjectUpdateX()
{
    if (mapobjectSelected)
      mapobjectSelected->nodePath.set_x(ui->objectPosX->value());
}

void MainWindow::MapObjectUpdateY()
{
    if (mapobjectSelected)
      mapobjectSelected->nodePath.set_y(ui->objectPosY->value());
}

void MainWindow::MapObjectUpdateZ()
{
    if (mapobjectSelected)
      mapobjectSelected->nodePath.set_z(ui->objectPosZ->value());
}

void MainWindow::MapObjectRotationX()
{
    if (mapobjectSelected)
    {
      LVecBase3 hpr = mapobjectSelected->nodePath.get_hpr();

      hpr.set_x(ui->objectRotationX->value());
      mapobjectSelected->nodePath.set_hpr(hpr);
    }
}

void MainWindow::MapObjectRotationY()
{
    if (mapobjectSelected)
    {
      LVecBase3 hpr = mapobjectSelected->nodePath.get_hpr();

      hpr.set_y(ui->objectRotationY->value());
      mapobjectSelected->nodePath.set_hpr(hpr);
    }
}

void MainWindow::MapObjectRotationZ()
{
    if (mapobjectSelected)
    {
      LVecBase3 hpr = mapobjectSelected->nodePath.get_hpr();

      hpr.set_z(ui->objectRotationZ->value());
      mapobjectSelected->nodePath.set_hpr(hpr);
    }
}

void MainWindow::MapObjectScaleX()
{
    if (mapobjectSelected)
    {
      LVecBase3 scale = mapobjectSelected->nodePath.get_scale();

      scale.set_x(ui->objectScaleX->value());
      mapobjectSelected->nodePath.set_scale(scale);
    }
}

void MainWindow::MapObjectScaleY()
{
    if (mapobjectSelected)
    {
      LVecBase3 scale = mapobjectSelected->nodePath.get_scale();

      scale.set_y(ui->objectScaleY->value());
      mapobjectSelected->nodePath.set_scale(scale);
    }
}

void MainWindow::MapObjectScaleZ()
{
    if (mapobjectSelected)
    {
      LVecBase3 scale = mapobjectSelected->nodePath.get_scale();

      scale.set_z(ui->objectScaleZ->value());
      mapobjectSelected->nodePath.set_scale(scale);
    }
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
      }
    }
}

void MainWindow::WaypointHovered(NodePath path)
{
    if (ui->waypointVisible->isChecked())
    {
      waypointHovered      = world->GetWaypointFromNodePath(path);
      ui->labelStatus->setText("Hovering a waypoint");
    }
}

void MainWindow::WaypointVisible()
{
    world->SetWaypointsVisible(ui->waypointVisible->isChecked());
}

void MainWindow::WaypointUpdateX()
{
  if (waypointSelected)
  {
    waypointSelected->nodePath.set_x(ui->waypointX->value());
    waypointSelected->PositionChanged();
  }
}

void MainWindow::WaypointUpdateY()
{
  if (waypointSelected)
  {
    waypointSelected->nodePath.set_y(ui->waypointY->value());
    waypointSelected->PositionChanged();
  }
}

void MainWindow::WaypointUpdateZ()
{
  if (waypointSelected)
  {
    waypointSelected->nodePath.set_z(ui->waypointZ->value());
    waypointSelected->PositionChanged();
  }
}

void MainWindow::UpdateSelection()
{
    QString str;
    bool    enabled;

    str  = QString::number(waypointsSelection.size());
    str += " waypoints selected";
    ui->waypointSelection->setText(str);
    enabled = waypointsSelection.size() > 1;
    ui->waypointEditor->setEnabled(enabled);

    waypointSelX = waypointSelY = waypointSelZ = 0;
    ui->waypointSelX->setValue(waypointSelX);
    ui->waypointSelY->setValue(waypointSelY);
    ui->waypointSelZ->setValue(waypointSelZ);
}

void MainWindow::WaypointAdd()
{
    float     posx, posy, posz;
    Waypoint* created;

    posx = ui->waypointX->value();
    posy = ui->waypointY->value();
    posz = ui->waypointZ->value();
    created = world->AddWayPoint(posx, posy, posz);
    WaypointSelect(created);
}

void MainWindow::WaypointDelete()
{
    Waypoint* toDel = waypointSelected;

    if (toDel)
    {
        WaypointSelect(toDel); // This unselects the waypoint
        world->DeleteWayPoint(toDel);
    }
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
      ui->waypointX->setValue(waypoint->nodePath.get_x());
      ui->waypointY->setValue(waypoint->nodePath.get_y());
      ui->waypointZ->setValue(waypoint->nodePath.get_z());
    }
    UpdateSelection();
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


void MainWindow::LoadMap(const QString& path)
{
    levelName = path;

    std::ifstream file;
    std::string   fullpath = (QDir::currentPath() + "/maps/" + levelName + ".blob").toStdString();

	file.open(fullpath.c_str(),ios::binary);
    if (file.is_open())
    {
        try
        {
            long  begin, end;
            long  size;
            char* raw;

            begin     = file.tellg();
            file.seekg (0, ios::end);
            end       = file.tellg();
            file.seekg(0, ios::beg);
            size      = end - begin;
            raw       = new char[size + 1];
            file.read(raw, size);
            file.close();
            raw[size] = 0;

            Utils::Packet* packet;

            packet = new Utils::Packet(raw, size);

            if (world)
              world->UnSerialize(*packet);
            else
              QMessageBox::warning(this, "Fatal Error", "World class not created. This whole thing's about to crash");

            delete   packet;
            delete[] raw;
        }
        catch (unsigned int error)
        {
          QMessageBox::warning(this, "Fatal Error", "Map file is corrupted. You are sooooo screwed.");
        }
    }
    else
      QMessageBox::warning(this, "Fatal Error", "Can't load map file '" + QString::fromStdString(fullpath) + "'");
}

void MainWindow::SaveMap()
{
  if (world)
  {
    std::ofstream file;
    std::string   path = (QDir::currentPath() + "/maps/" + levelName + ".blob").toStdString();

    file.open(path.c_str(),ios::binary);
    if (file.is_open())
    {
      Utils::Packet packet;

      world->Serialize(packet);
      packet.PrintContent();
      file.write(packet.raw(), packet.size());
      file.close();
    }
    else
      QMessageBox::warning(this, "Fatal Error", "Cannot save map");
  }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    QMainWindow::closeEvent(e);
    emit Closed();
}
