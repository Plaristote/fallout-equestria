#ifndef  LEVEL_PLAYER_HPP
# define LEVEL_PLAYER_HPP

# include "level/interactions/player.hpp"

class Player : public Interactions::Player
{
public:
  Player(Level& level) : Interactions::Player(level), player(0) {}
  
  void             SetPlayer(ObjectCharacter* character);
  ObjectCharacter* GetPlayer(void) const;
  void             UnsetPlayer(void);

private:
  void InitializeInventoryUi(void);
  void InitializeMainBar(void);
  void InitializeInteractions(void);
  void InitializePlayer(void);

  void TryToEndCombat(void);
  void PassTurn(void);
  void EquipItemForQuickUse(const std::string& target, unsigned int slot, InventoryObject* object);
  void UnequipItem(const std::string& target, unsigned int slot);

  ObjectCharacter*      player;
  Sync::ObserverHandler obs;
};

#endif
