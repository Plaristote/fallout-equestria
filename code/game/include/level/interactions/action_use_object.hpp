#ifndef  INTERACTIONS_ACTIONS_USE_OBJECT_HPP
# define INTERACTIONS_ACTIONS_USE_OBJECT_HPP

# include "action_runner.hpp"
# include "level/inventory.hpp"

namespace Interactions
{
  namespace Actions
  {
    class UseObject : public ActionRunner
    {
      UseObject(ObjectCharacter* character, InventoryObject* object, unsigned char actionIt);

      virtual void RunAction();
      InventoryObject* item;
      unsigned char    action_it;
    public:
      static ActionRunner* Factory(ObjectCharacter* character, InventoryObject* object, unsigned char action_it);
    };

    class UseObjectOn : public ActionRunner
    {
      UseObjectOn(ObjectCharacter* character, InstanceDynamicObject* target, InventoryObject* object, unsigned char action_it);
      virtual void RunAction();

      InventoryObject* item;
      unsigned char    action_it;
    public:
      static ActionRunner* Factory(ObjectCharacter* character, InstanceDynamicObject* target, InventoryObject* object, unsigned char action_it);
    };    
  }
}

#endif