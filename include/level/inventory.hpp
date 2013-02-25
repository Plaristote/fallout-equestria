#ifndef  INVENTORY_HPP
# define INVENTORY_HPP

# include "datatree.hpp"
# include "scriptengine.hpp"
# include "level/world.h"
# include "animatedobject.hpp"

class ObjectCharacter;
class InstanceDynamicObject;

enum EquipedMode
{
  EquipedMouth,
  EquipedMagic,
  EquipedBattleSaddle
};

class InventoryObject : public Data
{
public:
  struct EquipedModel : public AnimatedObject
  {
    EquipedModel(WindowFramework* win, InventoryObject* pObject);
    virtual ~EquipedModel();
    
    NodePath GetNodePath(void) const { return (np); }
    void     ResetAnimation(void)    {}

    NodePath np;
    InventoryObject& object;
  };

  InventoryObject(Data);
  ~InventoryObject();

  const std::string UseAsWeapon(ObjectCharacter* user, ObjectCharacter* target, unsigned char useType);
  const std::string UseOn(ObjectCharacter* user, InstanceDynamicObject* target, unsigned char useType);
  const std::string Use(ObjectCharacter* user, unsigned char useType);
  const std::string GetName(void) const { return (this->Key());             }
  const std::string GetIcon(void) const { return ((*this)["icon"].Value()); }
  DynamicObject*    CreateDynamicObject(World* world) const;
  EquipedModel*     CreateEquipedModel(World* world);

  bool              CanWeild(ObjectCharacter*, EquipedMode);
  void              SetEquiped(bool set)  { _equiped = set;    }
  bool              IsEquiped(void) const { return (_equiped); }
  bool              IsGroupableWith(const InventoryObject*) const;

private:
  template<class C>
  const std::string ExecuteHook(asIScriptFunction* hook, ObjectCharacter* user, C* target, unsigned char actionIt);
  
  DataTree           _dataTree;
  
  bool               _equiped;

  asIScriptContext*  _script_context;
  asIScriptModule*   _script_module;
  
  struct ActionHooks
  {
    ActionHooks() : Use(0), UseOnCharacter(0), UseOnDoor(0), UseOnOthers(0), UseAsWeapon(0) {}

    asIScriptFunction* Use;
    asIScriptFunction* UseOnCharacter;
    asIScriptFunction* UseOnDoor;
    asIScriptFunction* UseOnOthers;
    asIScriptFunction* UseAsWeapon;
  };
  typedef std::vector<ActionHooks> ActionsHooks;
  
  ActionsHooks       _actionHooks;

  asIScriptFunction* _hookUseOnCharacter;
  asIScriptFunction* _hookUseOnDoor;
  asIScriptFunction* _hookUseOnOthers;
  asIScriptFunction* _hookUseAsWeapon;
  
  asIScriptFunction* _hookCanWeildMouth, *_hookCanWeildMagic, *_hookCanWeildBattleSaddle;
};

class Inventory
{
public:
  typedef std::list<InventoryObject*> Content;
  
  Inventory(void) { _currentWeight = _capacity = 0; }
  
  void             LoadInventory(DynamicObject*);
  void             SaveInventory(DynamicObject*);
  void             LoadInventory(Data);
  void             SaveInventory(Data);

  void             AddObject(InventoryObject*);
  void             DelObject(InventoryObject*);
  bool             IncludesObject(InventoryObject*) const;
  const Content&   GetContent(void) const { return (_content); }
  Content&         GetContent(void)       { return (_content); }
  InventoryObject* GetObject(const std::string& name);
  unsigned short   ContainsHowMany(const std::string& name) const;

  unsigned short   GetCurrentWeight(void) const      { return (_currentWeight); }
  unsigned short   GetCapacity(void)      const      { return (_capacity);      }
  void             SetCapacity(unsigned short value) { _capacity = value;       }
  bool             CanCarry(InventoryObject*, unsigned short quantity = 1);
  
  Observatory::Signal<void> ContentChanged;

private:
  Content          _content;
  unsigned short   _currentWeight;
  unsigned short   _capacity;
};

#endif
