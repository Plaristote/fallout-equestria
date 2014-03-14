#include "level/interactions/action_use_object.hpp"
#include "level/objects/character.hpp"
#define AP_COST_USE 2

using namespace std;
using namespace Interactions;

Actions::UseObject::UseObject(ObjectCharacter* character, InventoryObject* object, unsigned char actionIt) : ActionRunner(character), item(object), action_it(actionIt)
{
  SetActionPointCost(item->GetActionPointCost(character, action_it));
  SetAnimationName("use");
  SetTargetType(ActionRunner::Self);
}

void Actions::UseObject::RunAction()
{
  item->Use(GetUser(), action_it);
}

void Actions::UseObjectOn::RunAction()
{
  item->UseOn(GetUser(), GetObjectTarget(), action_it);
}

Actions::UseObjectOn::UseObjectOn(ObjectCharacter* character, InstanceDynamicObject* target, InventoryObject* item, unsigned char action_it) : ActionRunner(character), item(item), action_it(action_it)
{
  SetActionPointCost(item->GetActionPointCost(character, action_it));
  SetAnimationName("use");
  SetTargetType(ActionRunner::Object);
  SetTarget(target);
}

ActionRunner* Actions::UseObject::Factory(ObjectCharacter* user, InventoryObject* item, unsigned char action_it)
{
  Interactions::ActionRunner* runner = new UseObject(user, item, action_it);

  runner->Run();  
  return (runner);
}

ActionRunner* Actions::UseObjectOn::Factory(ObjectCharacter* user, InstanceDynamicObject* target, InventoryObject* item, unsigned char action_it)
{
  UseObjectOn* runner = new UseObjectOn(user, target, item, action_it);

  runner->Run();  
  return (runner);
}