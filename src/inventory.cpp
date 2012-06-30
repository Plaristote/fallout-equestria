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
  if (!(data["ap-cost"].Nil()))
    (*this)["ap-cost"]  = data["ap-cost"].Value();
  (*this)["weight"]   = (data["weight"].Nil()) ? "0" : data["weight"].Value();
  (*this)["hidden"]   = data["hidden"].Value();
  (*this)["combat"]   = data["combat"].Value();
  (*this)["range"]    = data["range"].Value();
  (*this)["interactions"]["use"] = "1";

  (*this)["mode-mouth"]        = (data["mode-mouth"].Nil())        ? "1" : data["mode-mouth"].Value();
  (*this)["mode-magic"]        = (data["mode-magic"].Nil())        ? "1" : data["mode-magic"].Value();
  (*this)["mode-battlesaddle"] = (data["mode-battlesaddle"].Nil()) ? "1" : data["mode-battlesaddle"].Value();

  _equiped = false;

  _scriptContext = Script::Engine::Get()->CreateContext();
  _scriptModule  = 0;
  _hookUseOnCharacter = _hookUseOnDoor = _hookUseOnOthers = 0;
  _hookCanWeildBattleSaddle = _hookCanWeildMagic = _hookCanWeildMouth = 0;
  
  Data script = data["script"];

  if (!(script.Nil()) && _scriptContext)
  {
    Data hookCharacter  = script["hookCharacters"];
    Data hookDoor       = script["hookDoors"];
    Data hookOthers     = script["hookOthers"];
    Data hookWeapon     = script["hookWeapon"];
    Data hookWeildMouth = script["hookWeildMouth"];
    Data hookWeildMagic = script["hookWeildMagic"];
    Data hookWeildBS    = script["hookWeildBattleSaddle"];

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
      if (!(hookWeildMouth.Nil()))
      {
	std::string decl = "bool " + hookWeildMouth.Value() + "(Item@, Character@)";
	
	_hookCanWeildMouth  = _scriptModule->GetFunctionByDecl(decl.c_str());
      }
      if (!(hookWeildMagic.Nil()))
      {
	std::string decl = "bool " + hookWeildMagic.Value() + "(Item@, Character@)";
	
	_hookCanWeildMagic  = _scriptModule->GetFunctionByDecl(decl.c_str());
      }
      if (!(hookWeildBS.Nil()))
      {
	std::string decl = "bool " + hookWeildBS.Value() + "(Item@, Character@)";
	
	_hookCanWeildBattleSaddle  = _scriptModule->GetFunctionByDecl(decl.c_str());
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

bool InventoryObject::CanWeild(ObjectCharacter* character, EquipedMode mode)
{
  std::string        modeStr;
  asIScriptFunction* hook = 0;
  
  switch (mode)
  {
    case EquipedMouth:
      modeStr = "mode-mouth";
      hook    = _hookCanWeildMouth;
      break ;
    case EquipedMagic:
      modeStr = "mode-magic";
      hook    = _hookCanWeildMagic;
      break ;
    case EquipedBattleSaddle:
      modeStr = "mode-battlesaddle";
      hook    = _hookCanWeildBattleSaddle;
      break ;
  }
  if (_scriptContext && hook)
  {
    _scriptContext->Prepare(hook);
    _scriptContext->SetArgObject(0, this);
    _scriptContext->SetArgObject(1, character);
    _scriptContext->Execute();
    return (_scriptContext->GetReturnByte());
  }
  return ((*this)[modeStr].Value() == "1");
}

InventoryObject::EquipedModel* InventoryObject::CreateEquipedModel(World* world)
{
  if ((*this)["model"].Value() != "")
  {
    EquipedModel* equipedModel = new EquipedModel(world->window, this);

    return (equipedModel);
  }
  return (0);
}

InventoryObject::EquipedModel::EquipedModel(WindowFramework* window, InventoryObject* p) : AnimatedObject(window), object(*p)
{
  Data        anims   = object["animations"];
  std::string texture = object["texture"].Value();
  Texture*    texfile;

  _modelName   = object["model"].Value();
  for (short i = 0 ; i < _modelName.size() ; ++i)
  {
    if (_modelName[i] == '/' || _modelName[i] == '\\')
    {
      _modelName.erase(0, i);
      i = 0;
    }
  }
  for (short i = _modelName.size() - 1 ; i >= 0 ; --i)
  {
    if (_modelName[i] == '.')
    {
      _modelName.erase(i);
      break ;
    }
  }
  _modelName   = "items/" + _modelName;

  np = window->load_model(window->get_panda_framework()->get_models(), MODEL_ROOT + object["model"].Value());
  if (texture != "")
  {
    texfile    = TexturePool::load_texture(TEXT_ROOT + texture);
    if (texfile)
      np.set_texture(texfile);
  }
  if (!(anims.Nil()))
  {
    std::for_each(anims.begin(), anims.end(), [this](Data animation)
    {
      if (animation.Key() == "player")
	return ;
      LoadAnimation(animation.Value());
    });
  }
  np.set_name("Equiped" + object.GetName());
}

InventoryObject::EquipedModel::~EquipedModel()
{
  np.detach_node();
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

/*
 * Inventory
 */

void Inventory::AddObject(InventoryObject* toAdd)
{
  std::cout << "Weight is " << _currentWeight << ", adding object " << toAdd->GetName() << std::endl;
  Data weight = (*toAdd)["weight"];
  
  if (!(weight.Nil()))
    _currentWeight += (unsigned short)((*toAdd)["weight"]);
  _content.push_back(toAdd);
  ContentChanged.Emit();
  std::cout << "Weight is now " << _currentWeight << std::endl;
}

void Inventory::DelObject(InventoryObject* toDel)
{
  Content::iterator it = std::find(_content.begin(), _content.end(), toDel);

  if (it != _content.end())
  {
    Data weight = (*toDel)["weight"];
  
    if (!(weight.Nil()))
      _currentWeight -= (unsigned short)((*toDel)["weight"]);
    _content.erase(it);
    ContentChanged.Emit();
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
