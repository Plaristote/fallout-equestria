#include "level/interactions/action_use_spell.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"
#include "ui/alert_ui.hpp"
#define AP_COST_USE 2

using namespace std;
using namespace Interactions;

Actions::UseSpellOn::UseSpellOn(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& spell_name) : ActionRunner(user)
{
  Data   data_engine      = GetLevel()->GetDataEngine();
  string target_type_name;

  spell                   = data_engine["spells"][spell_name];
  SetAnimationName("use");
  SetActionPointCost(AP_COST_USE);
  SetTargetType(spell["targets"].Value());
}

void Actions::UseSpellOn::RunAction()
{
  try
  {
    AngelScript::Object                   object("scripts/buffs/" + spell["script"]["file"].Value());
    AngelScript::Type<ObjectCharacter*>   param1(GetUser());
    AngelScript::Type<void*>              param2(GetObjectTarget());

    object.asDefineMethod("use_spell", spell["script"]["method"].Value());
    if (GetTargetType() == Waypoint)
    {
      AngelScript::Type<int>              param_waypoint(GetWaypointTarget()->id);

      object.Call("use_spell", 2, &param1, &param_waypoint);
    }
    else
      object.Call("use_spell", 2, &param1, &param2);
  }
  catch (AngelScript::Exception& e)
  {
    AlertUi::NewAlert.Emit(string("Spell script couldn't be executed: ") + e.what());
  }
}

ActionRunner* Actions::UseSpellOn::Factory(ObjectCharacter* user, InstanceDynamicObject* target, const std::string& spell_name)
{
  UseSpellOn* runner = new UseSpellOn(user, target, spell_name);

  runner->Run();  
  return (runner);  
}
