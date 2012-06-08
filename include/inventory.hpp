#ifndef  INVENTORY_HPP
# define INVENTORY_HPP

# include "datatree.hpp"
# include "scriptengine.hpp"

class ObjectCharacter;
class InstanceDynamicObject;

class InventoryObject : public Data
{
public:
  enum Type
  {
    Weapon,
    Armor,
    Key,
    Consummible
  };

  InventoryObject(Data);
  ~InventoryObject();

  const std::string UseOn(ObjectCharacter* user, InstanceDynamicObject* target);

private:
  
  template<class C>
  const std::string ExecuteHook(asIScriptFunction* hook, ObjectCharacter* user, C* target);
  
  DataTree           _dataTree;
  Type               _type;

  asIScriptContext*  _scriptContext;
  asIScriptModule*   _scriptModule;
  asIScriptFunction* _hookUseOnCharacter;
  asIScriptFunction* _hookUseOnDoor;
  asIScriptFunction* _hookUseOnOthers;
};

class Inventory
{
public:
  typedef std::list<InventoryObject*> Content;

  void             AddObject(InventoryObject*);
  void             DelObject(InventoryObject*);
  const Content&   GetContent(void) const { return (_content); }
  Content&         GetContent(void)       { return (_content); }

private:
  Content _content;
};

#endif