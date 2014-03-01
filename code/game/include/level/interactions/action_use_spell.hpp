#ifndef  INTERACTIONS_ACTIONS_USE_SPELL_HPP
# define INTERACTIONS_ACTIONS_USE_SPELL_HPP

# include "action_runner.hpp"
# include "datatree.hpp"

namespace Interactions
{
  namespace Actions
  {
    class UseSpellOn : public ActionRunner
    {
      Data spell;
      
      UseSpellOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& spell_name);
      
      virtual void RunAction();

    public:
      static ActionRunner* Factory(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& spell_name);
    };
  }
}

#endif