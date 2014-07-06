#include "objectsMain.as"

bool CanWeild(Item@ item, Character@ user, string slot, int mode)
{
  Cout("bite");
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

int PulseRifleSuccessChance(Item@ item, Character@ user, Character@ target)
{
  return (95);
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

void EmpRadius(Item@ item, Character@ user, float x, float y, float z)
{
  ObjectList objects = level.GetObjectsInRadius(x, y, z, 50);
  int        i       = 0;

  while (i < objects.Size())
  {
    DynamicObject@ object    = objects[i];
    Character@     character = object.AsCharacter();
    
    if (@character != null)
    {
      Cout("Character in radius: " + character.GetName());
      level.AppendToConsole(character.GetName() + " critically hit for " + character.GetHitPoints());
      character.SetHitPoints(0);
    }
    i += 1;
  }
}

bool PulseRifleShoot(Item@ item, Character@ user, Character@ target)
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
      level.AppendToConsole(item.GetName() + " is out of ammo.");
    }
    return (false);
  }
  return (true);
}
