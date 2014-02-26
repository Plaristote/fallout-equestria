#ifndef  WORLD_WAYPOINT_HPP
# define WORLD_WAYPOINT_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <panda3d/collisionSegment.h>

# include "serializer.hpp"

struct World;
struct WorldLight;

struct Waypoint
{
    struct ArcObserver
    {
      virtual ~ArcObserver() {}
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

#endif
