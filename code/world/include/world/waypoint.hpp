#ifndef  WORLD_WAYPOINT_HPP
# define WORLD_WAYPOINT_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <panda3d/collisionSegment.h>

# include "serializer.hpp"

struct World;
struct WorldLight;
struct MapObject;

struct Waypoint
{
  struct ArcObserver
  {
    virtual ~ArcObserver() {}
    virtual bool CanGoThrough(Waypoint* from, Waypoint* to, void* object) = 0;
    virtual void GoingThrough(Waypoint* from, Waypoint* to, void* object) = 0;
  };

  struct Arc
  {
    Arc(Waypoint* from, Waypoint* to);
    Arc(const Arc&);
    ~Arc();

    bool operator==(Waypoint* other) { return (to == other); }
    void UpdateDirection(void);
    void Destroy(void);

    void SetVisible(bool);
    bool CanGoThrough(void* object);
    void GoingThrough(void* object);

    PT(CollisionSegment) csegment;
    PT(CollisionNode)    node;
    NodePath             nodePath;
    Waypoint*            from;
    Waypoint*            to;

    ArcObserver*         observer;
  };

  typedef std::list<Arc>                               Arcs;
  typedef std::vector<std::pair<Arc, unsigned short> > ArcsWithdrawed;

  unsigned int           id;
  unsigned char          floor;
  unsigned char          suggested_floor_above;
  unsigned char          floor_above;
  Arcs                   arcs;
  ArcsWithdrawed         arcs_withdrawed;
  NodePath               nodePath;
  std::list<WorldLight*> lights;
  NodePath               parent_path;

  void                            WithdrawArc(Waypoint* other);
  void                            UnwithdrawArc(Waypoint* other);
  std::pair<Arc, unsigned short>* GetWithdrawable(Waypoint* other);
  Waypoint*                       GetRandomWaypoint(void) const;

  Waypoint(NodePath root);
  Waypoint(void) {}

  bool                 operator==(const Waypoint& other) const;
  bool                 operator==(const Waypoint* other) const;
  bool                 operator==(unsigned int id)       const { return (this->id == id); }
  bool                 operator<(const Waypoint& other)  const { return (id < other.id); }
  bool                 operator>(const Waypoint& other)  const { return (id > other.id); }
  Arcs::iterator       Connect(Waypoint* other, ArcObserver* observer = 0);
  Arcs::iterator       Disconnect(Waypoint* other);
  void                 DisconnectAll(void);
  Arc*                 GetArcTo(unsigned int id);
  void                 PositionChanged(void);
  void                 UpdateArcDirection(Waypoint*);
  void                 SetSelected(bool);
  bool                 IsSelected(void) const { return (selected); }
  void                 SetLight(WorldLight* light, bool is_active);

  void                 SetArcsVisible(bool);

  // Pathfinding features
  float                GoalDistanceEstimate(const Waypoint& goal) const { return (GetDistanceEstimate(goal)); }
  float                GetDistanceEstimate(const Waypoint& other) const;
  float                GetDistanceEstimate(const LPoint3f other) const;
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

#endif
