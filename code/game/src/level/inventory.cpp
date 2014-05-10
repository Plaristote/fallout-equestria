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

  _equiped = (*this)["equiped"].NotNil();

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

void InventoryObject::ResetFromFixture(void)
{
  DataTree* objects = DataTree::Factory::JSON("data/objects.json");

  if (objects)
  {
    {
      Data item_index(objects);
      Data item_data = item_index[GetName()];

      if (item_data.NotNil())
      {
        for_each(begin(), end(), [](Data value) { value.Remove(); });
        Duplicate(item_data);
      }
    }
    delete objects;
  }
}

bool InventoryObject::IsGroupableWith(const InventoryObject* other) const
{
  return (GetName() == other->GetName());
}

bool InventoryObject::IsWeapon(void) const
{
  Data actions = (*this)["actions"];

  if (actions.NotNil())
  {
    auto it  = actions.begin();
    auto end = actions.end();

    for (; it != end ; ++it)
    {
      if ((*it)["combat"] == 1)
        return (true);
    }
  }
  return (false);
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
  if (_actionHooks.size() > use_type)
  {
    AngelScript::Object& hooks = _actionHooks[use_type];

    if (hooks.IsDefined("HitChances"))
    {
      AngelScript::Type<InventoryObject*> this_param(this);
      AngelScript::Type<ObjectCharacter*> user_param(user);
      AngelScript::Type<ObjectCharacter*> target_param(target);

      return (hooks.Call("HitChances", 3, &this_param, &user_param, &target_param));
    }
  }
  else
    cout << "[HitSuccessRate] No action '" << (int)use_type << "' for item " << GetName() << endl;
  return (0);
}

unsigned short    InventoryObject::GetActionPointCost(ObjectCharacter* user, unsigned char use_type)
{
  if (_actionHooks.size() > use_type)
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
  else
    cout << "[GetActionPointCost] No action '" << (int)use_type << "' for item " << GetName() << endl;
  return (2);
}

bool InventoryObject::UseAsWeapon(ObjectCharacter* user, ObjectCharacter* target, unsigned char useType)
{
  return (ExecuteHook("UseAsWeapon", user, target, useType));
}

bool InventoryObject::UseOn(ObjectCharacter* user, InstanceDynamicObject* target, unsigned char useType)
{
  if (_actionHooks.size() > useType)
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
  }
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
  InventoryObject* newObject     = new InventoryObject(item);

  (*newObject)["quantity"].Remove();
  AddObject(newObject);
  if (item["equiped"].NotNil())
    (*newObject)["equiped"].Duplicate(item["equiped"]);
}

void Inventory::LoadInventory(DynamicObject* object)
{
  std::for_each(object->inventory.begin(), object->inventory.end(), [this, &object](std::pair<std::string, int> data)
  {
    DataTree* dataTree = DataTree::Factory::StringJSON(data.first);
    {
      Data    item_data(dataTree);

      data.second = data.second > 9999 ? 1 : data.second; // Do not let object quantity go above 9999
      item_data.SetKey(item_data["Name"].Or(item_data["type"].Value()));
      for (int i = 0 ; i < data.second ; ++i)
        LoadItemFromData(item_data);
    }
    delete dataTree;
  });
  InitializeSlots();
}

