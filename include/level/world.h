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

# ifndef TRUE_SQRT
#  define SQRT my_sqrt
# else
#  define SQRT sqrt
# endif

//# define GAME_EDITOR

# ifdef GAME_EDITOR
#  define WAYPOINT_DEBUG
#  include <QDir>
#  define MODEL_ROOT QDir::currentPath().toStdString() + "/models/"
#  define TEXT_ROOT  QDir::currentPath().toStdString() + "/textures/"
# else
#  define MODEL_ROOT "models/"
#  define TEXT_ROOT  "textures/"
# endif

float my_sqrt(const float x);

namespace ColMask
{
  enum
  {
      None       = 0,
      Waypoint   = 1,
      Tile       = 2,
      DynObject  = 4,
      Object     = 8,
      FovTarget  = 16,
      WpPlane    = 32
  };
}

struct World;

struct FBoundingBox
{
  float left;
  float top;
  float width;
  float height;

  bool  Intersects(float x, float y)
  {
    return (x >= left && x <= left + width && y >= top && y <= top + height);
  }
};

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

    unsigned int        id;
    unsigned char       floor;
    Arcs                arcs;
    ArcsWithdrawed      arcs_withdrawed;
    NodePath            nodePath;
    FBoundingBox        mouseBox;
    
    void WithdrawArc(Waypoint* other);
    void UnwithdrawArc(Waypoint* other, ArcObserver* observer);
    std::pair<Arc, unsigned short>* GetWithdrawable(Waypoint* other);

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

    // Loading
    void                 LoadArcs(void);
    void                 Unserialize(Utils::Packet& packet);
    void                 UnserializeLoadArcs(World*);
    void                 Serialize(Utils::Packet& packet);

    void                 SetMouseBox(void);

private:
    friend struct World;
    bool                 selected;
    // unserialize tmp
    std::vector<int>     tmpArcs;
};

struct MapObject
{
    NodePath      nodePath;
    PT(Texture)   texture;
    
    unsigned char floor;

    std::string   strModel;
    std::string   strTexture;

    void UnSerialize(WindowFramework* window, Utils::Packet& packet);
    void Serialize(Utils::Packet& packet);
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
  
  LColor GetColor(void) const
  {
    return (light->get_color());
  }

  void   SetColor(float r, float g, float b, float a)
  {
    LColor color(r, g, b, a);
    
    light->set_color(color);
  }

  bool operator==(const std::string& comp) { return (name == comp); }

  void UnSerialize(World*, Utils::Packet& packet);
  void Serialize(Utils::Packet& packet);
  
  void ReparentTo(DynamicObject* object)
  {
    parent_type = Type_DynamicObject;
    parent      = object->nodePath;
    parent_i    = object;
  }
  
  void ReparentTo(MapObject* object)
  {
    parent_type = Type_MapObject;
    parent      = object->nodePath;
    parent_i    = object;
  }
  
  std::string name;
  NodePath    nodePath;
  Type        type;
  ParentType  parent_type;
  PT(Light)   light;
  Lens*       lens;
  float       zoneSize;

  std::list<NodePath> enlightened;
private:
  void Initialize(void);
  NodePath    parent;
  MapObject*  parent_i;
};

struct World
{
    typedef std::vector<Waypoint>    Waypoints;
    typedef std::list<MapObject>     MapObjects;
    typedef std::list<DynamicObject> DynamicObjects;
    typedef std::list<WorldLight>    WorldLights;
    typedef std::list<ExitZone>      ExitZones;
    typedef std::list<EntryZone>     EntryZones;
    typedef std::vector<NodePath>    Floors;

    WindowFramework* window;
    
    Floors           floors;

    NodePath         rootWaypoints;
    Waypoints        waypoints;

    NodePath         rootMapObjects;
    MapObjects       objects;

    NodePath         rootDynamicObjects;
    DynamicObjects   dynamicObjects;
    
    NodePath         rootLights;
    WorldLights      lights;
    
    ExitZones        exitZones;
    EntryZones       entryZones;

    World(WindowFramework* window);
    ~World(void);
    
    void      FloorResize(int);

    Waypoint* AddWayPoint(float x, float y, float z);
    void      DeleteWayPoint(Waypoint*);
    Waypoint* GetWaypointFromNodePath(NodePath path);
    Waypoint* GetWaypointFromId(unsigned int id);
    Waypoint* GetWaypointClosest(LPoint3);
    void      SetWaypointsVisible(bool v)
    { if (v) { rootWaypoints.show();  } else { rootWaypoints.hide();  } }
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
    OBJTYPE*       GetObjectFromNodePath(NodePath path, std::list<OBJTYPE>& list)
    {
        typename std::list<OBJTYPE>::iterator it  = list.begin();
        typename std::list<OBJTYPE>::iterator end = list.end();

	if (path.is_empty())
	  return (0);
        for (; it != end ; ++it)
        {
	  if ((*it).nodePath.is_empty())
	    continue ;
          if ((*it).nodePath.is_ancestor_of(path))
            return (&(*it));
        }
        return (0);
    }

    void           ObjectChangeFloor(MapObject&, unsigned char floor, unsigned short type);

    MapObject*     AddMapObject(const std::string& name, const std::string& model, const std::string& texture, float x, float y, float z);
    void           DeleteMapObject(MapObject*);
    MapObject*     GetMapObjectFromName(const std::string& name);
    MapObject*     GetMapObjectFromNodePath(NodePath path);
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
    
    void           AddExitZone(const std::string&);
    void           DeleteExitZone(const std::string&);
    ExitZone*      GetExitZoneByName(const std::string&);
    
    void           AddEntryZone(const std::string&);
    void           DeleteEntryZone(const std::string&);
    EntryZone*     GetEntryZoneByName(const std::string&);
    
    void           AddLight(WorldLight::Type, const std::string&);
    void           AddLight(WorldLight::Type, const std::string&, MapObject* parent);
    void           AddLight(WorldLight::Type, const std::string&, DynamicObject* parent);
    void           DeleteLight(const std::string&);
    WorldLight*    GetLightByName(const std::string&);
    void           CompileLight(WorldLight*, unsigned char = ColMask::Object | ColMask::DynObject);
    
    Waypoint*      GetWaypointAt(LPoint2f);

    void           UnSerialize(Utils::Packet& packet);
    void           Serialize(Utils::Packet& packet);

    void           CompileWaypoints(void);
    void           CompileDoors(void);
    
    static NodePath model_sphere;
};

#endif // WORLD_H

class BoundingBox;
