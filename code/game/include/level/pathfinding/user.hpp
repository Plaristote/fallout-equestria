#ifndef  PATHFINDING_USER_HPP
# define PATHFINDING_USER_HPP

# include "globals.hpp"
# include "level/pathfinding/path.hpp"
# include "level/pathfinding/collider.hpp"
# include "level/objects/instance_dynamic_object.hpp"
# include "level/path_preview.hpp"

namespace Pathfinding
{
  class User : public InstanceDynamicObject
  {
    struct Destination
    {
      Destination() : object(0), min_distance(0) {}

      InstanceDynamicObject* object;
      unsigned short         min_distance;
    };

    //typedef Pathfinding::Path Path;
    typedef PathPreview       Path;

  public:
    Sync::Signal<void>       ReachedDestination;
    Sync::Signal<void>       MovedFor1ActionPoint;

    User(Level* level, DynamicObject* object);

    virtual void             Run(float elapsed_time);
    
    void                     GoTo(Waypoint*);
    void                     GoTo(InstanceDynamicObject* target, int max_distance = 0);
    void                     GoToRandomDirection();
    unsigned short           GetPathDistance(Pathfinding::Collider*);
    unsigned short           GetPathDistance(Waypoint*);
    void                     TeleportTo(Waypoint*);
    void                     TruncatePath(unsigned short max_size);
    void                     SetPathVisible(bool set);
    
    void                     LookAt(LVecBase3);
    void                     LookAt(InstanceDynamicObject*);

    bool                     IsMoving(void) const;
    Path&                    GetPath(void)        { return (path);            }
    const Path&              GetPath(void)  const { return (path);            }

    virtual void             Serialize(Utils::Packet&);
    virtual void             Unserialize(Utils::Packet&);

    void                     StartRunAnimation();
  protected:
    void                     SetMovementAnimation(const std::string& name) { movement_animation = name;  }
    void                     SetMovementSpeed(float speed)                 { movement_speed     = speed; }
    
    virtual bool             HasLineOfSight(InstanceDynamicObject*) = 0;

  private:
    void                     RunMovement    (float elapsed_time);
    void                     RunRotate      (float elapsed_time);
    void                     RunNextMovement(float elapsed_time);
    void                     SetNextWaypoint(void);
    void                     MovePathForward(void);
    void                     FindNewWayOrAbort(void);
    bool                     GoThroughNextWaypoint(void);
    LPoint3                  GetDistanceToNextWaypoint(void) const;
    LPoint3                  GetDistanceToWaypoint(const Waypoint&) const;
    const Waypoint*          GetNextWaypoint(void) const;
    bool                     IsDistanceNull(LPoint3f) const;
    bool                     HasReachedTarget(void);
    void                     TriggerDestinationReached(void);
    
    Path                     path;
    std::string              movement_animation;
    float                    movement_speed;
    Destination              current_target;
    float                    rotation_goal;
    bool                     path_visible;
  };
}

#endif
