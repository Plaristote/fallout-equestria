#include "level/player.hpp"
#include "level/level.hpp"

void Player::SetPlayer(ObjectCharacter* character)
{
  Interactions::Player::SetPlayer(character);
  InitializeInteractions();
  InitializeInventoryUi();
  InitializeMainBar();
  InitializePlayer();
}

void Player::InitializePlayer(void)
{
  level.GetPlayerHalo().SetTarget(player);
  level.GetTargetOutliner().UsePerspectiveOfCharacter(player);
  player->GetFieldOfView().Launch();
}

void Player::InitializeInteractions(void)
{
  Interactions::InteractionList& interactions_on_player = player->GetInteractions();

  interactions_on_player.clear();
  interactions_on_player.push_back(Interactions::Interaction("use_object", player, &UseObjectOn));
  interactions_on_player.push_back(Interactions::Interaction("use_skill",  player, &UseSkillOn));
  interactions_on_player.push_back(Interactions::Interaction("use_magic",  player, &UseSpellOn));
}

void Player::InitializeInventoryUi(void)
{
  LevelUi& level_ui = level.GetLevelUi();

  obs.Connect(level_ui.GetInventory().DropObject, *this, &Interactions::Player::ActionDropObject);
  obs.Connect(level_ui.GetInventory().UseObject,  *this, &Interactions::Player::ActionUseObject);

  for (unsigned short it = 0 ; it < 2 ; ++it) // For every equiped item slot
  {
    level_ui.GetMainBar().SetEquipedItem  (it, player->GetEquipedItem(it));
    level_ui.GetInventory().SetEquipedItem(it, player->GetEquipedItem(it));
  }
}

void Player::InitializeMainBar(void)
{
  LevelUi& level_ui = level.GetLevelUi();

  level_ui.GetMainBar().SetStatistics(player->GetStatController());
  level_ui.GetMainBar().OpenSkilldex.Connect([this]() { UseSkillOn.Emit(player); });
  level_ui.GetMainBar().OpenSpelldex.Connect([this]() { UseSpellOn.Emit(0);         });
  obs.Connect(player->EquipedItemActionChanged, level_ui.GetMainBar(),   &GameMainBar::SetEquipedItemAction);
  obs.Connect(player->EquipedItemChanged,       level_ui.GetMainBar(),   &GameMainBar::SetEquipedItem);
  obs.Connect(player->EquipedItemChanged,       level_ui.GetInventory(), &GameInventory::SetEquipedItem);
  
  level_ui.GetMainBar().EquipedItemNextAction.Connect(*player, &ObjectCharacter::ItemNextUseType);
  level_ui.GetMainBar().UseEquipedItem.Connect       (*this,   &Interactions::Player::ActionTargetUse);
  level_ui.GetMainBar().CombatEnd.Connect            (*this,   &Player::TryToEndCombat);
  level_ui.GetMainBar().CombatPassTurn.Connect       (*this,   &Player::PassTurn);

  obs.Connect(level_ui.GetInventory().EquipItem,   [this](const std::string& target, unsigned int slot, InventoryObject* object)
  {
    EquipItemForQuickUse(target, slot, object);
  });
  obs.Connect(level_ui.GetInventory().UnequipItem, [this](const std::string& target, unsigned int slot)
  {
    UnequipItem(target, slot);
  });  
}

void Player::TryToEndCombat(void)
{
  LevelUi& level_ui = level.GetLevelUi();
  Combat&  combat   = level.GetCombat();
  
    if (combat.CanStop())
      combat.Stop();
    else
      level_ui.GetMainBar().AppendToConsole("You can't leave combat mode if enemies are nearby.");
}

void Player::PassTurn(void)
{
  LevelUi& level_ui = level.GetLevelUi();
  Combat&  combat   = level.GetCombat();
  
  if (combat.GetCurrentCharacter() == player)
    combat.NextTurn();
  else
    level_ui.GetMainBar().AppendToConsole("You cannot pass other characters' turns.");
}

void Player::EquipItemForQuickUse(const std::string& target, unsigned int slot, InventoryObject* object)
{
  if (target == "equiped")
    ActionEquipForQuickUse(slot, object);
}

void Player::UnequipItem(const std::string& target, unsigned int slot)
{
  if (target == "equiped")
    player->UnequipItem(slot);
}
