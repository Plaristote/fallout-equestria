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
  
  void                     Unserialize(Utils::Packet&);
  void                     Serialize(Utils::Packet&);

  void                     ActionTalkTo(ObjectCharacter* user);
  void                     ActionUse(InstanceDynamicObject* object);

  void                     SetInventory(Inventory* inventory);

  void                     ProcessCollisions(void)         { if (IsAlive()) InstanceDynamicObject::ProcessCollisions();     }
  bool                     HasOccupiedWaypoint(void) const { return (IsAlive() && Pathfinding::Collider::HasOccupiedWaypoint()); }

  void                     Run(float elapsedTime);
  float                    GetDistance(const InstanceDynamicObject* object) const;
  bool                     HasLineOfSight(InstanceDynamicObject* object);
  bool                     IsPlayer(void) const;
  bool                     IsInterrupted(void) const  { return (AnimationEndForObject.ObserverCount() > 0); }
  bool                     IsBusy(void) const;
  Inventory&               GetInventory(void)         { return (*_inventory);           }
  const Inventory&         GetInventory(void) const   { return (*_inventory); }
  const std::string        GetFactionName(void) const { return (_faction ? _faction->name : ""); }
  unsigned int             GetFaction(void) const     { return (_faction ? _faction->flag : 0);  }
    
  void                     SetFurtive(bool do_set);
  void                     SetVisible(bool do_set)           { _fading_in = do_set == true; _fading_off = !_fading_in; }
  void                     AddFlag(unsigned char flag)       { _flags |= flag; }
  void                     DelFlag(unsigned char flag)       { if (HasFlag(flag)) { _flags -= flag; } }
  bool                     HasFlag(unsigned char flag) const { return ((_flags & flag) != 0); }

  void                     PlayEquipedItemAnimation(unsigned short it, const std::string& name, InstanceDynamicObject* target);
  void                     RefreshEquipment(void);
  void                     SetEquipedItem(unsigned short it, InventoryObject* object, EquipedMode mode = EquipedMouth);
  InventoryObject*         GetEquipedItem(unsigned short it);
  NodePath                 GetEquipedItemNode(unsigned short it);
  unsigned char            GetequipedAction(unsigned short it) const { return (_equiped[it].actionIt); }
  void                     UnequipItem(unsigned short it);
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
  void                     Fading(void);
  
  void                     RequestCharacter(ObjectCharacter*, ObjectCharacter*, const std::string& func);
  
  Sync::ObserverHandler          _obs_handler;
  PT(Character)                  _character;
  DataTree*                      _statistics;
  StatController*                _stats;
  const WorldDiplomacy::Faction* _faction;
  unsigned int                   _self_enemyMask;
  unsigned char                  _flags;
  bool                           _fading_off, _fading_in;
  LineOfSight                    line_of_sight;
  FieldOfView                    field_of_view;
  AngelScript::Object*           script;
  Interactions::ActionRunner*    current_action;

  public:
    InventoryObject*             active_object;
    unsigned short               active_object_it;
  private:

  // Inventory and Equiped Items
  struct ItemEquiped
  {
    ItemEquiped(void) : actionIt(0), equiped(0), default_(0), graphics(0) {}
    unsigned char                  actionIt;
    EquipedMode                    mode;
    InventoryObject*               equiped;
    InventoryObject*               default_;
    InventoryObject::EquipedModel* graphics;
    NodePath                       jointHorn, jointBattleSaddle, jointMouth;
  };

  Inventory*                _inventory;
  ItemEquiped               _equiped[2];

  
  // Script
};

template<> struct ObjectType2Code<ObjectCharacter> { enum { Type = ObjectTypes::Character }; };

#endif
