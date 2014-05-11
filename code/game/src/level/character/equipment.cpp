#include "level/objects/character.hpp"
#include "level/level.hpp"

using namespace std;

ObjectCharacter::Equipment::~Equipment()
{
  for_each(equiped_items.begin(), equiped_items.end(), [](EquipedItem equiped_item)
  { delete equiped_item.render; });
}

ObjectCharacter::EquipedItem* ObjectCharacter::Equipment::GetEquipedItem(InventoryObject *item)
{
  auto it  = equiped_items.begin();
  auto end = equiped_items.end();

  for (; it != end ; ++it)
  {
    if (it->item == item)
      return (&(*it));
  }
  return (0);
}

ObjectCharacter::EquipedItem* ObjectCharacter::Equipment::GetEquipedItem(const string& slot, unsigned short slot_number)
{
  auto it  = equiped_items.begin();
  auto end = equiped_items.end();

  for (; it != end ; ++it)
  {
    Data equiped_data = (*it->item)["equiped"];

    if (equiped_data["target"].Value() == slot &&
        equiped_data["slot"] == slot_number)
      return (&(*it));
  }
  return (0);
}

void ObjectCharacter::Equipment::SetInventory(Inventory *inventory)
{
  if (this->inventory)
    DisconnectAll();
  this->inventory = inventory;
  if (inventory)
  {
    Connect(inventory->EquipedItem,   *this, &ObjectCharacter::Equipment::EquipItem);
    Connect(inventory->UnequipedItem, *this, &ObjectCharacter::Equipment::UnequipItem);
    inventory->InitializeSlots();
    inventory->GetItemSlot("equiped", 0);
    inventory->GetItemSlot("equiped", 1);
    inventory->ForeachSlot([this](Inventory::Slots& slot)
    {
      for (unsigned int i = 0 ; i < slot.SlotCount() ; ++i)
        EquipDefaultItemForSlot(slot.GetName(), i);
    });
  }
}

void ObjectCharacter::Equipment::EquipDefaultItemForSlot(const std::string& slot, unsigned short slot_number)
{
  if ((inventory->GetEquipedItem(slot, slot_number)) == 0)
  {
    string default_item = GetDefaultItemForSlot(slot, slot_number);

    cout << "equiping default item " << default_item << endl;
    if (default_item != "")
    {
      InventoryObject* item = inventory->GetObject(default_item);

      if (!item)
        item = inventory->AddObject(default_item);
      if (item)
        inventory->SetEquipedItem(slot, slot_number, item);
    }
  }
  else
    cout << "slot " << slot_number << " already occupied" << endl;
}

void ObjectCharacter::Equipment::EquipItem(InventoryObject *item)
{
  EquipedItem   equiped_item;
  std::string   slot_type     = (*item)["equiped"]["target"].Value();
  unsigned char slot_number   = (int)((*item)["equiped"]["slot"].Or(0));
  NodePath      joint         = GetJointForSlot(slot_type, slot_number);

  cout << "Equipment::EquipItem " << item->GetName() << endl;
  if (joint.is_empty())
    cout << "Joint for slot " << slot_type << '#' << slot_number << " does not exist." << endl;
  equiped_item.item           = item;
  equiped_item.current_action = 0;
  equiped_item.render         = item->CreateEquipedModel(character.GetLevel()->GetWorld());
  equiped_item.slot           = &(inventory->GetItemSlot(slot_type, slot_number));
  if (equiped_item.render)
    equiped_item.render->GetNodePath().reparent_to(joint);
  else
    cout << "Item " << item->GetName() << " has no render" << endl;
  equiped_item.item->SetEquiped(&character, true);
  ClearSlot(slot_type, slot_number);
  equiped_items.push_back(equiped_item);
  if (slot_type == "equiped") // Quick access interface update
    character.EquipedItemChanged.Emit(slot_number, item);
}

void ObjectCharacter::Equipment::ClearSlot(const std::string& slot, unsigned short slot_number)
{
  auto it  = equiped_items.begin();
  auto end = equiped_items.end();

  for (; it != end ; ++it)
  {
    Data equiped_data = (*it->item)["equiped"];

    if (equiped_data["target"].Value() == slot &&
        equiped_data["slot"] == slot_number)
    {
      equiped_items.erase(it);
      break ;
    }
  }
}

void ObjectCharacter::Equipment::UnequipItem(InventoryObject *item)
{
  auto it  = equiped_items.begin();
  auto end = equiped_items.end();

  for (; it != end ; ++it)
  {
    if (it->item == item)
    {
      Data equiped_data =(*item)["equiped"];

      it->item->SetEquiped(&character, false);
      if (it->render)
        delete it->render;
      equiped_items.erase(it);
      EquipDefaultItemForSlot(equiped_data["target"].Value(), equiped_data["slot"]);
      return ;
    }
  }
}

string ObjectCharacter::Equipment::GetDefaultItemForSlot(const string &slot, unsigned short slot_number)
{
  if (character.script != 0)
  {
    stringstream function_name;

    function_name << "default_item_for_" << slot << "_" << (slot_number + 1);
    if (!character.script->IsDefined(function_name.str()))
      character.script->asDefineMethod(function_name.str(), "string " + function_name.str() + "(Character@)");
    if (character.script->IsDefined(function_name.str()))
    {
      AngelScript::Type<ObjectCharacter*> self(&character);

      return (*(string*)(character.script->Call(function_name.str(), 1, &self)));
    }
  }
  return ("");
}

NodePath ObjectCharacter::Equipment::GetJointForSlot(const string &slot_name, unsigned short slot_number)
{
  if (inventory)
  {
    Inventory::Slot&         slot        = inventory->GetItemSlot(slot_name, slot_number);
    PT(CharacterJointBundle) body_bundle = character._character->get_bundle(0);
    PT(CharacterJoint)       joint;
    stringstream             joint_name;
    string                   equip_mode_name = character.GetLevel()->GetEquipModes().GetNameForMode(slot.mode);

    /*joint_name << slot;
    if (equip_mode_name != "")
      joint_name << '-' << equip_mode_name;
    joint_name << '#' << (slot_number + 1);*/
    joint_name << "Horn"; // TODO Get models with the proper joints
    joint = character._character->find_joint(joint_name.str());
    if (joint)
    {
      NodePath body_node  = character.GetNodePath().find("**/+Character");
      NodePath joint_node = body_node.attach_new_node(joint_name.str());

      body_bundle->control_joint(joint_name.str(), joint_node.node());
      return (joint_node);
    }
  }
  return (NodePath());
}
