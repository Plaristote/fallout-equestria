#ifndef  INVENTORY_HPP
# define INVENTORY_HPP

# include "datatree.hpp"
# include "scriptengine.hpp"
# include "world.h"

class ObjectCharacter;
class InstanceDynamicObject;

class InventoryObject : public Data
{
public:
  InventoryObject(Data);
  ~InventoryObject();

  const std::string UseAsWeapon(ObjectCharacter* user, ObjectCharacter* target);
  const std::string UseOn(ObjectCharacter* user, InstanceDynamicObject* target);
  const std::string GetName(void) const { return (this->Key()); }
  DynamicObject*    CreateDynamicObject(World* world) const;

  void              SetEquiped(bool set)  { _equiped = set;    }
  bool              IsEquiped(void) const { return (_equiped); }

private:
  template<class C>
  const std::string ExecuteHook(asIScriptFunction* hook, ObjectCharacter* user, C* target);
  
  DataTree           _dataTree;
  
  bool               _equiped;

  asIScriptContext*  _scriptContext;
  asIScriptModule*   _scriptModule;
  asIScriptFunction* _hookUseOnCharacter;
  asIScriptFunction* _hookUseOnDoor;
  asIScriptFunction* _hookUseOnOthers;
  asIScriptFunction* _hookUseAsWeapon;
};

class Inventory
{
public:
  typedef std::list<InventoryObject*> Content;
  
  Inventory(void) { _currentWeight = _capacity = 0; }

  void             AddObject(InventoryObject*);
  void             DelObject(InventoryObject*);
  const Content&   GetContent(void) const { return (_content); }
  Content&         GetContent(void)       { return (_content); }
  InventoryObject* GetObject(const std::string& name);

  unsigned short   GetCurrentWeight(void) const      { return (_currentWeight); }
  unsigned short   GetCapacity(void)      const      { return (_capacity);      }
  void             SetCapacity(unsigned short value) { _capacity = value;       }
  bool             CanCarry(InventoryObject*);

private:
  Content          _content;
  unsigned short   _currentWeight;
  unsigned short   _capacity;
};

#endif