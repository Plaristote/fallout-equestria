#include "level/interactions/action_use.hpp"
#include "level/objects/character.hpp"
#define AP_COST_USE 2

using namespace std;
using namespace Interactions;

Actions::Use::Use(ObjectCharacter* character, InstanceDynamicObject* target) : ActionRunner(character)
{
  SetActionPointCost(AP_COST_USE);
  SetAnimationName("use");
  SetTargetType(Object);
  SetTarget(target);
}

void Actions::Use::RunAction()
{
  InstanceDynamicObject* target = GetObjectTarget();

  target->ActionUse(GetUser());
}

ActionRunner* Actions::Use::Factory(ObjectCharacter* user, InstanceDynamicObject* target)
{
  Actions::Use* runner = new Actions::Use(user, target);

  runner->Run();
  return (runner);
}
