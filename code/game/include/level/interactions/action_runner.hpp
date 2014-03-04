#ifndef  INTERACTION_RUNNER_HPP
# define INTERACTION_RUNNER_HPP

# include "globals.hpp"
# include "observatory.hpp"

class InstanceDynamicObject;
class ObjectCharacter;
class Level;
class Waypoint;

namespace Interactions
{
  class ActionRunner
  {
    union Target
    {
      InstanceDynamicObject* object;
      ::Waypoint*            waypoint;
    };
  public:
    enum TargetType
    {
      Self,
      Waypoint,
      Character,
      Object
    };
    
    ActionRunner(ObjectCharacter* user);
    virtual ~ActionRunner(void);

    void                   Run(void);

    void                   SetActionPointCost(unsigned short value)  { action_point_cost = value; }
    void                   SetAnimationName(const std::string& name) { animation_name    = name;  }
    void                   SetRange(unsigned int range)              { this->range       = range; }
    ObjectCharacter*       GetUser(void)   const                     { return (user);             }
    InstanceDynamicObject* GetObjectTarget(void) const               { return (target.object);    }
    ::Waypoint*            GetWaypointTarget(void) const             { return (target.waypoint);  }
    void                   SetTarget(InstanceDynamicObject* o)       { target.object   = o;       }
    void                   SetTarget(::Waypoint* w)                  { target.waypoint = w;       }
    void                   SetTargetType(TargetType type)            { target_type     = type;    }
    void                   SetTargetType(const std::string&);
    TargetType             GetTargetType(void) const                 { return (target_type);      }

  protected:
    static void            ConsoleWrite(ObjectCharacter* user, const std::string& message);
    bool                   CheckAndRemoveActionPoints(void);
    Level*                 GetLevel(void) const;
    void                   PickObject(void);
    void                   PickWaypoint(void);

    virtual void           ReachTarget(void);
    virtual void           PlayAnimation(void);
    virtual void           RunAction(void);

    Sync::ObserverHandler  observers;
  private:
    ObjectCharacter*       user;
    TargetType             target_type;
    Target                 target;
    unsigned int           range;
    unsigned int           action_point_cost;
    std::string            animation_name;
  };
}

#endif