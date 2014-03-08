#include "level/inventory.hpp"
#include "level/level.hpp"
#include "level/objects/locker.hpp"
#include "ui/alert_ui.hpp"
#include <gametask.hpp>
#include <gametask.hpp>
#include <algorithm>
#define USE_DEBUG_OUTPUT

using namespace std;

InventoryObject::InventoryObject(Data data) : Data(&_dataTree), _object("scripts/objects/" + data["script"]["file"].Value())
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

  Data script = data["script"];

  if (!(script.Nil()))
  {
    if (script["file"].Value() != "")
    {
      _object.asDefineMethod("CanWeild", "bool CanWeild(Item@, Character@, string, int)");
      _object.asDefineMethod("SetEquiped", "void SetEquiped(Item@, Character@, bool)");
    }
  }

  std::for_each(data["actions"].begin(), data["actions"].end(), [this](Data action)
  {
    AngelScript::Object     hooks(this->_object.GetContext(), this->_object.GetModule());
    Data                    action_data  = (*this)["actions"][action.Key()];
    function<bool (string)> sanity_check = [&action](string name) -> bool { return (action[name].NotNil() && action[name].Value() != ""); };

    action_data.Duplicate(action);
    if (action_data["targeted"].Nil())
      action_data["targeted"] = 1;
    if (sanity_check("hookUse"))
      hooks.asDefineMethod("Use",            "bool " + action["hookUse"].Value() + "(Item@, Character@)");
    if (sanity_check("hookCharacters"))
      hooks.asDefineMethod("UseOnCharacter", "bool " + action["hookCharacters"].Value() + "(Item@, Character@, Character@)");
    if (sanity_check("hookDoors"))
      hooks.asDefineMethod("UseOnDoor",      "bool " + action["hookDoors"].Value() + "(Item@, Character@, Door@)");
    if (sanity_check("hookOthers"))
      hooks.asDefineMethod("UseOnOthers",    "bool " + action["hookOthers"].Value() + "(Item@, Character@, DynamicObject@)");
    if (sanity_check("hookWeapon"))
      hooks.asDefineMethod("UseAsWeapon",    "bool " + action["hookWeapon"].Value() + "(Item@, Character@, Character@)");
    if (sanity_check("hookHitChances"))
      hooks.asDefineMethod("HitChances",     "int " + action["hookHitChances"].Value() + "(Item@, Character@, Character@)");
    if (sanity_check("hookCanUse"))
      hooks.asDefineMethod("CanUse",         "bool " + action["hookCanUse"].Value() + "(Item@, Character@, DynamicObject@)");
    if (sanity_check("hookActionPoints"))
      hooks.asDefineMethod("ActionPointCost", "int " + action["hookActionPoints"].Value() + "(Item@, Character@, DynamicObject@)");
    _actionHooks.push_back(hooks);
  });
}

InventoryObject::~InventoryObject()
{
}

bool InventoryObject::IsGroupableWith(const InventoryObject* other) const
{
  return (GetName() == other->GetName());
}

void InventoryObject::SetEquiped(ObjectCharacter* character, bool set)
{
  if (_equiped != set && _object.IsDefined("SetEquiped"))
  {
    AngelScript::Type<InventoryObject*> this_param(this);
    AngelScript::Type<ObjectCharacter*> character_param(character);
    AngelScript::Type<bool>             set_param(set);

    _object.Call("SetEquiped", 3, &this_param, &character_param, &set_param);
  }
  _equiped = set;
}

