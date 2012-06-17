  #ifndef  OBJECT_NODE_HPP
# define OBJECT_NODE_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/animControlCollection.h>
# include "datatree.hpp"
# include "observatory.hpp"
# include "world.h"
# include "inventory.hpp"

//HAIL MICROSOFT
#ifdef WIN32
static inline double round(double val)
{
    return floor(val + 0.5);
}
#endif

class Level;

struct WaypointModifier
{  
  virtual void ProcessCollisions(void);
  void         UnprocessCollisions(void);
  int          GetOccupiedWaypointAsInt(void) const { return (_waypointOccupied->id); }
  Waypoint*    GetOccupiedWaypoint(void)     const  { return (_waypointOccupied);     }
  void         SetOccupiedWaypoint(Waypoint* wp)    { _waypointOccupied = wp;         }

protected:
  void         WithdrawAllArcs(unsigned int id);
  void         WithdrawAllArcs(Waypoint* waypoint);
  void         WithdrawArc(unsigned int id1, unsigned int id2);
  void         WithdrawArc(Waypoint* first, Waypoint* second);

  Level*                          _level;
  Waypoint*                       _waypointOccupied;
  std::list<std::pair<int, int> > _waypointDisconnected;

private:
  struct WithdrawedArc
  {
    WithdrawedArc(Waypoint* first, Waypoint* second, Waypoint::ArcObserver* observer) : first(first), second(second), observer(observer) {}
    Waypoint              *first, *second;
    Waypoint::ArcObserver *observer;
  };
  typedef std::list<WithdrawedArc>        WithdrawedArcs;

  WithdrawedArcs                          _withdrawedArcs;
};

enum ObjectType
{
  Character, Door, Shelf, Locker, Item, Other
};

template<class C>
struct ObjectType2Code { enum { Type = ObjectType::Other }; };

class InstanceDynamicObject : public WaypointModifier
{
public:
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionUse;
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionUseObjectOn;
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionUseSkillOn;
  static Observatory::Signal<void (InstanceDynamicObject*)> ActionTalkTo;

  struct Interaction
  {
    Interaction(const std::string& name, InstanceDynamicObject* instance, Observatory::Signal<void (InstanceDynamicObject*)>* signal)
    : name(name), instance(instance), signal(signal) {}

    std::string                                         name;
    InstanceDynamicObject*                              instance;
    Observatory::Signal<void (InstanceDynamicObject*)>* signal;
  };
  typedef std::list<Interaction> InteractionList;
  
  struct GoToData
  {
    Waypoint*              nearest;
    InstanceDynamicObject* objective;
    int                    max_distance;
    int                    min_distance;
  };
  
  InstanceDynamicObject(Level* level, DynamicObject* object);
  virtual ~InstanceDynamicObject() {}

  virtual void       Run(float elapsedTime)
  {
    TaskAnimation();
  };

  bool               operator==(NodePath np)             const { return (_object->nodePath.is_ancestor_of(np)); }
  bool               operator==(const std::string& name) const { return (GetName() == name);                    }
  std::string        GetName(void)                       const { return (_object->nodePath.get_name());         }
  NodePath           GetNodePath(void)                   const { return (_object->nodePath);                    }
  InteractionList&   GetInteractions(void)                     { return (_interactions);                        }
  const std::string& GetDialog(void)                     const { return (_object->dialog);                      }
  DynamicObject*     GetDynamicObject(void)                    { return (_object);                              }
  virtual GoToData   GetGoToData(InstanceDynamicObject* character);
  
  template<class C>
  C*                 Get(void)
  {
    if (ObjectType2Code<C>::Type == _type)
      return (reinterpret_cast<C*>(this));
    return (0);
  }

  bool                     pendingAnimationDone;
  InstanceDynamicObject*   pendingActionOn;
  InventoryObject*         pendingActionObject;

  virtual void             CallbackActionUse(InstanceDynamicObject* object) { ThatDoesNothing(); }
  
  void                     PlayAnimation(const std::string& name, bool loop = false);
  Observatory::Signal<void (InstanceDynamicObject*)> AnimationEnded;

protected:
  unsigned char            _type;
  DynamicObject*           _object;
  
  // Interactions
  void                     ResetInteractions(void) { _interactions.clear(); }
  void                     ThatDoesNothing();

  InteractionList          _interactions;

  // Animations
  void                     LoadAnimation(const std::string& name);
  void                     TaskAnimation(void);
  
  std::string               _modelName;
  AnimControlCollection     _anims;
  AnimControl*              _anim;
};


#endif