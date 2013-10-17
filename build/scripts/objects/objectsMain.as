
bool UseKeyOnDoor(Item@ item, Character@ user, Door@ door)
{
  if (door.IsOpen() && door.IsLocked() == false)
  {
    level.ConsoleWrite("Close the door first");
    return (false);
  }
  if (door.GetKeyName() == item.GetName())
  {
    bool   current_state = door.IsLocked();
    string postStr       = (current_state ? " unlocked a door" : " locked a door");

    door.Unlock();
    if (door.IsLocked() != current_state)
      level.ConsoleWrite(user.GetName() + postStr);
    else
      level.ConsoleWrite("FATAL ERROR UseKeyOnDoor");
    return (true);
  }
  level.ConsoleWrite("The door won't open");
  return (true);
}

int UnarmedSuccessChance(Item@ item, Character@ user, Character@ target)
{
  Data  user_stats   = user.GetStatistics();
  Data  target_stats = target.GetStatistics();
  int   perception   = user_stats["Special"]["PER"].AsInt();
  int   skill        = user_stats["Skills"]["Unarmed"].AsInt();
  int   armor_class  = target_stats["Statistics"]["Armor Class"].AsInt();
  float distance     = user.GetDistance(target.AsObject());
  int   hit_chances;

  hit_chances = skill - 30 + (perception - 2) * 16 - (distance / 2) - armor_class;
  if (hit_chances < 5)
    hit_chances = 5;
  else if (hit_chances > 95)
    hit_chances = 95;
  return (hit_chances);
}

int ShootSuccessChance(Item@ item, Character@ user, Character@ target)
{
  Data   item_data   = item.AsData();
  Data   action      = item_data["actions"]["Shoot"];
  float  range       = action["range"].AsFloat();
  float  distance    = user.GetDistance(target.AsObject());
  string type        = action["type"].AsString();
  int    skill       = user.GetStatistics()["Skills"][type].AsInt();
  int    armor_class = target.GetStatistics()["Statistics"]["Armor Class"].AsInt();
  int    precision   = 0;

  if (range < distance)
    return (0);
  else
  {
    if (!(action["precision"].Nil()))
      precision        = action["precision"].AsInt();
    //Cout("Skill Points: " + skill);
    //Cout("Distance:     " + distance);
    int hit_chances = 10 + skill - distance + precision - armor_class;

    //Cout("Precision:   " + precision);
    //Cout("Armor Class: " + armor_class);
    //Cout("Hit Chances: " + hit_chances);
    if (hit_chances > 95)
      hit_chances = 95;
    if (hit_chances < 0)
      hit_chances = 0;
    return (hit_chances);
  }
}

int DamageCalculation(Item@ item, Character@ user, Character@ target, int critical_roll)
{
  Data   statistics          = user.GetStatistics();
  Data   item_data           = item.AsData();
  Data   action              = item_data["actions"]["Shoot"];
  int    min_damage          = action["damage"].AsInt();
  int    max_damage          = action["damage-max"].AsInt();
  int    damage              = min_damage + (Random() % (max_damage - min_damage));
  int    critical_damage     = 2;
  int    armor_resistance    = 0; // To implement
  int    resistance_modifier = 0; // To implement

  damage += statistics["Statistics"]["Bonus Damage"].AsInt();
  {
    int critical_chance = statistics["Statistics"]["Critical Chance"].AsInt();

    if (critical_roll <= critical_chance)
      critical_damage = 2 + critical_roll % 5;
  }
  damage *= critical_damage;
  damage /= 2;
  damage *= ((100 - (armor_resistance + resistance_modifier)) / 100);
  return (damage);
}

