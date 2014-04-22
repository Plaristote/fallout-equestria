#include "level/player.hpp"
#include "level/level.hpp"

void Player::UnsetPlayer(void)
{
  level.GetLevelUi().GetMainBar().SetStatistics(0);
  obs.DisconnectAll();
  player = 0;
}

void Player::SetPlayer(ObjectCharacter* character)
{
  player = character;
  Interactions::Player::SetPlayer(character);
  InitializeInteractions();
  InitializeInventoryUi();
  InitializeMainBar();
  InitializePlayer();
}

ObjectCharacter* Player::GetPlayer() const
{
  return (player);
}

void Player::InitializePlayer(void)
{
  level.GetPlayerHalo().SetTarget(player);
  level.GetTargetOutliner().UsePerspectiveOfCharacter(player);
  player->GetFieldOfView().Launch();
  player->SetVisible(true);
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
  obs.Connect(level_ui.GetMainBar().OpenSkilldex, [this]() { UseSkillOn.Emit(player); });
  obs.Connect(level_ui.GetMainBar().OpenSpelldex, [this]() { UseSpellOn.Emit(0);      });
  obs.Connect(player->EquipedItemActionChanged, level_ui.GetMainBar(),   &GameMainBar::SetEquipedItemAction);
  obs.Connect(player->EquipedItemChanged,       level_ui.GetMainBar(),   &GameMainBar::SetEquipedItem);
  obs.Connect(player->EquipedItemChanged,       level_ui.GetInventory(), &GameInventory::SetEquipedItem);

  obs.Connect(level_ui.GetMainBar().EquipedItemNextAction, *player, &ObjectCharacter::ItemNextUseType);
  obs.Connect(level_ui.GetMainBar().UseEquipedItem,        *this,   &Interactions::Player::ActionTargetUse);
  obs.Connect(level_ui.GetMainBar().CombatEnd,             *this,   &Player::TryToEndCombat);
  obs.Connect(level_ui.GetMainBar().CombatPassTurn,        *this,   &Player::PassTurn);

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
