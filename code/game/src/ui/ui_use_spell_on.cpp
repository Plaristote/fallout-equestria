#include "ui/ui_use_spell_on.hpp"

using namespace std;

UiUseSpellOn::UiUseSpellOn(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* player, InstanceDynamicObject* target) : UiUseSkillOn(window, context)
{
  if (root)
  {
    map<string, short> spells;

    spells.insert(pair<string,short>("Levitate", 100));
    spells.insert(pair<string,short>("Teleport", 52));
    spells.insert(pair<string,short>("Magic Shield", 80));
    SetSkillList(spells);
  }
} 
