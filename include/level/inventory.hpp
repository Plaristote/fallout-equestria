#ifndef  INVENTORY_HPP
# define INVENTORY_HPP

# include "datatree.hpp"
# include "scriptengine.hpp"
# include "level/world.h"
# include "animatedobject.hpp"
# include "as_object.hpp"

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

  int               HitSuccessRate(ObjectCharacter* user, ObjectCharacter* target, unsigned char useType);
  bool              UseAsWeapon(ObjectCharacter* user, ObjectCharacter* target, unsigned char useType);
  bool              UseOn(ObjectCharacter* user, InstanceDynamicObject* target, unsigned char useType);
  bool              Use(ObjectCharacter* user, unsigned char useType);
  const std::string GetName(void) const { return (this->Key());             }
  const std::string GetIcon(void) const { return ((*this)["icon"].Value()); }
  DynamicObject*    CreateDynamicObject(World* world) const;
  EquipedModel*     CreateEquipedModel(World* world);

  bool              CanUse(ObjectCharacter*, InstanceDynamicObject* target, unsigned int use_type);
  bool              CanWeild(ObjectCharacter*, std::string slot, unsigned char mode = 0);
  bool              CanWeild(ObjectCharacter*, EquipedMode);
  void              SetEquiped(ObjectCharacter*, bool set);
  bool              IsEquiped(void) const { return (_equiped); }
  bool              IsGroupableWith(const InventoryObject*) const;
  bool              IsHidden(void)  const { return ((*this)["hidden"] == 1); }

private:
  template<class C>
  bool              ExecuteHook(const std::string& hook, ObjectCharacter* user, C* target, unsigned char actionIt);
  
  DataTree           _dataTree;
  
  bool               _equiped;

  asIScriptContext*  _script_context;
  asIScriptModule*   _script_module;
  
  typedef std::vector<AngelScript::Object> ActionsHooks;

  AngelScript::Object _object;  
  ActionsHooks        _actionHooks;

  asIScriptFunction* _hookCanWeild, *_hookSetEquiped;
};

class Inventory
{
public:
  typedef std::list<InventoryObject*> Content;

  struct Slot
  {
    Slot(void) : empty(true), mode(0), object(0) {}

    bool             empty;
    unsigned char    mode;
    InventoryObject* object;
  };

  struct Slots
  {
    Slots(const std::string& name) : name(name) {}

	Slots&       operator=(const Slots& cpy)
	{
		name  = cpy.name;
		slots = cpy.slots;
		return (*this);
	}

    bool        operator==(const std::string& name) const { return (this->name == name); }
    const Slot& operator[](unsigned int i)          const { return (slots[i]);           }
    Slot&       operator[](unsigned int i)
    {
      if (i >= slots.size())
        slots.resize(i + 1);
      return (slots[i]);
    }

  private:
    std::string       name;
    std::vector<Slot> slots;
  };

  Inventory(void) { _currentWeight = _capacity = 0; }
  
  void               LoadInventory(DynamicObject*);
  void               SaveInventory(DynamicObject*);
  void               LoadInventory(Data);
  void               SaveInventory(Data);

  //
  // Content
  //
  void               AddObject(InventoryObject*);
  void               DelObject(InventoryObject*);
  bool               IncludesObject(InventoryObject*) const;
  const Content&     GetContent(void) const { return (_content); }
  Content&           GetContent(void)       { return (_content); }
  InventoryObject*   GetObject(const std::string& name);
  unsigned short     ContainsHowMany(const std::string& name) const;

  int                GetObjectIterator(InventoryObject* object) const;

  unsigned short     GetCurrentWeight(void) const      { return (_currentWeight); }
  unsigned short     GetCapacity(void)      const      { return (_capacity);      }
  void               SetCapacity(unsigned short value) { _capacity = value;       }
  bool               CanCarry(InventoryObject*, unsigned short quantity = 1);
  
  Sync::Signal<void> ContentChanged;

  //
  // Slots
  //
  void               InitializeSlots(void);
  const Slot&        GetConstItemSlot(const std::string& type_slot, unsigned int slot = 0) const;
  Slot&              GetItemSlot(const std::string& type_slot, unsigned int slot = 0);
  bool               SlotHasEquipedItem(const std::string& type_slot, unsigned int slot = 0) const;
  unsigned char      GetEquipedMode(const std::string& type_slot, unsigned int slot = 0) const;
  InventoryObject*   GetEquipedItem(const std::string& type_slot, unsigned int slot = 0);
  void               SetEquipedItem(const std::string& type_slot, unsigned int slot, InventoryObject* object, unsigned char equip_mode = 0);
  void               SetEquipedItem(const std::string& type_slot, InventoryObject* object, unsigned char equip_mode = 0)
  {
    SetEquipedItem(type_slot, 0, object, equip_mode);
  }

  Sync::Signal<void (const std::string&, unsigned int, InventoryObject*)> UnequipedItem;
  Sync::Signal<void (const std::string&, unsigned int, InventoryObject*)> EquipedItem;

private:
  Content            _content;
  unsigned short     _currentWeight;
  unsigned short     _capacity;
  std::vector<Slots> _slots;
};

#endif
