#ifndef WORLD_H
# define WORLD_H

# include "globals.hpp"

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/nodePathCollection.h>
# include <panda3d/collisionSphere.h>
# include <panda3d/collisionSegment.h>
# include <panda3d/collisionTube.h>
# include <panda3d/collisionNode.h>
# include <panda3d/collisionHandlerQueue.h>
# include <panda3d/collisionTraverser.h>

# include <panda3d/pointLight.h>
# include <panda3d/directionalLight.h>
# include <panda3d/ambientLight.h>
# include <panda3d/spotlight.h>

# include <algorithm>
# include "serializer.hpp"

# include "divide_and_conquer.hpp"

# ifndef TRUE_SQRT
#  define SQRT my_sqrt
# else
#  define SQRT sqrt
# endif

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

float   my_sqrt(const float x);
LPoint3 NodePathSize(NodePath np);
void    SetCollideMaskOnSingleNodepath(NodePath np, unsigned short collide_mask);

namespace ColMask
{
  enum
  {
      None                  = 0,
      Waypoint              = 1,
      DynObject             = 2,
      Object                = 4,
      FovTarget             = 8,
      WpPlane               = 16,
      FovBlocker            = 32,
      CheckCollisionOnModel = 64
  };
}

struct World;
struct WorldLight;

struct Waypoint
{
    struct ArcObserver
    {
      virtual bool CanGoThrough(unsigned char type) = 0;
      virtual void GoingThrough(void*)              = 0;
    };

    struct Arc
    {
        Arc(NodePath from, Waypoint* to);
        Arc(const Arc&);
    ~Arc();

        bool operator==(Waypoint* other) { return (to == other); }
        void UpdateDirection(void);
        void Destroy(void);

        void SetVisible(bool);

        PT(CollisionSegment) csegment;
        PT(CollisionNode)    node;
        NodePath             nodePath;
        NodePath             from;
        Waypoint*            to;

        ArcObserver*         observer;
    };

    typedef std::list<Arc> Arcs;
    typedef std::vector<std::pair<Arc, unsigned short> > ArcsWithdrawed;

    unsigned int           id;
    unsigned char          floor;
    Arcs                   arcs;
    ArcsWithdrawed         arcs_withdrawed;
    NodePath               nodePath;
    std::list<WorldLight*> lights;

    void WithdrawArc(Waypoint* other);
    void UnwithdrawArc(Waypoint* other, ArcObserver* observer);
    std::pair<Arc, unsigned short>* GetWithdrawable(Waypoint* other);
    Waypoint*                       GetRandomWaypoint(void) const;

    Waypoint(NodePath root);
    Waypoint(void) {}

    bool                 operator==(const Waypoint& other) const;
    bool                 operator==(const Waypoint* other) const;
    bool                 operator==(unsigned int id)       const { return (this->id == id); }
    bool                 operator<(const Waypoint& other)  const { return (id < other.id); }
    bool                 operator>(const Waypoint& other)  const { return (id > other.id); }
    Arcs::iterator       ConnectUnsafe(Waypoint* other);
    Arcs::iterator       Connect(Waypoint* other);
    Arcs::iterator       Disconnect(Waypoint* other);
    void                 DisconnectAll(void);
    Arc*                 GetArcTo(unsigned int id);
    void                 PositionChanged(void);
    void                 UpdateArcDirection(Waypoint*);
    void                 SetSelected(bool);
    bool                 IsSelected(void) const { return (selected); }

    void                 SetArcsVisible(bool);

    // Pathfinding features
    float                GoalDistanceEstimate(const Waypoint& goal) const { return (GetDistanceEstimate(goal)); }
    float                GetDistanceEstimate(const Waypoint& other) const;
    std::list<Waypoint*> GetSuccessors(Waypoint* parent);
    float                GetCost(Waypoint&) { return (1.f); }
    // Divide and conquer
    LPoint3f             GetPosition(void) const { return (nodePath.get_pos()); }

    // Loading
    void                 LoadArcs(void);
    void                 Unserialize(Utils::Packet& packet);
    void                 UnserializeLoadArcs(World*);
    void                 Serialize(World*, Utils::Packet& packet);

private:
    friend struct World;
    bool                 selected;
    // unserialize tmp
    std::vector<int>     tmpArcs;
};

struct MapObject
{
  typedef std::vector<Waypoint*> Waypoints;

  enum Collider
  {
    NONE,
    MODEL,
    BOX,
    SPHERE
  };

  NodePath      nodePath, render, collision_node;
  PT(Texture)   texture;
  unsigned char floor;
  Waypoints     waypoints;
  NodePath      waypoints_root;

  std::string   strModel;
  std::string   strTexture;
  std::string   parent;
  Collider      collider;

  void          SetFloor(unsigned char floor);
  void          ReparentTo(MapObject* object);

