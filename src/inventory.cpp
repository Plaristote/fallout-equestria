#include "inventory.hpp"
#include <level.hpp>
#include <algorithm>

InventoryObject::InventoryObject(Data data) : Data(&_dataTree)
{
  // Duplicate the DataBranch into the InventoryObject
  Duplicate(data);

  // Set the default values
  (*this)["weight"]   = (data["weight"].Nil()) ? "0" : data["weight"].Value();
  (*this)["interactions"]["use"] = "1";
  
  if (data["mode-mouth"].Nil())
    (*this)["mode-mouth"] = "1";
  if (data["mode-magic"].Nil())
    (*this)["mode-magic"] = "1";

  _equiped = false;

  _scriptContext = Script::Engine::Get()->CreateContext();
  _scriptModule  = 0;
  _hookUseOnCharacter = _hookUseOnDoor = _hookUseOnOthers = 0;
  _hookCanWeildBattleSaddle = _hookCanWeildMagic = _hookCanWeildMouth = 0;
  
  Data script = data["script"];

  if (!(script.Nil()) && _scriptContext)
  {
    Data hookWeildMouth = script["hookWeildMouth"];
    Data hookWeildMagic = script["hookWeildMagic"];
    Data hookWeildBS    = script["hookWeildBattleSaddle"];

    if (script["file"].Value() != "")
    {
      _scriptModule       = Script::ModuleManager::Require("item" + data.Key(), "scripts/objects/" + script["file"].Value());
      if (_scriptModule)
      {
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
  
  ForEach(data["actions"], [this](Data action)
  {
    Data        copy = (*this)["actions"][action.Key()];
    ActionHooks hooks;
    
    copy["ap-cost"]        = action["ap-cost"].Value();
    copy["targeted"]       = (action["targeted"].Nil() ? "1" : action["targeted"].Value());
    copy["range"]          = action["range"].Value();
    copy["combat"]         = action["combat"].Value();
    
    if (_scriptModule)
    {
      if (action["hookUse"].Nil() == false)
	hooks.Use             = _scriptModule->GetFunctionByDecl(("string " + action["hookUse"].Value() + "(Item@, Character@)").c_str());
      if (action["hookCharacters"].Nil() == false)
	hooks.UseOnCharacter  = _scriptModule->GetFunctionByDecl(("string " + action["hookCharacters"].Value() + "(Item@, Character@, Character@)").c_str());
      if (action["hookDoors"].Nil() == false)
	hooks.UseOnDoor       = _scriptModule->GetFunctionByDecl(("string " + action["hookDoors"].Value() + "(Item@, Character@, Door@)").c_str());
      if (action["hookOthers"].Nil() == false)
	hooks.UseOnOthers     = _scriptModule->GetFunctionByDecl(("string " + action["hookOthers"].Value() + "(Item@, Character@, DynamicObject@)").c_str());
      if (action["hookWeapon"].Nil() == false)
	hooks.UseAsWeapon     = _scriptModule->GetFunctionByDecl(("string " + action["hookWeapon"].Value() + "(Item@, Character@, Character@)").c_str());
    }
    _actionHooks.push_back(hooks);
  });
}

InventoryObject::~InventoryObject()
{
  Script::ModuleManager::Release(_scriptModule);
  if (_scriptContext)
    _scriptContext->Release();
}

bool InventoryObject::IsGroupableWith(const InventoryObject* other) const
{
  return (GetName() == other->GetName());
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

const std::string InventoryObject::UseAsWeapon(ObjectCharacter* user, ObjectCharacter* target, unsigned char useType)
{
  return (ExecuteHook(_actionHooks[useType].UseAsWeapon, user, target, useType));
}

const std::string InventoryObject::UseOn(ObjectCharacter* user, InstanceDynamicObject* target, unsigned char useType)
{
  ObjectCharacter* charTarget;
  ObjectDoor*      doorTarget;
  ActionHooks&     hooks = _actionHooks[useType];

  if (hooks.UseOnCharacter && (charTarget = target->Get<ObjectCharacter>()) != 0)
    return (ExecuteHook(hooks.UseOnCharacter, user, charTarget, useType));
  if (hooks.UseOnDoor      && (doorTarget = target->Get<ObjectDoor>())      != 0)
    return (ExecuteHook(hooks.UseOnDoor, user, doorTarget, useType));
  if (hooks.UseOnOthers)
    return (ExecuteHook(hooks.UseOnOthers, user, target, useType));
  return ("That does nothing");
}

const std::string InventoryObject::Use(ObjectCharacter* user, unsigned char useType)
{
  return (ExecuteHook(_actionHooks[useType].Use, user, (ObjectCharacter*)0, useType));
}

template<class C>
const std::string InventoryObject::ExecuteHook(asIScriptFunction* hook, ObjectCharacter* user, C* target, unsigned char useType)
{
  if (hook)
  {
    int ret;

    ret = _scriptContext->Prepare(hook);
    
    switch (ret)
    {
      case asCONTEXT_ACTIVE:
	return ("/!\\ Script Context still active");
      default:
	break ;
    }
    
    _scriptContext->SetArgObject(0, this);
    _scriptContext->SetArgObject(1, user);
    if (target != nullptr)
      _scriptContext->SetArgObject(2, target);
    ret = _scriptContext->Execute();
    switch (ret)
    {
      case asEXECUTION_FINISHED:
	break ;
      case asERROR:
	return ("/!\\ Failed to execute script (Invalid Context) !");
      case asEXECUTION_EXCEPTION:
	return ("/!\\ Failed to execute script (Exception) !");
      default:
	return ("/!\\ Failed to execute script !");
    }
    return (*(reinterpret_cast<std::string*>(_scriptContext->GetReturnObject())));
  }
  return ("That does nothing");
}

/*
 * Inventory
 */
void Inventory::LoadInventory(DynamicObject* object)
{
  ForEach(object->inventory, [this](std::pair<std::string, int> data)
  {
    Data      objectTree = Level::CurrentLevel->GetItems();
    DataTree  objectTmp;
    DataTree* dataTree = DataTree::Factory::StringJSON(data.first);
    {
      Data      data_(dataTree);
      Data*     objectBuilder = new Data(dataTree);

      if (!data_["type"].Nil())
      {
	Data      object(&objectTmp);

	object.Duplicate(objectTree[data_["type"].Value()]);
	if (!(data_["ammo"].Nil()))
	{
	  if (!(data_["ammo"]["ammount"].Nil()))
	    object["ammo"]["ammount"] = data_["ammo"]["ammount"];
	  if (!(data_["ammo"]["current"].Nil()))
	    object["ammo"]["current"] = data_["ammo"]["current"];
	}
	delete objectBuilder;
	objectBuilder = new Data(object);
      }
      for (int i = 0 ; i < data.second ; ++i)
      {
	InventoryObject* newObject = new InventoryObject(*objectBuilder);

	AddObject(newObject);
      }
      delete objectBuilder;
    }
    delete dataTree;
  });
}

void Inventory::SaveInventory(DynamicObject* object)
{
  Content::iterator it  = _content.begin();
  Content::iterator end = _content.end();
  
  object->inventory.clear();
  for (; it != end ; ++it)
  {
    Content::iterator groupIt  = _content.begin();
    InventoryObject&  item     = **it;
    bool              ignore   = true;
    int               quantity = 1;

    for (; groupIt != end ; ++groupIt)
    {
      if (groupIt == it && quantity == 1)
	ignore = false;
      if (item.IsGroupableWith(*groupIt))
	quantity++;
    }
    if (!ignore)
    {
      std::string str;

      DataTree::Writers::StringJSON(item, str);
      object->inventory.push_back(std::pair<std::string, int>(str, quantity));
    }
  }
}

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
