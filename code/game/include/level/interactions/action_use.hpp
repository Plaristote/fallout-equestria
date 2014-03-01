#ifndef  INTERACTIONS_ACTION_USE_HPP
# define INTERACTIONS_ACTION_USE_HPP

# include "action_runner.hpp"

namespace Interactions
{
  namespace Actions
  {
    class Use : public ActionRunner
    {
      Use(ObjectCharacter* character, InstanceDynamicObject* target);

      virtual void RunAction();

    public:
      static ActionRunner* Factory(ObjectCharacter* user, InstanceDynamicObject* target);
    };
  }
}

#endif