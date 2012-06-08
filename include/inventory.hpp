#ifndef  INVENTORY_HPP
# define INVENTORY_HPP

# include "datatree.hpp"
# include "scriptengine.hpp"

class ObjectCharacter;
class InstanceDynamicObject;

class InventoryObject : public Data
{
public:
  InventoryObject(Data);
  ~InventoryObject();

  const std::string UseOn(ObjectCharacter* user, InstanceDynamicObject* target);
  const std::string GetName(void) const { return (this->Key()); }

private:
  
  template<class C>
  const std::string ExecuteHook(asIScriptFunction* hook, ObjectCharacter* user, C* target);
  
  DataTree           _dataTree;

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
  InventoryObject* GetObject(const std::string& name);

private:
  Content _content;
};

#endif