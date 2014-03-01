#ifndef  INTERACTIONS_PLAYER_HPP
# define INTERACTIONS_PLAYER_HPP

# include "interaction.hpp"
# include "as_object.hpp"

class Level;
class ObjectCharacter;
class InventoryObject;
class Inventory;

namespace Interactions
{
  struct Player
  {
    friend class Level;
    
    Player(Level& level);

    void    SetPlayer(ObjectCharacter*);

    Trigger LookAt;
    Trigger Use;
    Trigger UseObjectOn;
    Trigger UseSkillOn;
    Trigger UseSpellOn;
    Trigger TalkTo;

    void    ActionLookAt(InstanceDynamicObject* object);
    void    ActionUse(InstanceDynamicObject* object);
    void    ActionTalkTo(InstanceDynamicObject* object);
    void    ActionBarter(ObjectCharacter*);
    void    ActionDropObject(InventoryObject* object);
    void    ActionUseObject(InventoryObject* item);
    void    ActionUseObjectOn(InstanceDynamicObject* object);
    void    ActionUseSkillOn(InstanceDynamicObject* object);
    void    ActionUseSpellOn(InstanceDynamicObject* object);
    void    ActionTargetUse(unsigned short it);
    void    ActionEquipObject(const std::string& target, unsigned int slot, InventoryObject* object);
    void    ActionEquipForQuickUse(unsigned short slot, InventoryObject* object);
    void    ActionLootWithScript(Inventory* inventory, InstanceDynamicObject* target, asIScriptContext* context, const std::string& filepath);
    void    ActionLoot(Inventory* inventory);

  private:
    Level&           level;
    ObjectCharacter* player;
  };
}

#endif