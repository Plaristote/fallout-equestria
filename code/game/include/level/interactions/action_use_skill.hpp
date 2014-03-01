#ifndef  INTERACTIONS_ACTIONS_USE_SKILL_HPP
# define INTERACTIONS_ACTIONS_USE_SKILL_HPP

# include "action_runner.hpp"

namespace Interactions
{
  namespace Actions
  {
    class UseSkillOn : public ActionRunner
    {
      const std::string skill;

      UseSkillOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill);

      virtual void RunAction();
      
    public:
      static ActionRunner* Factory(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& skill_name);
    };
  }
}

#endif