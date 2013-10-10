#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <panda3d/cmath.h>
#include <QMainWindow>
#include "qpandaapplication.h"
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
#include "world.h"
#include "tabscript.h"
#include "tabdialog.h"
#include "tabl18n.h"
#include "serializer.hpp"
#include "dialogsavemap.h"

namespace Ui {
class MainWindow;
}

class WindowFramework;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QPandaApplication* app, QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent*);

    void LoadAllMaps(void);
    void LoadAllStatsheets(void);

    void  Copy(Utils::Packet&);
    void  Paste(Utils::Packet&);

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
    void UpdateCameraZoom(int);
    void CopyClicked(void);
    void PasteClicked(void);

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
    void UpdateSelection(void);
    void WaypointDiscardSelection(void);
    void WaypointSyncTerrain(void);
    void TerrainPickerPicked(bool);
    void TerrainSelectorPicked(bool);

    // ENTRY/EXIT ZONES
    void EntryZoneAdd(void);
    void EntryZoneDelete(void);
    void EntryZoneSetAsSelection(void);
    void EntryZoneChanged(QString);

    void ExitZoneAdd(void);
    void ExitZoneDelete(void);
    void ExitZoneSetAsSelection(void);
    void ExitZoneChanged(QString);
    void ExitZoneDestinationAdd(void);
    void ExitZoneDestinationDelete(void);

    // OBJECTS
    void ObjectAdd(void);

    // MAPOBJECTS
    void MapObjectWizard(void);
    void MapObjectAdd(void);
    void MapObjectDelete(void);
    void MapObjectHovered(NodePath);
    void MapObjectNameChanged(QString);
    void MapObjectUpdateX(void);
    void MapObjectUpdateY(void);
    void MapObjectUpdateZ(void);
    void MapObjectRotationX(void);
    void MapObjectRotationY(void);
    void MapObjectRotationZ(void);
    void MapObjectScaleX(void);
    void MapObjectScaleY(void);
    void MapObjectScaleZ(void);
    void MapObjectColliderUpdatePos(void);
    void MapObjectColliderUpdateType(void);
    void MapObjectFloor(void);
    void MapObjectFocus(MapObject*);
    void MapObjectGenerateWaypoints(void);

    // DYNAMICOBJECTS
    void DynamicObjectVisible(void);
    void DynamicObjectWizard(void);
    void DynamicObjectAdd(void);
    void DynamicObjectDelete(void);
    void DynamicObjectHovered(NodePath);
    void DynamicObjectNameChanged(QString);
    void DynamicObjectUpdateX(void);
    void DynamicObjectUpdateY(void);
    void DynamicObjectUpdateZ(void);
    void DynamicObjectRotationX(void);
    void DynamicObjectRotationY(void);
    void DynamicObjectRotationZ(void);
    void DynamicObjectScaleX(void);
    void DynamicObjectScaleY(void);
    void DynamicObjectScaleZ(void);
    void DynamicObjectSetWaypoint(void);
    void DynamicObjectFocus(DynamicObject*);

    // LIGHTS
    void LightVisible(void);
    void LightSetEnabled(void);
    void LightSetDisabled(void);
    void LightSelected(void);
    void LightAdd(void);
    void LightDelete(void);
    void LightUpdatePosition(void);
    void LightUpdateType(void);
    void LightCompile(void);
    void LightFocus(WorldLight*);

    void SelectWaypointZone(float from_x, float from_y, float to_x, float to_y);


private:
    QPandaApplication&       _app;
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
