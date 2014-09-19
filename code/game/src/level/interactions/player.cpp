#include "level/interactions/player.hpp"
#include "level/interactions/action_use.hpp"
#include "level/interactions/action_use_object.hpp"
#include "level/interactions/action_use_skill.hpp"
#include "level/interactions/action_use_spell.hpp"
#include "level/interactions/action_use_weapon.hpp"
#include "level/interactions/action_drop_object.hpp"
#include "ui/ui_use_object_on.hpp"
#include "ui/ui_use_skill_on.hpp"
#include "ui/ui_use_spell_on.hpp"
#include "ui/ui_loot.hpp"
#include "ui/ui_equip_mode.hpp"
#include "level/level.hpp"

using namespace std;

Interactions::Player::Player(Level& level) : level(level)
{
  Use.Connect       (*this,  &Player::ActionUse);
  TalkTo.Connect    (*this,  &Player::ActionTalkTo);
  UseObjectOn.Connect(*this, &Player::ActionUseObjectOn);
  UseSkillOn.Connect(*this,  &Player::ActionUseSkillOn);
  UseSpellOn.Connect(*this,  &Player::ActionUseSpellOn);
  LookAt.Connect    (*this,  &Player::ActionLookAt);
}

void Interactions::Player::SetPlayer(ObjectCharacter* player)
{
  this->player = player;
}

void Interactions::Player::ActionBarter(ObjectCharacter* character)
{
  level.GetLevelUi().OpenUiBarter(player, character);
}

void Interactions::Player::ActionTalkTo(InstanceDynamicObject* object)
{
  if (level.GetState() == Level::Fight)
    level.GetLevelUi().GetMainBar().AppendToConsole(i18n::T("Can't talk during fight."));
  else if ((player->HasLineOfSight(object)) && player->GetPathDistance(object) <= 3)
  {
    // Player must look at its target
    player->LookAt(object);
    // If target is a character, then it must look back
    ObjectCharacter* talkTo = object->Get<ObjectCharacter>();

    if (talkTo)
    {
      talkTo->LookAt(player);
      talkTo->ActionTalkTo(player);
    }
  }
  else
  {
    player->ReachedDestination.DisconnectAll();
    player->ReachedDestination.Connect([this, object]() { ActionTalkTo(object); });
    player->GoTo(object, 3);
    if (!player->IsMoving())
      level.GetLevelUi().GetMainBar().AppendToConsole(i18n::T("No line of sight"));
  }
}

void Interactions::Player::ActionTargetUse(short unsigned int it)
{
  MouseEvents&                  mouse        = level.GetMouse();
  ObjectCharacter::EquipedItem* equiped_item = player->GetEquipment().GetEquipedItem("equiped", it);

  std::cout << "ActionTargetUse" << std::endl;
  if (equiped_item && equiped_item->item)
  {
    InventoryObject* item      = equiped_item->item;
    unsigned short   action_it = equiped_item->current_action;
    Data             action    = (*item)["actions"][action_it];

    player->active_object    = item;
    player->active_object_it = action_it;
    if (action["targeted"].Value() == "1")
    {
      if (action["combat"].Value() == "1" && level.GetState() != Level::Fight)
        level.GetCombat().Start(player);
      mouse.SetState(MouseEvents::MouseTarget);
      mouse.TargetPicked.Connect([this, item, action, action_it](InstanceDynamicObject* object)
      {
        ObjectCharacter* character = object->Get<ObjectCharacter>();
        ActionRunner*    runner    = 0;

        if (character != 0 && action["combat"].Value() == "1")
        {
          if (character->IsAlive())
            runner = Interactions::Actions::UseWeaponOn::Factory(player, character, item, action_it);
          else
            level.GetLevelUi().GetMainBar().AppendToConsole(i18n::T("It's already dead."));
        }
        else
          runner = Interactions::Actions::UseObjectOn::Factory(player, object,    item, action_it);
        if (runner)
          level.GetMouse().SetState(MouseEvents::MouseAction);
      });
    }
    else
      Interactions::Actions::UseObject::Factory(player, item, action_it);
  }
}

