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

void SetEquiped(Item@ item, Character@ user, string slot, int mode, string joint, bool set_equiped)
{
  Cout("SetEquiped " + slot + " shotgun" + mode);
  Data data = item.AsData();

  if (slot == "equiped")
  {
    if (mode == 0)
    {
      string task_name = "equiped-" + data["equiped"]["target"].AsString() + "-" + data["equiped"]["slot"].AsString();
    
      if (set_equiped)
      {
        Task@ task = user.GetTaskSet().PushTask(task_name);

        Cout("Adding gfx");
        task.AddGfx("sparkle", joint);
      }
      else
        user.GetTaskSet().RemoveTask(task_name);
    }
  }
}

bool ShotgunShoot(Item@ item, Character@ user, Character@ target)
{
  Cout("Shooting shotgun");
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
      level.AppendToConsole(item.GetName() + " is out of ammo.");
    }
    return (false);
  }
  return (true);
}
