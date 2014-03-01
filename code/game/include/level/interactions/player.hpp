#ifndef  INTERACTIONS_PLAYER_HPP
# define INTERACTIONS_PLAYER_HPP

# include "interaction.hpp"

namespace Interactions
{
  struct Player
  {
    Trigger LookAt;
    Trigger Use;
    Trigger UseObjectOn;
    Trigger UseSkillOn;
    Trigger UseSpellOn;
    Trigger TalkTo;
  };
}

#endif