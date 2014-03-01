#include "level/level.hpp"
#include <level/interactions/action_runner.hpp>
#include <level/interactions/action_use.hpp>
#include <level/interactions/action_use_object.hpp>
#include <level/interactions/action_use_skill.hpp>
#include <level/interactions/action_use_spell.hpp>
#include <level/interactions/action_use_weapon.hpp>
#include "ui/alert_ui.hpp"
#include "ui/ui_barter.hpp"
#include "ui/ui_use_object_on.hpp"
#include "ui/ui_use_skill_on.hpp"
#include "ui/ui_use_spell_on.hpp"

#define AP_COST_USE             2

#define ScriptErrorEmit(message) \
  { Script::Engine::ScriptError.Emit(std::string("<span class='console-error'>") + message + std::string("</span>")); return ; }

typedef std::function<void (AnimatedObject*)> ActionCallback;

using namespace std;

void Level::CallbackActionUse(InstanceDynamicObject* object)
{
  level_ui.CloseRunningUi<LevelUi::UiItInteractMenu>();
  Interactions::Actions::Use::Factory(GetPlayer(), object);
}

void Level::CallbackActionBarter(ObjectCharacter* character)
{
  level_ui.OpenUiBarter(GetPlayer(), character);
}

void Level::CallbackActionTalkTo(InstanceDynamicObject* object)
{
  level_ui.CloseRunningUi<LevelUi::UiItInteractMenu>();
  if (GetState() == Fight)
  {
    ConsoleWrite(i18n::T("Can't talk during fight."));
    return ;
  }
  if ((GetPlayer()->HasLineOfSight(object)) && GetPlayer()->GetPathDistance(object) <= 3)
  {
    // Player must look at its target
    GetPlayer()->LookAt(object);
    // If target is a character, then it must look back
    ObjectCharacter* talkTo = object->Get<ObjectCharacter>();
    if (talkTo)
      talkTo->LookAt(GetPlayer());

    {
      DialogController* controller = level_ui.OpenUiDialog(talkTo);

      controller->StartBarter.Connect(*this, &Level::CallbackActionBarter);
    }
  }
  else
  {
    GetPlayer()->GoTo(object, 3);
    if (GetPlayer()->GetPath().Size() > 1)
      GetPlayer()->ReachedDestination.Connect([this, object]() { CallbackActionTalkTo(object); });
    else
      ConsoleWrite(i18n::T("No line of sight"));
  }
}

void Level::CallbackActionUseObjectOn(InstanceDynamicObject* target)
{
  UiUseObjectOn* ui = level_ui.OpenUiUseObjectOn(&(GetPlayer()->GetInventory()));

  ui->ObjectSelected.Connect([this, target](InventoryObject* object)
  {
    Interactions::Actions::UseObjectOn::Factory(GetPlayer(), target, object, 0);
  });
}

void Level::CallbackActionUseSkillOn(InstanceDynamicObject* target)
{
  UiUseSkillOn* ui_use_skill_on = level_ui.OpenUiUseSkillOn(GetPlayer(), target);

  ui_use_skill_on->SkillPicked.Connect([this, target](const std::string& skill)
  {
    Interactions::Actions::UseSkillOn::Factory(GetPlayer(), target, skill);
  });
  ui_use_skill_on->Show();
}

void Level::CallbackActionUseSpellOn(InstanceDynamicObject* target)
{
  UiUseSpellOn* ui_use_spell_on = level_ui.OpenUiUseSpellOn(GetPlayer(), target);

  ui_use_spell_on->Show();
  ui_use_spell_on->SkillPicked.Connect([this, target](const std::string& skill)
  {
    Interactions::Actions::UseSpellOn::Factory(GetPlayer(), target, skill);
  });
}

void Level::CallbackActionTargetUse(unsigned short it)
{
  ObjectCharacter* player    = GetPlayer();
  InventoryObject* item      = player->GetEquipedItem(it);
  unsigned char    action_it = player->GetequipedAction(it);
  
  std::cout << "ActionTargetUse" << std::endl;
  if (item)
  {
    Data action = (*item)["actions"][action_it];

    player->active_object    = item;
    player->active_object_it = action_it;
    if (action["targeted"].Value() == "1")
    {
      if (action["combat"].Value() == "1" && _state != Fight)
        StartFight(player);
      mouse.SetState(MouseEvents::MouseTarget);
      mouse.TargetPicked.Connect([this, player, item, action, action_it](InstanceDynamicObject* object)
      {
        ObjectCharacter* character = object->Get<ObjectCharacter>();
        
        if (character != 0 && action["combat"].Value() == "1")
          Interactions::Actions::UseWeaponOn::Factory(player, character, item, action_it);
        else
          Interactions::Actions::UseObjectOn::Factory(player, object,    item, action_it);
      });
    }
    else
      Interactions::Actions::UseObject::Factory(player, item, action_it);
  }
}