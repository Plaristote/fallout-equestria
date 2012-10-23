
string UseKeyOnDoor(Item@ item, Character@ user, Door@ door)
{
  if (door.IsOpen() && door.IsLocked() == false)
    return ("Close the door first");
  if (door.GetKeyName() == item.GetName())
  {
    string postStr = (door.IsLocked() ? " unlocked a door" : " locked a door");

    door.Unlock();
    if (door.IsLocked())
      return (user.GetName() + postStr);
    else
      return ("FATAL ERROR UseKeyOnDoor");
  }
  return ("The door won't open");
}

string ReloadWeapon(Item@ item, Character@ user)
{
  Inventory@ inventory   = user.GetInventory();
  Data       itemData    = item.AsData();
  string     currentAmmo = itemData["ammo"]["current"].AsString();
  int        ammountAmmo = itemData["ammo"]["ammount"].AsInt();
  int        maximumAmmo = itemData["ammo"]["maximum"].AsInt();
  Item@      nextAmmunition;

  @nextAmmunition = inventory.GetObject(currentAmmo);
  // If no ammunition of that type left, and barrel is empty, check other types of ammo
  if (@nextAmmunition == null && ammountAmmo == 0)
  {
    Data     ammoTypes = itemData["ammo"]["types"];
    int      typeCount = ammoTypes.Count();
    int      i         = 0;

    while (i < typeCount)
    {
      string typeName  = ammoTypes[i].AsString();

      if (typeName == currentAmmo)
        continue ;
      @nextAmmunition = inventory.GetObject(typeName);
      if (@nextAmmunition != null)
      {
        currentAmmo                 = typeName;
        itemData["ammo"]["current"] = currentAmmo;
        break ;
      }
      i++;
    }
  }

  // If still no ammunition object fetched, impossible to reload
  if (@nextAmmunition == null)
    return ("Out of ammo");

  while (@nextAmmunition != null && ammountAmmo <= maximumAmmo)
  {
    inventory.DelObject(nextAmmunition);
    @nextAmmunition = inventory.GetObject(currentAmmo);
    ammountAmmo++;
  }

  return ("");
}
