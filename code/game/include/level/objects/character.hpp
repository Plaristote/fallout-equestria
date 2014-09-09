#ifndef  CHARACTER_HPP
# define CHARACTER_HPP

# include "globals.hpp"
# include "level/objects/instance_dynamic_object.hpp"
# include "level/inventory.hpp"
# include "time_manager.hpp"
# include "cmap/statcontroller.hpp"
# include "level/diplomacy.hpp"
# include "level/characters/line_of_sight.hpp"
# include "level/characters/field_of_view.hpp"
# include "level/characters/action_points.hpp"
# include "level/interactions/action_runner.hpp"
# include <panda3d/character.h>
# include <panda3d/pointLight.h>

class ObjectCharacter : public CharacterActionPoints
{
  friend class Interactions::ActionRunner;
public:
  ObjectCharacter(Level* level, DynamicObject* object);
  ~ObjectCharacter(void);

  Pathfinding::Path        GetPathTowardsObject(Collider* character);

  void                     Unserialize(Utils::Packet&);
  void                     Serialize(Utils::Packet&);

  void                     ActionTalkTo(ObjectCharacter* user);
  void                     ActionUse(InstanceDynamicObject* object);

  void                     SetInventory(Inventory* inventory);

  void                     ProcessCollisions(void)         { if (IsAlive()) InstanceDynamicObject::ProcessCollisions();     }
  bool                     HasOccupiedWaypoint(void) const { return (IsAlive() && Pathfinding::Collider::HasOccupiedWaypoint()); }

  void                     Run(float elapsedTime);
  void                     RunRegularBehaviour(float elapsedTime);
  void                     RunCombatBehaviour(float elapsedTime);
  bool                     HasLineOfSight(InstanceDynamicObject* object);
  bool                     IsPlayer(void) const;
  bool                     IsInterrupted(void) const;
  bool                     IsBusy(void) const;
  Inventory&               GetInventory(void)         { return (*_inventory);           }
  const Inventory&         GetInventory(void) const   { return (*_inventory); }
  const std::string        GetFactionName(void) const { return (_faction ? _faction->name : ""); }
  unsigned int             GetFaction(void) const     { return (_faction ? _faction->flag : 0);  }
  NodePath                 GetJoint(const std::string& name);

  void                     SetFurtive(bool do_set);

  void                     PlayEquipedItemAnimation(unsigned short it, const std::string& name, InstanceDynamicObject* target);
  void                     RefreshEquipment(void);
  void                     ItemNextUseType(unsigned short it);
  Sync::Signal<void (unsigned short, InventoryObject*)>                EquipedItemChanged;
  Sync::Signal<void (unsigned short, InventoryObject*, unsigned char)> EquipedItemActionChanged;

  FieldOfView&             GetFieldOfView(void)       { return (field_of_view); }
  const FieldOfView&       GetFieldOfView(void) const { return (field_of_view); }
  void                     SetFaction(const std::string&);
  void                     SetFaction(unsigned int flag);
  void                     SetAsEnemy(ObjectCharacter*, bool);
  bool                     IsEnemy(const ObjectCharacter*) const;
  bool                     IsAlly(const ObjectCharacter*)  const;

  Script::StdList<ObjectCharacter*> GetNearbyEnemies(void) const;
  Script::StdList<ObjectCharacter*> GetNearbyAllies(void)  const;
  
  // Script Communication Tools
  void                     RequestAttack(ObjectCharacter* attack, ObjectCharacter* from);
  void                     RequestHeal(ObjectCharacter* heal, ObjectCharacter* from);
  void                     RequestFollow(ObjectCharacter* follow, ObjectCharacter* from);
  void                     RequestStopFollowing(ObjectCharacter* follow, ObjectCharacter* from);
  int                      AskMorale(void);
  void                     SendMessage(std::string&);

private:
  void                     SetupScript(AngelScript::Object*);
  void                     RefreshStatistics(void);
  void                     RunDeath(void);
  
  void                     RequestCharacter(ObjectCharacter*, ObjectCharacter*, const std::string& func);
  
  Sync::ObserverHandler          _obs_handler;
  PT(Character)                  _character;
  DataTree*                      _statistics;
  StatController*                _stats;
  const WorldDiplomacy::Faction* _faction;
  unsigned int                   _self_enemyMask;
  LineOfSight                    line_of_sight;
  FieldOfView                    field_of_view;
  AngelScript::Object*           script;
  Interactions::ActionRunner*    current_action;

  public:
    InventoryObject*             active_object;
    unsigned short               active_object_it;

  // Inventory and Equiped Items
  struct EquipedItem
  {
    unsigned char                  current_action;
    InventoryObject*               item;
    Inventory::Slot*               slot;
    InventoryObject::EquipedModel* render;
  };

  struct Equipment : public Sync::ObserverHandler
  {
    Equipment(ObjectCharacter& self) : character(self), inventory(0) {}
    ~Equipment();

    void         SetInventory(Inventory* inventory);
    void         EquipDefaultItemForSlot(const std::string& slot, unsigned short slot_number = 0);
    void         EquipItem(InventoryObject* item);
    void         UnequipItem(InventoryObject* item);
    std::string  GetDefaultItemForSlot(const std::string& slot, unsigned short slot_number = 0);
    NodePath     GetJointForSlot(const std::string& slot, unsigned short slot_number = 0);
    EquipedItem* GetEquipedItem(InventoryObject* item);
    EquipedItem* GetEquipedItem(const std::string& slot, unsigned short slot_number = 0);
    NodePath     GetEquipedItemNode(const string &slot_type, unsigned short it);

  private:
    void         ClearSlot(const std::string& slot, unsigned short slot_number = 0);

    ObjectCharacter&       character;
    Inventory*             inventory;
    std::list<EquipedItem> equiped_items;
  };

  friend struct Equipment;

  Equipment&               GetEquipment(void) { return (equipment); }

private:
  Inventory*                _inventory;
  Equipment                 equipment;
};

template<> struct ObjectType2Code<ObjectCharacter> { enum { Type = ObjectTypes::Character }; };

#endif
