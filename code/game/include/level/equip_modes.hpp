#ifndef  EQUIP_MODES_HPP
# define EQUIP_MODES_HPP

# include "globals.hpp"
# include "datatree.hpp"
# include "level/objects/character.hpp"
# include "level/inventory.hpp"
# include <functional>
# include <map>

class EquipModes
{
public:
  EquipModes(void);
  ~EquipModes(void);

  void SearchForUserOnItemWithSlot(ObjectCharacter* user, InventoryObject* item, const std::string& slot);
  void Clear(void);
  void Foreach(std::function<void (unsigned char, const std::string&)> callback);

  bool HasOptions(void) const { return (modes.size() > 0); }
  bool HasChoice(void)  const { return (modes.size() > 1); }

private:
  DataTree*                            tree_equip_modes;
  std::map<unsigned char, std::string> modes;
};

#endif