bool Shoot(Item@ item, Character@ user, Character@ target)
{
  int  ap        = user.GetActionPoints();
  Data item_data = item.AsData();
  Data action    = item_data["actions"]["Shoot"];
  int  ap_cost   = action["ap-cost"].AsInt();
  int  ammo      = GetAmmoAmount(item_data);

  if (ap_cost <= ap)
  {
    float range    = action["range"].AsFloat();
    float distance = user.GetDistance(target.AsObject());
    int   success_rate;
    int   roll     = Random() % 100;

    if (distance > range)
    {
      level.ConsoleWrite("Out of range");
      return (false);
    }
    user.SetActionPoints(ap - ap_cost);
    SetAmmoAmount(item_data, ammo - 1);
    level.PlaySound("shoot/shotgun");
    success_rate   = ShootSuccessChance(item, user, target);
    if (roll <= success_rate)
    {
      int  critical_roll = Random() % 100;
      int  damage        = DamageCalculation(item, user, target, critical_roll);
      bool is_critical   = critical_roll <= user.GetStatistics()["Statistics"]["Critical Chance"].AsInt();

      target.SetHitPoints(target.GetHitPoints() - damage);
      if (is_critical)
      {
        level.ConsoleWrite("You critically hit " + target.GetName() + " for " + damage + " Hit Points.");
        return (true);
      }
      else
      {
        level.ConsoleWrite("You hit " + target.GetName() + " for " + damage + " Hit Points.");
        return (true);
      }
    }
    level.ConsoleWrite("You missed");
    return (false);
  }
  level.ConsoleWrite("Not enough action points");
  return (false);
}

string GetAmmoType(Data item)
{
  return (item["ammo"]["current"].AsString());
}

int GetAmmoAmount(Data item)
{
  if (item["ammo"]["amount"].Nil())
    return (0);
  return (item["ammo"]["amount"].AsInt());
}

void SetAmmoType(Data item, string type)
{
  item["ammo"]["current"] = type;
}

void SetAmmoAmount(Data item, int amount)
{
  item["ammo"]["amount"] = amount;
}

bool ReloadWeapon(Item@ item, Character@ user)
{
  Inventory@ inventory   = user.GetInventory();
  Data       itemData    = item.AsData();
  int        ap_cost     = itemData["actions"]["Reload"]["ap-cost"].AsInt();
  string     currentAmmo = GetAmmoType(itemData);
  int        ammountAmmo = GetAmmoAmount(itemData);
  int        maximumAmmo = itemData["ammo"]["maximum"].AsInt();
  Item@      nextAmmunition;

  if (ammountAmmo == maximumAmmo)
  {
    level.ConsoleWrite(item.GetName() + " is already fully loaded.");
    return (false);
  }
  if (user.GetActionPoints() < ap_cost)
  {
    level.ConsoleWrite("Not enough action points");
    return (false);
  }
  @nextAmmunition = inventory.GetObject(currentAmmo);
  // If no ammunition of that type left, and barrel is empty, check other types of ammo
  if (@nextAmmunition == null && ammountAmmo == 0)
  {
    Data     ammoTypes = itemData["ammo"]["types"];
    int      typeCount = ammoTypes.Count();
    int      i         = 0;

    if (ammoTypes.Nil())
      typeCount = 0;
    while (i < typeCount)
    {
      string typeName  = ammoTypes[i].AsString();

      if (typeName != currentAmmo)
      {
        @nextAmmunition = inventory.GetObject(typeName);
        if (@nextAmmunition != null)
        {
          currentAmmo                 = typeName;
          itemData["ammo"]["current"] = currentAmmo;
          break ;
        }
      }
      i++;
    }
  }

  // If still no ammunition object fetched, impossible to reload
  if (@nextAmmunition == null)
  {
    level.PlaySound("out-of-ammo");
    level.ConsoleWrite("Out of ammo");
    return (false);
  }

  while (@nextAmmunition != null && ammountAmmo < maximumAmmo)
  {
    inventory.DelObject(nextAmmunition);
    @nextAmmunition = inventory.GetObject(currentAmmo);
    ammountAmmo++;
  }
  SetAmmoAmount(itemData, ammountAmmo);

  if (@user == @(level.GetPlayer()))
    level.ConsoleWrite(item.GetName() + " now loaded with " + ammountAmmo + "/" + maximumAmmo + " rounds.");
  level.PlaySound("reload/pistol");
  user.SetActionPoints(user.GetActionPoints() - ap_cost);
  return (true);
}
