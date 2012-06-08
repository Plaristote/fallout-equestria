#include "inventory.hpp"
#include <level.hpp>
#include <algorithm>

InventoryObject::InventoryObject(Data data) : Data(&_dataTree)
{
  (*this)["texture"]  = data["texture"].Value();
  (*this)["model"]    = data["model"].Value();
  (*this)["scale"]    = data["scale"].Value();
  (*this)["pos"]["x"] = data["pos"]["x"].Value();
  (*this)["pos"]["y"] = data["pos"]["y"].Value();
  (*this)["interactions"]["use"] = "1";

  _scriptContext = 0;
  _scriptModule  = 0;
  _hookUseOnCharacter = _hookUseOnDoor = _hookUseOnOthers = 0;
  
  Data script = data["script"];
  
  if (!(script.Nil()))
  {
    Data hookCharacter = script["hookCharacters"];
    Data hookDoor      = script["hookDoors"];
    Data hookOthers    = script["hookOthers"];

    _scriptModule       = Script::ModuleManager::Require("item" + data["name"].Value(), script["file"]);
    if (!(hookCharacter.Nil()))
    {
      std::string decl = "string " + hookCharacter.Value() + "(Character@, Character@)";
      
      _hookUseOnCharacter = _scriptModule->GetFunctionByDecl(decl.c_str());
    }
    if (!(hookDoor.Nil()))
    {
      std::string decl = "string " + hookDoor.Value() + "(Character@, Door@)";
      
      _hookUseOnDoor      = _scriptModule->GetFunctionByDecl(decl.c_str());
    }
    if (!(hookOthers.Nil()))
    {
      std::string decl = "string " + hookOthers.Value() + "(Character@, DynamicObject@)";
      
      _hookUseOnOthers    = _scriptModule->GetFunctionByDecl(decl.c_str());
    }
  }
}

InventoryObject::~InventoryObject()
{
  Script::ModuleManager::Release(_scriptModule);
}

const std::string InventoryObject::UseOn(ObjectCharacter* user, InstanceDynamicObject* target)
{
  ObjectCharacter* charTarget;
  ObjectDoor*      doorTarget;
  
  if (_hookUseOnCharacter && (charTarget = target->Get<ObjectCharacter>()) != 0)
    return (ExecuteHook(_hookUseOnCharacter, user, charTarget));
  if (_hookUseOnDoor      && (doorTarget = target->Get<ObjectDoor>())      != 0)
    return (ExecuteHook(_hookUseOnDoor, user, doorTarget));
  if (_hookUseOnOthers)
    return (ExecuteHook(_hookUseOnOthers, user, target));
  return ("That does nothing");
}

template<class C>
const std::string InventoryObject::ExecuteHook(asIScriptFunction* hook, ObjectCharacter* user, C* target)
{
  _scriptContext->Prepare(hook);
  _scriptContext->SetArgObject(0, user);
  _scriptContext->SetArgObject(1, target);
  _scriptContext->Execute();
  return (*(reinterpret_cast<std::string*>(_scriptContext->GetReturnObject())));
}

void Inventory::AddObject(InventoryObject* toAdd)
{
  _content.push_back(toAdd);
}

void Inventory::DelObject(InventoryObject* toDel)
{
  Content::iterator it = std::find(_content.begin(), _content.end(), toDel);

  if (it != _content.end())
    _content.erase(it);
}
