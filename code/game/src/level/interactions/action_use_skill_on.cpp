#include "level/interactions/action_use_skill.hpp"
#include "level/objects/character.hpp"
#define AP_COST_USE 2

using namespace std;
using namespace Interactions;

Actions::UseSkillOn::UseSkillOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill) : ActionRunner(user), skill(skill)
{
  SetActionPointCost(AP_COST_USE);
  SetAnimationName("use");
  SetTargetType(user != target ? ActionRunner::Object : ActionRunner::Self);
  SetTarget(target);
}

void Actions::UseSkillOn::RunAction()
{
  InstanceDynamicObject* target = GetObjectTarget();
  
  if (GetUser() != 0 && target != 0)
    target->ActionUseSkill(GetUser(), skill);
}

ActionRunner* Actions::UseSkillOn::Factory(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill_name)
{
  UseSkillOn* runner = new UseSkillOn(user, target, skill_name);

  runner->Run();  
  return (runner);
}
  
