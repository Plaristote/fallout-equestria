#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <panda3d/cmath.h>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include "dialognewscript.h"
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include "qangelscriptedit.h"
#include <QDir>
#include "datatree.hpp"
#include <QListWidgetItem>
#include "dialogsplashscreen.h"
#include "dialognewmap.h"
#include "wizardobject.h"
#include "dialogobject.h"
#include "dialogwaypointgenerate.h"
#include "world/world.h"
#include "tabscript.h"
#include "tabdialog.h"
#include "tabl18n.h"
#include "serializer.hpp"
#include "dialogsavemap.h"
#include <QMenu>

namespace Ui {
class MainWindow;
}

class WindowFramework;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent*);

    void LoadAllMaps(void);
    void LoadAllStatsheets(void);

    bool CloseAllScript(void);

    void ShowWaypointZone(void);

    // WORKAROUND(S) FOR SHITTY MSVC C++11 SUPPORT:
    void CallbackUpdateProgressBar(const std::string&, float);
signals:
    void Closed(void);
    void SigDisplayError(QString title, QString message);
    void SigUpdateProgressBar(QString label, float value);

public slots:
    //void WaypointHovered(NodePath);
    void PandaButtonPressed(QMouseEvent*);
    void PandaButtonRelease(QMouseEvent*);

private slots:
    void SetDefaultLocalization(void);
    void PandaInitialized(void);
    void LoadProject(void);
    void FilterInit(void);
    void CurrentTabChanged(int);
    void EnableLevelEditor(void);
    void DisableLevelEditor(void);
    void UpdateProgressBar(QString label, float value);
    void DisplayError(QString title, QString message);
    void SetFreeCamera(bool);
    void SetCameraLocked(bool);
    void UpdateCameraZoom(int);
    void CopyClicked(void);
    void PasteClicked(void);
    void FilterCharacters(QString);

    // CHARSHEETS
    void AddCharsheet(void);
    void DeleteCharsheet(void);

    // MAPS
    void LoadMap(const QString&);
    void SaveMap(void);
    void CreateMap(void);
    void MapFocused(void);

    void CameraMoveTop(void);
    void CameraMoveLeft(void);
    void CameraMoveRight(void);
    void CameraMoveBottom(void);

    // WAYPOINTS
    void WaypointVisible(void);
    void WaypointConnect(void);
    void WaypointDisconnect(void);
    void WaypointUpdateSelX(void);
    void WaypointUpdateSelY(void);
    void WaypointUpdateSelZ(void);
    void WaypointSelDelete(void);
    void WaypointHovered(NodePath);
    void WaypointSelect(Waypoint*);
    void WaypointClone(void);
    void UpdateSelection(void);
    void WaypointDiscardSelection(void);
    void WaypointSyncTerrain(void);
    void TerrainPickerPicked(bool);
    void TerrainSelectorPicked(bool);
    void WaypointForceFloorAbove(void);
    void HideAlLWaypoints(void);

    // ZONES
    void ZoneAdd(void);
    void ZoneDelete(void);
    void ZoneSetAsSelection(void);
    void ZoneChanged(QString);
    void ZoneDestinationAdd(void);
    void ZoneDestinationDelete(void);

    // OBJECTS
    void ObjectAdd(void);
    void DeleteSelection(void);

    // MAPOBJECTS
    void MapObjectWizard(void);
    void MapObjectAdd(void);
    void MapObjectHovered(NodePath);
    void MapObjectFocus(MapObject*);
    void MapObjectGenerateWaypoints(void);

    // DYNAMICOBJECTS
    void DynamicObjectWizard(void);
    void DynamicObjectAdd(void);
    void DynamicObjectHovered(NodePath);
    void DynamicObjectSetWaypoint(DynamicObject*);
    void DynamicObjectFocus(DynamicObject*);
    void SelectWaypointFromObject(DynamicObject*);
    void CharacterAdd(void);

    // LIGHTS
    void LightAdd(void);
    void LightFocus(WorldLight*);

    // PARTICLES
    void ParticleObjectAdd(void);
    void ParticleObjectFocus(ParticleObject*);

    void SelectWaypointZone(float from_x, float from_y, float to_x, float to_y);

    void LevelListUpdated(void);

private:
    WindowFramework*         _window;
    Ui::MainWindow*          ui;
    TabScript                tabScript;
    TabDialog                tabDialog;
    TabL18n                  tabL18n;

public:
    DialogSplashscreen       splashScreen;
private:

    unsigned short           level_editor_lock;
    QString                  levelName;
    World*                   world;
    float                    wp_select_x, wp_select_y;
    std::list<Waypoint*>     waypointsSelection;
    Waypoint*                waypointSelected;
    Waypoint*                waypointHovered;
    float                    waypointSelX, waypointSelY, waypointSelZ;
    DialogWaypointGenerate   waypointGenerate;
    DialogSaveMap            dialogSaveMap;
    bool                     save_map_use_thread;

    QMenu                    level_add_object_menu;
    WizardObject             wizardObject;

    bool                     wizardMapObject;
    MapObject*               mapobjectSelected;
    MapObject*               mapobjectHovered;
    void                     MapObjectSelect(void);

    bool                     wizardDynObject;
    DynamicObject*           dynamicObjectSelected;
    DynamicObject*           dynamicObjectHovered;
    void                     DynamicObjectSelect(void);

    WorldLight*              lightSelected;
    bool                     lightIgnoreChanges;

    DialogNewMap             dialogNewMap;

    DialogObject             dialogObject;
    DataTree*                objectFile;

    WaypointGenerator*       wp_generator;
    Utils::Packet*           clipboard;
};

#endif // MAINWINDOW_H
