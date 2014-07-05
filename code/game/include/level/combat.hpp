#ifndef  LEVEL_COMBAT_HPP
# define LEVEL_COMBAT_HPP

# include "globals.hpp"
# include "serializer.hpp"
# include <list>

class Level;
class ObjectCharacter;

class Combat
{
public:
  Combat(Level& level, std::list<ObjectCharacter*>& characters) : level(level), characters(characters), iterator(characters.end())
  {}

  void             Start(ObjectCharacter*);
  bool             CanStop(void) const;
  void             Stop(void);
  void             NextTurn(void);
  ObjectCharacter* GetCurrentCharacter(void) const;
  
  void             Serialize(Utils::Packet&) const;
  void             Unserialize(Utils::Packet&);

private:
  void             FinalizeRound(void);
  void             FinalizeCharacterTurn(ObjectCharacter* character);
  void             InitializeCharacterTurn(ObjectCharacter* character);
  void             FinishFightForCharacter(ObjectCharacter* character);
  void             RefreshScriptedTasks(ObjectCharacter* character);
  void             InitializeLevelForFight(void);
  void             FinalizeFightForLevel(void);

  Level&                                level;
  std::list<ObjectCharacter*>&          characters;
  std::list<ObjectCharacter*>::iterator iterator;
};

#endif
