#ifndef  INTERACTIONS_ACTION_DROP_OBJECT_HPP
# define INTERACTIONS_ACTION_DROP_OBJECT_HPP

# include "level/interactions/action_runner.hpp"

class InventoryObject;
class Waypoint;
class ObjectCharacter;

namespace Interactions
{
  namespace Actions
  {
    class DropObject : public ActionRunner
    {
      InventoryObject* item;
      ::Waypoint*      drop_on;
      
      DropObject(ObjectCharacter* character, InventoryObject* item, ::Waypoint* drop_on);
      
      virtual void RunAction(void);
    public:
      static ActionRunner* Factory(ObjectCharacter* character, InventoryObject* object);
    };
  }
}

#endif