  void          UnSerialize(World* world, Utils::Packet& packet);
  void          UnserializeWaypoints(World*, Utils::Packet& packet);
  void          Serialize(Utils::Packet& packet);
  static void   InitializeTree(World* world);
  void          InitializeCollider(Collider type, LPoint3f position, LPoint3f scale, LPoint3f hpr);
};

namespace Interactions
{
  enum
  {
      None      = 0,
      Use       = 1,
      UseSkill  = 2,
      UseObject = 4,
      TalkTo    = 8
  };
}

struct DynamicObject : public MapObject
{
    enum Type
    {
        Door,
        Shelf,
        Locker,
        Character,
    Item
    };

    Waypoint* waypoint;
    Type      type;

    // All
    std::string          script;

    // Door / Locker
    bool                            locked;
    std::string                     key; // Also used to store item names for DynamicObject::Item
    std::list<std::pair<int, int> > lockedArcs;

    // Shelf / Character
    std::list<std::pair<std::string, int> > inventory;

    // Character
    std::string          charsheet;

    // Interactions
    char                 interactions;
    std::string          dialog;

    void UnSerialize(World*, Utils::Packet& packet);
    void Serialize(Utils::Packet& packet);
};

struct Zone
{
  bool operator==(const std::string& comp) { return (name == comp); }

  bool Contains(unsigned int id) const
  {
    auto      it = waypoints.begin(), end = waypoints.end();

    for (; it != end ; ++it)
    {
      if ((*it)->id == id)
        return (true);
    }
    return (false);
  }

  bool Contains(Waypoint* wp) const { return (Contains(wp->id)); }

  std::string          name;
  std::list<Waypoint*> waypoints;
};

struct ExitZone : public Zone
{
  std::list<std::string> destinations;
};

typedef Zone EntryZone;

struct WorldLight
{
  enum Type
  {
    Point,
    Directional,
    Ambient,
    Spot
  };

  enum ParentType
  {
    Type_None,
    Type_MapObject,
    Type_DynamicObject
  };

  WorldLight(Type type, ParentType ptype, NodePath parent, const std::string& name) : name(name), type(type), parent_type(ptype), parent(parent), parent_i(0)
  {
    Initialize();
  }

  WorldLight(NodePath parent) : parent(parent), parent_i(0) {}

  void   SetEnabled(bool);
  void   Destroy(void);

  LColor GetColor(void) const
  {
    return (light->get_color());
  }

  void   SetColor(float r, float g, float b, float a)
  {
    LColor color(r, g, b, a);

    light->set_color(color);
  }

  LVecBase3f GetAttenuation(void) const
  {
    switch (type)
    {
      case Point:
      {
        PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

        return (point_light->get_attenuation());
      }
      case Spot:
      {
        PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

        return (spot_light->get_attenuation());
      }
      default:
          break;
    }
    return (LVecBase3f(0, 0, 0));
  }

  void   SetAttenuation(float a, float b, float c)
  {
    switch (type)
    {
      case Point:
      {
        PT(PointLight) point_light = reinterpret_cast<PointLight*>(light.p());

        point_light->set_attenuation(LVecBase3(a, b, c));
        break ;
      }
      case Spot:
      {
        PT(Spotlight) spot_light = reinterpret_cast<Spotlight*>(light.p());

        spot_light->set_attenuation(LVecBase3(a, b, c));
        break ;
      }
      default:
        break ;
    }
  }

  void   SetPosition(LPoint3 position)
  {
      nodePath.set_pos(position);
#ifdef GAME_EDITOR
      symbol.set_pos(position);
#endif
  }

  bool operator==(const std::string& comp) { return (name == comp); }

  void UnSerialize(World*, Utils::Packet& packet);
  void Serialize(Utils::Packet& packet);

  void ReparentTo(World* world);

  void ReparentTo(DynamicObject* object)
  {
    ReparentTo((MapObject*)object);
    parent_type = Type_DynamicObject;
  }

  void ReparentTo(MapObject* object)
  {
    parent_type = Type_MapObject;
    parent      = object->nodePath;
    parent_i    = object;
    nodePath.reparent_to(parent);
#ifdef GAME_EDITOR
    symbol.reparent_to(parent);
#endif
  }

  MapObject*  Parent(void) const
  {
      return (parent_i);
  }

  std::string name;
  NodePath    nodePath;
  Type        type;
  ParentType  parent_type;
  PT(Light)   light;
  Lens*       lens;
  float       zoneSize;
  bool        enabled;

  std::list<NodePath> enlightened;
#ifdef GAME_EDITOR
  NodePath    symbol;
#endif
private:
  void Initialize(void);
  NodePath    parent;
  MapObject*  parent_i;
};

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
    LPlane         GetWaypointPlane(short currentFloor) const;

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

    static NodePath model_sphere;

    DivideAndConquer::Graph<Waypoint, LPoint3f> waypoint_graph;
};

#endif // WORLD_H