void Inventory::LoadInventory(Data items)
{
  Data      objectTree = GameTask::CurrentGameTask->GetItemIndex();

  _content.clear();
  for_each(items.begin(), items.end(), [this, objectTree](Data item)
  {
    unsigned int quantity;
    DataTree     new_item_tree;
    {
      Data       new_item(&new_item_tree);
      Data       model = objectTree[item["Name"].Value()];

      new_item.Duplicate(model);
      new_item.SetKey(item["Name"]);
      if (item["ammo"]["current"].NotNil())
        new_item["ammo"]["current"] = item["ammo"]["current"].Value();
      if (item["ammo"]["amount"].NotNil())
        new_item["ammo"]["amount"]  = item["ammo"]["amount"].Or(0);
      if (item["equiped"].NotNil())
        new_item["equiped"].Duplicate(item["equiped"]);
      quantity = item["quantity"].NotNil() ? (unsigned int)item["quantity"] : 1;
      if (quantity > 9999) { quantity = 1; }
      for (unsigned short i = 0 ; i < quantity ; ++i)
      {
        LoadItemFromData(new_item);
        new_item["equiped"].Remove();
      }
    }
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
      SetEquipedItem(slot["target"], slot["slot"], ptr, slot["mode"].Or(0));
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

    if (item["equiped"].Nil())
    {
      for (; groupIt != end ; ++groupIt)
      {
        if (groupIt == it)
        {
          if (quantity == 0)
          {
            ignore = false;
            quantity = 1;
          }
          else
            break ;
        }
        else if (item.IsGroupableWith(*groupIt) && (**groupIt)["equiped"].Nil())
          quantity++;
      }
    }
    else
      ignore = false;
    if (!ignore)
    {
      std::string str;

      if (quantity == 0) quantity = 1;
      item["Name"] = item.Key();
      DataTree::Writers::StringJSON(item, str);
      object->inventory.push_back(std::pair<std::string, int>(str, quantity));
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

DECLARE_GETTER(Inventory::Slot&,Inventory::GetItemSlot,(const std::string& type_slot, unsigned int slot),
{
 auto it = std::find(_slots.begin(), _slots.end(), type_slot);

 if (it == _slots.end())
 {
   _slots.push_back(Slots(type_slot));
   return (GetItemSlot(type_slot, slot));
 }
 else
 {
   Inventory::Slot& obj = (*it)[slot];

   return (obj);
 }
})

InventoryObject*       Inventory::GetEquipedItem(const std::string& type_slot, unsigned int slot)
{
  return (GetItemSlot(type_slot, slot).object);
}

unsigned char          Inventory::GetEquipedMode(const std::string& type_slot, unsigned int slot) const
{
  return (GetItemSlot(type_slot, slot).mode);
}

void                   Inventory::UnequipAllItems()
{
  auto it  = _slots.begin();
  auto end = _slots.end();

  for (; it != end ; ++it)
  {
    Inventory::Slots& slots = *it;

    for (unsigned int i = 0 ; i < slots.SlotCount() ; ++i)
      SetEquipedItem(slots.GetName(), i, 0, 0);
  }
}

bool                   Inventory::SlotHasEquipedItem(const std::string& type_slot, unsigned int slot) const
{
  return (!(GetItemSlot(type_slot, slot).empty));
}

void                   Inventory::SetEquipedItem(const string &type_slot, unsigned int slot, InventoryObject *object, const string &equip_mode)
{
  if (Level::CurrentLevel)
  {
    bool          found_equip_mode = false;
    unsigned char equip_mode_it;

    Level::CurrentLevel->GetEquipModes().SearchForUserOnItemWithSlot(0, 0, type_slot);
    Level::CurrentLevel->GetEquipModes().Foreach([&found_equip_mode, &equip_mode_it, equip_mode](unsigned char it, const std::string it_name)
    {
      if (it_name == equip_mode)
      {
        equip_mode_it    = it;
        found_equip_mode = true;
      }
    });
    if (found_equip_mode)
      SetEquipedItem(type_slot, slot, object, equip_mode_it);
  }
  else
    cout << "[Inventory] Can't call SetEquipedItem without a running level" << endl;
}

void                   Inventory::SetEquipedItem(const std::string& type_slot, unsigned int it_slot, InventoryObject* object, unsigned char equip_mode)
{
  Inventory::Slot& slot = GetItemSlot(type_slot, it_slot);

  if (slot.object)
  {
    InventoryObject* currently_equiped = slot.object;
    Data             data              = *currently_equiped;

    slot.object = 0;
    UnequipedItem.Emit(currently_equiped);
    data["equiped"].Remove();
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
  if (object)
    EquipedItem.Emit(object);
  ContentChanged.Emit();
}

void Inventory::ResetItemsFromFixtures()
{
  for_each(_content.begin(), _content.end(), [](InventoryObject* object)
  {
    object->ResetFromFixture();
  });
}
