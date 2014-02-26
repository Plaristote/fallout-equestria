#include "level/equip_modes.hpp"

using namespace std;

EquipModes::EquipModes(void)
{
  tree_equip_modes = DataTree::Factory::JSON("data/equip_modes.json");
}

EquipModes::~EquipModes(void)
{
  if (tree_equip_modes)
    delete tree_equip_modes;
}

void EquipModes::Clear(void)
{
  modes.clear();
}

void EquipModes::Foreach(function<void (unsigned char, const string&)> callback)
{
  auto it  = modes.begin();
  auto end = modes.end();
  
  for (; it != end ; ++it)
    callback(it->first, it->second);
}

void EquipModes::SearchForUserOnItemWithSlot(ObjectCharacter* user, InventoryObject* item, const string& slot)
{
  Data          equip_modes(tree_equip_modes);
  unsigned char counter = 0;

  Clear();
  for_each(equip_modes.begin(), equip_modes.end(), [this, user, item, slot, &counter](Data equip_mode)
  {
    if (item->CanWeild(user, slot, counter))
      modes.insert(std::pair<unsigned char, string>(counter, equip_mode.Value()));
    counter++;
  });
}