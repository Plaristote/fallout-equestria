#include "general_pony.as"
#include "lib-combat-base.as"

void main(Character@ self, float elapsedTime)
{
  Data       data_engine = level.GetDataEngine();
  Character@ player      = level.GetPlayer();

  if (data_engine["variables"]["Sterling"]["allied"].AsInt() == 1)
    ai_template_follow_char(self, player, elapsedTime);
}

void combat(Character@ self)
{
  default_combat_behavior(self);
}