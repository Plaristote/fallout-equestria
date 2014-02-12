#ifndef WORLD_H
# define WORLD_H

# include "globals.hpp"

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/collisionSphere.h>
# include <panda3d/collisionSegment.h>
# include <panda3d/collisionTube.h>
# include <panda3d/collisionNode.h>
# include <panda3d/collisionHandlerQueue.h>
# include <panda3d/collisionTraverser.h>

# include <algorithm>
# include "serializer.hpp"

# include "divide_and_conquer.hpp"

# include "is_game_editor.h"

# ifdef GAME_EDITOR
#  define WAYPOINT_DEBUG
#  include <QDir>
#  define MODEL_ROOT QDir::currentPath().toStdString() + "/models/"
#  define TEXT_ROOT  QDir::currentPath().toStdString() + "/textures/"
# else
#  define MODEL_ROOT "models/"
#  define TEXT_ROOT  "textures/"
# endif

# ifdef _WIN32 // Workaround for broken CollisionNode stuff on Windows
#  define new_CollisionNode(str) CollisionNode::Factory(str);
# else
#  define new_CollisionNode(str) new CollisionNode(str);
# endif

// Following c functions are implemented in level/world/misc.cpp
LPoint3  NodePathSize(NodePath np);
void     SetCollideMaskOnSingleNodepath(NodePath np, unsigned short collide_mask);
LPoint3f operator*(LPoint3f,LPoint3f);


#include "world/colmask.hpp"
#include "world/interactions.hpp"

#include "world/waypoint.hpp"
#include "world/map_object.hpp"
#include "world/dynamic_object.hpp"
#include "world/light.hpp"
#include "world/zone.hpp"

struct World
{
#ifndef GAME_EDITOR
    typedef std::vector<Waypoint>    Waypoints;
#else
    typedef std::list<Waypoint>      Waypoints;
#endif
    typedef std::list<MapObject>     MapObjects;
    typedef std::list<DynamicObject> DynamicObjects;
    typedef std::list<WorldLight>    WorldLights;
    typedef std::list<ExitZone>      ExitZones;
    typedef std::list<EntryZone>     EntryZones;
    typedef std::vector<NodePath>    Floors;

    WindowFramework* window;

    NodePath         floors_node;
    Floors           floors;

    NodePath         rootWaypoints;
    Waypoints        waypoints;

    NodePath         rootMapObjects;
    MapObjects       objects;

    NodePath         rootDynamicObjects;
    DynamicObjects   dynamicObjects;

    NodePath         rootLights;
    WorldLights      lights;
    bool             sunlight_enabled;
#ifdef GAME_EDITOR
    NodePath         lightSymbols;
    bool             do_compile_doors;
    bool             do_compile_waypoints;
#endif

    ExitZones        exitZones;
    EntryZones       entryZones;

    World(WindowFramework* window);
    ~World(void);

    void      FloorResize(int);

    Waypoint*      AddWayPoint(float x, float y, float z);
    void           DeleteWayPoint(Waypoint*);
    Waypoint*      GetWaypointFromNodePath(NodePath path);
    Waypoint*      GetWaypointFromId(unsigned int id);
    Waypoint*      GetWaypointClosest(LPoint3, unsigned char floor);
    void           SetWaypointsVisible(bool v);
    void           GetWaypointLimits(short currentFloor, LPoint3& upperRight, LPoint3& upperLeft, LPoint3& bottomLeft) const;

    template<class OBJTYPE>
    void           DeleteObject(MapObject* ptr, std::list<OBJTYPE>& list)
    {
        typename std::list<OBJTYPE>::iterator it  = list.begin();
        typename std::list<OBJTYPE>::iterator end = list.end();

        for (; it != end ; ++it)
        {
            if (&(*it) == ptr)
            {
                ptr->nodePath.remove_node();
                list.erase(it);
                break ;
            }
        }
    }

