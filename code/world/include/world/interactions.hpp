#ifndef  WORLD_INTERACTIONS_HPP
# define WORLD_INTERACTIONS_HPP

namespace Interactions
{
  enum
  {
      None      = 0,
      Use       = 1,
      UseSkill  = 2,
      UseObject = 4,
      TalkTo    = 8,
      UseSpell  = 16
  };
}

#endif
