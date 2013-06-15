#include "objectsMain.as"

// Modes:
// 0 -> EquipedMouth
// 1 -> EquipedMagic
// 2 -> EquipedBattleSaddle
bool CanWeild(Item@ item, Character@ user, string slot, int mode)
{
  Cout("CAN WEILD HOOK CALLED FFS");
  if (slot == "equiped")
  {
    if (mode == 1)
      return (true);
    else if (mode == 2)
    {
      // Check the user's battlesaddle
      return (true);
    }
  }
  return (false);
}