    template<class OBJTYPE>
    OBJTYPE*       GetObjectFromName(const std::string& name, std::list<OBJTYPE>& list)
    {
        typename std::list<OBJTYPE>::iterator it  = list.begin();
        typename std::list<OBJTYPE>::iterator end = list.end();

        for (; it != end ; ++it)
        {
          if ((*it).nodePath.get_name() == name)
            return (&(*it));
        }
        return (0);
    }

    template<class OBJTYPE>
    OBJTYPE*       GetObjectFromNodePath(NodePath path, std::list<OBJTYPE>& list, const std::string& parent = "")
    {
      typename std::list<OBJTYPE>::iterator it  = list.begin();
      typename std::list<OBJTYPE>::iterator end = list.end();

      if (path.is_empty())
        return (0);
      for (; it != end ; ++it)
      {
        if (parent == "" || parent == it->parent)
        {
          if ((!(*it).nodePath.is_empty() && (*it).nodePath.is_ancestor_of(path)))
          {
            OBJTYPE* children = GetObjectFromNodePath(path, list, it->nodePath.get_name());

            return (children ? children : &(*it));
          }
        }
      }
      return (0);
    }

    void           ObjectChangeFloor(MapObject&, unsigned char floor, unsigned short type);

    MapObject*     AddMapObject(const std::string& name, const std::string& model, const std::string& texture, float x, float y, float z);
    void           DeleteMapObject(MapObject*);
    MapObject*     GetMapObjectFromName(const std::string& name);
    MapObject*     GetMapObjectFromNodePath(NodePath path);
    MapObject*     GetMapObjectFromCollisionNode(NodePath path);
    void           SetMapObjectsVisible(bool v);
    void           MapObjectChangeFloor(MapObject&, unsigned char floor);

    DynamicObject* InsertDynamicObject(DynamicObject&);
    DynamicObject* AddDynamicObject(const std::string& name, DynamicObject::Type type, const std::string& model, const std::string& texture);
    void           DeleteDynamicObject(DynamicObject*);
    DynamicObject* GetDynamicObjectFromName(const std::string& name);
    DynamicObject* GetDynamicObjectFromNodePath(NodePath path);
    void           SetDynamicObjectsVisible(bool v);
    void           DynamicObjectSetWaypoint(DynamicObject&, Waypoint&);
    void           DynamicObjectChangeFloor(DynamicObject&, unsigned char floor);

    void           ReparentObject(MapObject* object, MapObject*     new_parent);
    void           ReparentObject(MapObject* object, const std::string& name);

    void           AddExitZone(const std::string&);
    void           DeleteExitZone(const std::string&);
    ExitZone*      GetExitZoneByName(const std::string&);
    bool           IsInExitZone(unsigned int id) const;

    void           AddEntryZone(const std::string&);
    void           DeleteEntryZone(const std::string&);
    EntryZone*     GetEntryZoneByName(const std::string&);

    void           AddLight(WorldLight::Type, const std::string&);
    void           AddLight(WorldLight::Type, const std::string&, MapObject* parent);
    void           AddLight(WorldLight::Type, const std::string&, DynamicObject* parent);
    void           DeleteLight(const std::string&);
    WorldLight*    GetLightByName(const std::string&);
    void           CompileLight(WorldLight*, unsigned char = ColMask::Object | ColMask::DynObject);

    typedef std::function<void (const std::string&, float)> ProgressCallback;

    void           UnSerialize(Utils::Packet& packet);
    void           Serialize(Utils::Packet& packet, std::function<void (const std::string&, float)> progress_callback = [](const std::string&, float){});

    void           UpdateMapTree(void);
    void           CompileWaypoints(ProgressCallback);
    void           CompileDoors(ProgressCallback);

    NodePath       debug_pathfinding;
    NodePath       model_sphere;

    DivideAndConquer::Graph<Waypoint, LPoint3f> waypoint_graph;
};

#endif // WORLD_H
