#include "inventory.hpp"
#include <level.hpp>
#include <algorithm>

InventoryObject::InventoryObject(Data data) : Data(&_dataTree)
{
  this->SetKey(data.Key());
  (*this)["icon"]     = data["icon"].Value();
  (*this)["texture"]  = data["texture"].Value();
  (*this)["model"]    = data["model"].Value();
  (*this)["scale"]    = data["scale"].Value();
  (*this)["pos"]["x"] = data["pos"]["x"].Value();
  (*this)["pos"]["y"] = data["pos"]["y"].Value();
  (*this)["weight"]   = data["weight"].Value();
  (*this)["hidden"]   = data["hidden"].Value();
  (*this)["combat"]   = data["combat"].Value();
  (*this)["range"]    = data["range"].Value();
  (*this)["interactions"]["use"] = "1";

  _equiped = false;

  _scriptContext = Script::Engine::Get()->CreateContext();
  _scriptModule  = 0;
  _hookUseOnCharacter = _hookUseOnDoor = _hookUseOnOthers = 0;
  
  Data script = data["script"];

  if (!(script.Nil()) && _scriptContext)
  {
    Data hookCharacter = script["hookCharacters"];
    Data hookDoor      = script["hookDoors"];
    Data hookOthers    = script["hookOthers"];
    Data hookWeapon    = script["hookWeapon"];

    _scriptModule       = Script::ModuleManager::Require("item" + data.Key(), "scripts/objects/" + script["file"].Value());
    if (_scriptModule)
    {
      if (!(hookCharacter.Nil()))
      {
	std::string decl = "string " + hookCharacter.Value() + "(Item@, Character@, Character@)";
	
	_hookUseOnCharacter = _scriptModule->GetFunctionByDecl(decl.c_str());
      }
      if (!(hookDoor.Nil()))
      {
	std::string decl = "string " + hookDoor.Value() + "(Item@, Character@, Door@)";
	
	_hookUseOnDoor      = _scriptModule->GetFunctionByDecl(decl.c_str());
      }
      if (!(hookOthers.Nil()))
      {
	std::string decl = "string " + hookOthers.Value() + "(Item@, Character@, DynamicObject@)";
	
	_hookUseOnOthers    = _scriptModule->GetFunctionByDecl(decl.c_str());
      }
      if (!(hookWeapon.Nil()))
      {
	std::string decl = "string " + hookWeapon.Value() + "(Item@, Character@, Character@)";
	
	_hookUseAsWeapon    = _scriptModule->GetFunctionByDecl(decl.c_str());
      }
    }
  }
}

InventoryObject::~InventoryObject()
{
  Script::ModuleManager::Release(_scriptModule);
  if (_scriptContext)
    _scriptContext->Release();
}

DynamicObject* InventoryObject::CreateDynamicObject(World* world) const
{
  DynamicObject* object;
  Data           self = *this;

  object               = world->AddDynamicObject("item" + Key(), DynamicObject::Item, self["model"], self["texture"]);
  object->interactions = Interactions::Use;
  return (object);
}

const std::string InventoryObject::UseAsWeapon(ObjectCharacter* user, ObjectCharacter* target)
{
  return (ExecuteHook(_hookUseOnCharacter, user, target));
}

const std::string InventoryObject::UseOn(ObjectCharacter* user, InstanceDynamicObject* target)
{
  ObjectCharacter* charTarget;
  ObjectDoor*      doorTarget;
  
  if (_hookUseOnCharacter && (charTarget = target->Get<ObjectCharacter>()) != 0)
    return (ExecuteHook(_hookUseOnCharacter, user, charTarget));
  if (_hookUseOnDoor      && (doorTarget = target->Get<ObjectDoor>())      != 0)
  {
    std::cout << "Door key = " << doorTarget->GetKeyName() << std::endl;
    return (ExecuteHook(_hookUseOnDoor, user, doorTarget));
  }
  if (_hookUseOnOthers)
    return (ExecuteHook(_hookUseOnOthers, user, target));
  return ("That does nothing");
}

template<class C>
const std::string InventoryObject::ExecuteHook(asIScriptFunction* hook, ObjectCharacter* user, C* target)
{
  _scriptContext->Prepare(hook);
  _scriptContext->SetArgObject(0, this);
  _scriptContext->SetArgObject(1, user);
  _scriptContext->SetArgObject(2, target);
  _scriptContext->Execute();
  return (*(reinterpret_cast<std::string*>(_scriptContext->GetReturnObject())));
}

void Inventory::AddObject(InventoryObject* toAdd)
{
  _currentWeight += (unsigned short)((*toAdd)["weight"]);
  _content.push_back(toAdd);
}

void Inventory::DelObject(InventoryObject* toDel)
{
  Content::iterator it = std::find(_content.begin(), _content.end(), toDel);

  if (it != _content.end())
  {
    _currentWeight -= (unsigned short)((*toDel)["weight"]);
    _content.erase(it);
  }
}

InventoryObject* Inventory::GetObject(const std::string& name)
{
  Content::iterator it  = _content.begin();
  Content::iterator end = _content.end();
  
  for (; it != end ; ++it)
  {
    std::cout << "Object: " << (*it)->GetName() << std::endl;
    if ((*(*it)).GetName() == name)
      return (*it);
  }
  return (0);
}

bool Inventory::CanCarry(InventoryObject* object)
{
  return (_capacity >= _currentWeight + (unsigned short)((*object)["weight"]));
}