bool InventoryObject::CanWeild(ObjectCharacter* character, std::string slot, unsigned char mode)
{
  if (_object.IsDefined("CanWeild"))
  {
    AngelScript::Type<InventoryObject*> this_param(this);
    AngelScript::Type<ObjectCharacter*> character_param(character);
    AngelScript::Type<std::string*>     slot_param(&slot);
    AngelScript::Type<int>              mode_param(mode);

    return (_object.Call("CanWeild", 4, &this_param, &character_param, &slot_param, &mode_param));
  }
  return (false);
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

  _modelName   = object["model"].Value();
  for (unsigned short i = 0 ; i < _modelName.size() ; ++i)
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
  {
    float scale = object["scale"];

    if (object["scale"].NotNil())
      np.set_scale(scale);
  }
  {
    Data offset   = object["offset"];
    Data rotation = object["rotation"];

    if (offset.NotNil())
    {
      LPoint3f position(0, 0, 0);

      if (offset["x"].NotNil())
        position.set_x(offset["x"]);
      if (offset["y"].NotNil())
        position.set_y(offset["y"]);
      if (offset["z"].NotNil())
        position.set_z(offset["z"]);
      np.set_pos(position);
    }
    if (rotation.NotNil())
    {
      LPoint3f position(0, 0, 0);

      rotation.Output();
      if (rotation["x"].NotNil())
        position.set_x(rotation["x"]);
      if (rotation["y"].NotNil())
        position.set_y(rotation["y"]);
      if (rotation["z"].NotNil())
        position.set_z(rotation["z"]);
      np.set_hpr(position);
    }
  }
  if (texture != "")
  {
    Texture*    texfile = TexturePool::load_texture(TEXT_ROOT + texture);

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
  if (object)
  {
    object->interactions = Interactions::Use;
    object->waypoint     = 0;
    object->type         = DynamicObject::Item;
  }
  else
    AlertUi::NewAlert.Emit("Could not load graphical assets for item " + Key());
  return (object);
}

bool              InventoryObject::CanUse(ObjectCharacter* user, InstanceDynamicObject* target, unsigned int use_type)
{
  AngelScript::Object& hooks = _actionHooks[use_type];

  if (hooks.IsDefined("CanUse"))
  {
    AngelScript::Type<InventoryObject*>       this_param(this);
    AngelScript::Type<ObjectCharacter*>       user_param(user);
    AngelScript::Type<InstanceDynamicObject*> target_param(target);

    return (hooks.Call("CanUse", 3, &this_param, &user_param, &target_param));
  }
  return (true);
}

int               InventoryObject::HitSuccessRate(ObjectCharacter* user, ObjectCharacter* target, unsigned char use_type)
{
  AngelScript::Object& hooks = _actionHooks[use_type];

  if (hooks.IsDefined("HitChances"))
  {
    AngelScript::Type<InventoryObject*> this_param(this);
    AngelScript::Type<ObjectCharacter*> user_param(user);
    AngelScript::Type<ObjectCharacter*> target_param(target);

    return (hooks.Call("HitChances", 3, &this_param, &user_param, &target_param));
  }
  return (0);
}

unsigned short    InventoryObject::GetActionPointCost(ObjectCharacter* user, unsigned char use_type)
{
  AngelScript::Object hooks = _actionHooks[use_type];

  if (hooks.IsDefined("ActionPointCost"))
  {
    AngelScript::Type<InventoryObject*> this_param(this);
    AngelScript::Type<ObjectCharacter*> user_param(user);
    AngelScript::Type<int>              use_type_param(use_type);

    return ((int)hooks.Call("ActionPointCost", 3, &this_param, &user_param, &use_type_param));
  }
  return ((*this)["actions"][use_type]["ap-cost"]);
}

bool InventoryObject::UseAsWeapon(ObjectCharacter* user, ObjectCharacter* target, unsigned char useType)
{
  return (ExecuteHook("UseAsWeapon", user, target, useType));
}

bool InventoryObject::UseOn(ObjectCharacter* user, InstanceDynamicObject* target, unsigned char useType)
{
  ObjectCharacter*     charTarget;
  Lockable*            lockTarget;
  AngelScript::Object& hooks = _actionHooks[useType];

  if (hooks.IsDefined("UseOnCharacter") && (charTarget = target->Get<ObjectCharacter>()) != 0)
    return (ExecuteHook("UseOnCharacter", user, charTarget, useType));
  if (hooks.IsDefined("UseOnDoor")      &&
     (((lockTarget = target->Get<ObjectDoor>()) != 0) ||
       (lockTarget = target->Get<ObjectLocker>()) != 0))
    return (ExecuteHook("UseOnDoor", user, lockTarget, useType));
  if (hooks.IsDefined("UseOnOthers"))
    return (ExecuteHook("UseOnOthers", user, target, useType));
  Level::CurrentLevel->GetLevelUi().GetMainBar().AppendToConsole(i18n::T("That does nothing"));
  return (false);
}

bool InventoryObject::Use(ObjectCharacter* user, unsigned char useType)
{
  return (ExecuteHook("Use", user, (ObjectCharacter*)0, useType));
}

template<class C>
bool InventoryObject::ExecuteHook(const std::string& hook, ObjectCharacter* user, C* target, unsigned char useType)
{
  try
  {
    AngelScript::Object& handle = _actionHooks[useType];

    if (handle.IsDefined(hook))
    {
      AngelScript::Type<InventoryObject*> this_param(this);
      AngelScript::Type<ObjectCharacter*> user_param(user);
      AngelScript::Type<C*>               target_param(target);

      if (target == nullptr)
        return ((bool)(handle.Call(hook, 2, &this_param, &user_param)));
      return ((bool)(handle.Call(hook, 3, &this_param, &user_param, &target_param)));
    }
    cout << "Method " << hook << " undefined" << endl;
    Level::CurrentLevel->GetLevelUi().GetMainBar().AppendToConsole(i18n::T("That does nothing"));
  }
  catch (const AngelScript::Exception& exception)
  {
    AlertUi::NewAlert.Emit("Script crashed: " + std::string(exception.what()));
  }
  return (false);
}

/*
 * Inventory
 */
void Inventory::LoadItemFromData(Data item)
{
  InventoryObject* newObject    = new InventoryObject(item);
  Data             equiped_data = item["equiped-save"];

  if (equiped_data.NotNil() && equiped_data.Count() > 0)
  {
    Data equiped = (*newObject)["equiped"];
    Data source  = equiped_data[0];

    equiped.Duplicate(source);
    equiped.SetKey("equiped");
    source.Remove();
  }
  (*newObject)["quantity"].Remove();
  (*newObject)["equiped-save"].Remove();
  AddObject(newObject);
}

void Inventory::LoadInventory(DynamicObject* object)
{
  std::for_each(object->inventory.begin(), object->inventory.end(), [this](std::pair<std::string, int> data)
  {
    Data      objectTree = GameTask::CurrentGameTask->GetItemIndex();
    DataTree* dataTree = DataTree::Factory::StringJSON(data.first);
    {
      Data    item_data(dataTree);

      data.second = data.second > 9999 ? 1 : data.second; // Do not let object quantity go above 9999
      item_data.SetKey(item_data["Name"]);
      cout << "Loading " << data.second << " instances of " << item_data.Key() << endl;
      for (int i = 0 ; i < data.second ; ++i)
        LoadItemFromData(item_data);
    }
    delete dataTree;
  });
  InitializeSlots();
}

void Inventory::LoadInventory(Data items)
{
  _content.clear();
  for_each(items.begin(), items.end(), [this](Data item)
  {
    unsigned int     quantity;

    quantity = item["quantity"].NotNil() ? (unsigned int)item["quantity"] : 1;
    for (unsigned short i = 0 ; i < quantity ; ++i)
      LoadItemFromData(item);
  });
  InitializeSlots();
}

void Inventory::InitializeSlots(void)
{
  _slots.clear();
  for_each(_content.begin(), _content.end(), [this](InventoryObject* ptr)
  {
    InventoryObject& item = *ptr;
    Data             slot = item["equiped"];

    if (slot.NotNil())
      SetEquipedItem(slot["target"], slot["slot"], ptr, (unsigned short)slot["mode"]);
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
    int               quantity = 0;
    std::vector<Data> equip_data;

    for (; groupIt != end ; ++groupIt)
    {
      if (groupIt == it && quantity == 0)
        ignore = false;
      if (item.IsGroupableWith(*groupIt))
        quantity++;
      if (item["equiped"].NotNil())
        equip_data.push_back(item["equiped"]);
    }
    if (quantity == 0) quantity = 1;
    if (!ignore)
    {
      std::string str;

      item["Name"] = item.Key();
      do
      {
        // Equiped items have unique data tagged to them, so they need
        // to be saved in a separate container. Other items are saved once
        if (equip_data.size() > 0)
        {
          item["equiped_save"]["new"].Duplicate(*equip_data.begin());
          equip_data.erase(equip_data.begin());
          DataTree::Writers::StringJSON(item, str);
          item["equiped-save"].Remove();
          object->inventory.push_back(std::pair<std::string, int>(str, 1));
          quantity--;
        }
        else
        {
          DataTree::Writers::StringJSON(item, str);
          object->inventory.push_back(std::pair<std::string, int>(str, quantity));
        }
      } while (equip_data.size() > 0);
    }
  }
}

void Inventory::SaveInventory(Data items)
{
  Content::iterator it  = _content.begin();
  Content::iterator end = _content.end();

  for (; it != end ; ++it)
  {
    Content::iterator groupIt  = _content.begin();
    InventoryObject&  item     = **it;
    bool              ignore   = true;
    int               quantity = 0;
    std::vector<Data> equip_data;

    for (; groupIt != end ; ++groupIt)
    {
      if (groupIt == it && quantity == 0)
	ignore = false;
      if (item.IsGroupableWith(*groupIt))
	quantity++;
      if (item["equiped"].NotNil())
        equip_data.push_back(item["equiped"]);
    }
    if (quantity == 0) quantity = 1;
    if (!ignore)
    {
      for (unsigned int i = 0 ; i < equip_data.size() ; ++i)
      {
        item["equiped-save"]["new"].Duplicate(equip_data[i]);
        item["equiped-save"]["new"].SetKey("");
      }
      item["quantity"] = quantity;
      items[item.Key()].RemoveAllChildren();
      items[item.Key()].Duplicate(item);
      item["quantity"].Remove();
      item["equiped-save"].Remove();
    }
  }
}

InventoryObject* Inventory::AddObject(const string& name)
{
  Data items = GameTask::CurrentGameTask->GetItemIndex();
  
  if (items[name].NotNil())
  {
    InventoryObject* object = new InventoryObject(items[name]);

    AddObject(object);
    return (object);
  }
  return (0);
}

void Inventory::AddObject(InventoryObject* toAdd)
{
  Data weight = (*toAdd)["weight"];

  if (!(weight.Nil()))
    _currentWeight += (unsigned short)((*toAdd)["weight"]);
  (*toAdd)["equiped"].Remove();
  _content.push_back(toAdd);
  ContentChanged.Emit();
}

void Inventory::DelObject(InventoryObject* toDel)
{
  Content::iterator it = std::find(_content.begin(), _content.end(), toDel);

  if (it != _content.end())
  {
    Data weight  = (*toDel)["weight"];
    Data equiped = (*toDel)["equiped"];

    if (!(weight.Nil()))
      _currentWeight -= (unsigned short)((*toDel)["weight"]);
    if (equiped.NotNil())
    {
      SetEquipedItem(equiped["target"], equiped["slot"], 0, 0);
      (*toDel)["equiped"].Remove();
    }
    _content.erase(it);
    ContentChanged.Emit();
  }
}

bool Inventory::IncludesObject(InventoryObject* obj) const
{
  Content::const_iterator it = std::find(_content.begin(), _content.end(), obj);

  return (it != _content.end());
}

int              Inventory::GetObjectIterator(InventoryObject* object) const
{
  auto it  = _content.begin();
  auto end = _content.end();

  for (int i = 0 ; it != end ; ++it, ++i)
  {
    InventoryObject* comp = *it;

    if (object == comp)
      return (i);
  }
  return (-1);
}

InventoryObject* Inventory::GetObject(const std::string& name)
{
  Content::iterator it  = _content.begin();
  Content::iterator end = _content.end();
  
  for (; it != end ; ++it)
  {
    if ((*(*it)).GetName() == name)
      return (*it);
  }
  return (0);
}

unsigned short Inventory::ContainsHowMany(const std::string& name) const
{
  Content::const_iterator it    = _content.begin();
  Content::const_iterator end   = _content.end();
  unsigned short          count = 0;

  for (; it != end ; ++it)
  {
    if ((*(*it)).GetName() == name)
      count++;
  }
  return (count);
}

bool Inventory::CanCarry(InventoryObject* object, unsigned short quantity)
{
  return ((_capacity >= _currentWeight + (unsigned short)((*object)["weight"]) * quantity) || (_capacity == 0));
}

//
// Slots
//

// This here contraption allows to duplicate a single method to generate a const
// and a non-const getter at the same time. Might be worth moving into Boots.
#define DECLARE_GETTER(return_type, name, parameters, code) \
const return_type name parameters const \
code \
return_type name parameters \
code

Inventory::Slot& Inventory::GetItemSlot(const std::string& type_slot, unsigned int slot)
{
  auto it = std::find(_slots.begin(), _slots.end(), type_slot);

  if (it == _slots.end())
  {
    _slots.push_back(Slots(type_slot));
    return (GetItemSlot(type_slot, slot));
  }
  /*if (slot >= it->size())
    it->resize(slot + 1);*/
  {
    Inventory::Slot& obj = (*it)[slot];

    return (obj);
  }
}

const Inventory::Slot& Inventory::GetConstItemSlot(const std::string& type_slot, unsigned int slot) const
{
  auto it = std::find(_slots.begin(), _slots.end(), type_slot);

  if (it == _slots.end())
    throw std::domain_error("Item slot not found");
  return ((*it)[slot]);
}

InventoryObject*       Inventory::GetEquipedItem(const std::string& type_slot, unsigned int slot)
{
  try
  {
    return (GetConstItemSlot(type_slot, slot).object);
  }
  catch (const std::domain_error&)
  {
    return (0);
  }
}

unsigned char          Inventory::GetEquipedMode(const std::string& type_slot, unsigned int slot) const
{
  try
  {
    return (GetConstItemSlot(type_slot, slot).mode);
  }
  catch (...)
  {
    return (0);
  }
}

bool                   Inventory::SlotHasEquipedItem(const std::string& type_slot, unsigned int slot) const
{
  try
  {
    return (!(GetConstItemSlot(type_slot, slot).empty));
  }
  catch (...)
  {
    return (false);
  }
}

void                   Inventory::SetEquipedItem(const std::string& type_slot, unsigned int it_slot, InventoryObject* object, unsigned char equip_mode)
{
  Inventory::Slot& slot = GetItemSlot(type_slot, it_slot);

  if (slot.object)
  {
    Data data = *slot.object;

    data["equiped"].Remove();
    UnequipedItem.Emit(type_slot, it_slot, slot.object);
  }
  if (object)
  {
    Data data = *object;

    data["equiped"]["target"] = type_slot;
    data["equiped"]["slot"]   = it_slot;
    data["equiped"]["mode"]   = (unsigned short)equip_mode;
  }
  slot.mode   = equip_mode;
  slot.object = object;
  slot.empty  = object == 0;
  EquipedItem.Emit(type_slot, it_slot, object);
}