void Interactions::Player::ActionUse(InstanceDynamicObject* object)
{
  Interactions::Actions::Use::Factory(player, object);
}

void Interactions::Player::ActionLookAt(InstanceDynamicObject* object)
{
  object->ActionLookAt(player);
}

void Interactions::Player::ActionUseObjectOn(InstanceDynamicObject* target)
{
  UiUseObjectOn* ui = level.GetLevelUi().OpenUiUseObjectOn(&(player->GetInventory()));

  ui->ObjectSelected.Connect([this, target](InventoryObject* object)
  {
    Interactions::Actions::UseObjectOn::Factory(player, target, object, 0);
  });
}

void Interactions::Player::ActionUseSkillOn(InstanceDynamicObject* target)
{
  UiUseSkillOn* ui_use_skill_on = level.GetLevelUi().OpenUiUseSkillOn(player, target);

  ui_use_skill_on->SkillPicked.Connect([this, target](const std::string& skill)
  {
    Interactions::Actions::UseSkillOn::Factory(player, target, skill);
  });
  ui_use_skill_on->Show();
}

void Interactions::Player::ActionUseSpellOn(InstanceDynamicObject* target)
{
  UiUseSpellOn* ui_use_spell_on = level.GetLevelUi().OpenUiUseSpellOn(player, target);

  ui_use_spell_on->Show();
  ui_use_spell_on->SkillPicked.Connect([this, target](const std::string& skill)
  {
    Interactions::Actions::UseSpellOn::Factory(player, target, skill);
  });
}

void Interactions::Player::ActionLootWithScript(Inventory* inventory, InstanceDynamicObject* target, asIScriptContext* context, const std::string& filepath)
{
  if (inventory && target && context)
  {
    UiLoot* ui_loot = level.GetLevelUi().OpenUiLoot(&player->GetInventory(), inventory);

    ui_loot->SetScriptObject(player, target, context, filepath);
  }
}

void Interactions::Player::ActionLoot(Inventory* inventory)
{
  if (!inventory)
  {
    Script::Engine::ScriptError.Emit("<span class='console-error'>[PlayerLoot] Aborted: NullPointer Error</span>");
    return ;
  }
  level.GetLevelUi().OpenUiLoot(&player->GetInventory(), inventory);
}

void Interactions::Player::ActionEquipForQuickUse(unsigned short it, InventoryObject* object)
{
  EquipModes& equip_modes = level.GetEquipModes();

  equip_modes.SearchForUserOnItemWithSlot(player, object, "equiped");
  if (equip_modes.HasOptions())
  {
    auto player_set_equiped_item = [this, it, object](unsigned char mode)
    {
      player->GetInventory().SetEquipedItem("equiped", it, object, mode);
    };
    
    if (equip_modes.HasChoice())
    {
      UiEquipMode* ui = level.GetLevelUi().OpenUiEquipMode();

      equip_modes.Foreach([ui](unsigned char mode, const string& name)
      {
        ui->AddOption(mode, name);
      });
      ui->EquipModeSelected.Connect(player_set_equiped_item);
      ui->Initialize();
    }
    else
    {
      equip_modes.Foreach([player_set_equiped_item](unsigned char mode, const string&) { player_set_equiped_item(mode); });
    }
  }
  else
    level.GetLevelUi().GetMainBar().AppendToConsole(i18n::T("You can't equip " + i18n::T(object->GetName())));
}

void Interactions::Player::ActionEquipObject(const std::string& target, unsigned int slot, InventoryObject* object)
{
  if (target == "equiped")
    ActionEquipForQuickUse(slot, object);
  else
  {
    cout << "Trying to equip " << object->GetName() << " in slot " << target << endl;
  }
}

void Interactions::Player::ActionDropObject(InventoryObject* object)
{
  cout << "ActionDropObject" << endl;
  Interactions::Actions::DropObject::Factory(player, object);
}

void Interactions::Player::ActionUseObject(InventoryObject* object)
{
  Interactions::Actions::UseObject::Factory(player, object, 0); // Default action is 0
}
