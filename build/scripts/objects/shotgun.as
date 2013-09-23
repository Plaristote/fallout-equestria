#include "objectsMain.as"

// Modes:
// 0 -> EquipedMouth
// 1 -> EquipedMagic
// 2 -> EquipedBattleSaddle
bool CanWeild(Item@ item, Character@ user, string slot, int mode)
{
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

bool ShotgunShoot(Item@ item, Character@ user, Character@ target)
{
  return (Shoot(item, user, target));
}

bool CanUse(Item@ item, Character@ user, DynamicObject@ target)
{
  Data item_data = item.AsData();

  if (GetAmmoAmount(item_data) < 1)
  {
    if (@user == @(level.GetPlayer()))
    {
      level.PlaySound("out-of-ammo");
      level.ConsoleWrite(item.GetName() + " is out of ammo.");
    }
    return (false);
  }
  return (true);
}
