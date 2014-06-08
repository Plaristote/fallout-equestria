
bool UseKeyOnDoor(Item@ item, Character@ user, Door@ door)
{
  if (door.IsOpen() && door.IsLocked() == false)
    door.Close();
  if (door.GetKeyName() == item.GetName())
  {
    bool   current_state = door.IsLocked();
    string postStr       = (current_state ? " unlocked a door" : " locked a door");

    door.Unlock();
    if (door.IsLocked() != current_state)
      level.AppendToConsole(user.GetName() + postStr);
    else
      level.AppendToConsole("FATAL ERROR UseKeyOnDoor");
  }
  level.AppendToConsole("The door won't open");
  return (true);
}

int get_statistic(Character@ user, string key)
{
  Data user_stats         = user.GetStatistics();
  Data statistic_variable = user_stats["Variables"][key];
  
  if (statistic_variable.Nil())
    statistic_variable = user_stats["Statistics"][key];
  return (statistic_variable.Nil() ? 0 : statistic_variable.AsInt());
}

int UnarmedSuccessChance(Item@ item, Character@ user, Character@ target)
{
  Data  user_stats   = user.GetStatistics();
  Data  target_stats = target.GetStatistics();
  int   perception   = user_stats["Special"]["PER"].AsInt();
  int   skill        = user_stats["Skills"]["Unarmed"].AsInt();
  int   armor_class  = get_statistic(user, "Armor Class");
  float distance     = user.GetDistance(target.AsObject());
  int   hit_chances;

  hit_chances = skill - 30 + (perception - 2) * 16 - ceil(distance / 2) - armor_class;
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
  Data   skill_data  = user.GetStatistics()["Skills"][type];
  Data   armor_data  = target.GetStatistics()["Variables"]["Armor Class"];
  int    precision   = 0;
  int    skill;
  int    armor_class;
  
  if (skill_data.Nil())
    skill = 100;
  else
    skill = skill_data.AsInt();
  if (armor_data.Nil())
    armor_data = target.GetStatistics()["Statistics"]["Armor Class"];
  if (armor_data.Nil())
    armor_class = 0;
  else
    armor_class = armor_data.AsInt();

  if (range >= distance)
  {
    if (!(action["precision"].Nil()))
      precision        = action["precision"].AsInt();
    //Cout("Skill Points: " + skill);
    //Cout("Distance:     " + distance);
    int hit_chances = 10 + skill - ceil(distance) + precision - armor_class;

    //Cout("Precision:   " + precision);
    //Cout("Armor Class: " + armor_class);
    //Cout("Hit Chances: " + hit_chances);
    if (hit_chances > 95)
      hit_chances = 95;
    if (hit_chances < 0)
      hit_chances = 0;
    return (hit_chances);
  }
  else
    Cout("Out of range");
  return (0);
}

int GetCharacterResistance(Character@ character, string attack_type)
{
  Data statistics = character.GetStatistics();
  Data resistance = statistics["Armor"]["Resistance"][attack_type];
  
  if (resistance.Nil())
    return (0);
  return (resistance.AsInt());
}

bool  ComputeIfCritical(Character@ user, int critical_roll)
{
  Data statistics = user.GetStatistics();

  return (critical_roll <= statistics["Statistics"]["Critical Chance"].AsInt());
}

float ComputeDamageResistance(Item@ item, string action_name, Character@ target, float damage)
{
  string damage_type         = "Melee";
  Data   statistics          = target.GetStatistics();
  int    resistance_modifier = get_statistic(target, "Damage Resistance");
  int    armor_resistance;

  if (@item != null)
  {
    Data damage_type_data = item.AsData()["actions"][action_name]["damage_type"];
    
    if (damage_type_data.NotNil())
      damage_type = damage_type_data.AsString();
  }
  armor_resistance = GetCharacterResistance(target, damage_type);
  damage           = damage - (damage / 100 * resistance_modifier);
  damage           = damage - (damage / 100 * armor_resistance);
  return (damage);
}

float ComputeCriticalDamage(float damage, int critical_roll)
{
  int critical_damage = 1 + critical_roll % 3;

  return (damage * critical_damage);
}

float ComputeBaseDamage(float min_damage, float max_damage)
{
  int rounded_damage_range = ceil(max_damage - min_damage);

  if (rounded_damage_range <= 0)
    rounded_damage_range = 1;
  return (min_damage + (Random() % rounded_damage_range));
}

float ComputeDamage(Item@ item, string action, Character@ user, Character@ target, int critical_roll)
{
  Data  statistics = user.GetStatistics();
  float min_damage, max_damage;
  float damage     = 0;

  if (@item == null)
  {
    max_damage = get_statistic(user, "Melee Damage");
    min_damage = max_damage / 1.5;
  }
  else
  {
    Data item_data  = item.AsData();
    Data damage_max = item_data["damage-max"];
    Data damage_min = item_data["damage"];

    if (damage_max.Nil())
      damage_max = get_statistic(user, "Melee Damage") + 2;
    if (damage_min.Nil())
      damage_min = get_statistic(user, "Melee Damage");
    max_damage = damage_max.Nil() ? 10 : damage_max.AsInt();
    min_damage = damage_min.Nil() ? 8  : damage_min.AsInt();
    Cout("Max Damage: " + max_damage);
    Cout("Min Damage: " + min_damage);
  }
  damage = ComputeBaseDamage(min_damage, max_damage);
  Cout("Base damage: " + damage);
  damage = ComputeDamageResistance(item, action, target, damage);
  Cout("Damage - resistance: " + damage);
  if (ComputeIfCritical(user, critical_roll))
    damage = ComputeCriticalDamage(damage, critical_roll);
  Cout("Damage after critical test: " + damage);
  return (damage);
}

int DamageCalculation(Item@ item, string action_name, Character@ user, Character@ target, int critical_roll)
{
  Data   statistics          = user.GetStatistics();
  Data   item_data           = item.AsData();
  Data   action              = item_data["actions"][action_name];
  int    min_damage          = action["damage"].AsInt();
  int    max_damage          = action["damage-max"].AsInt();
  int    damage              = min_damage + (Random() % (max_damage - min_damage));
  int    critical_damage     = 2;
  int    armor_resistance    = GetCharacterResistance(target, item_data["actions"][action_name]["damage_type"].AsString());
  int    resistance_modifier = 0; // To implement

  damage += statistics["Statistics"]["Bonus Damage"].AsInt();
  {
    int critical_chance = statistics["Statistics"]["Critical Chance"].AsInt();

    if (critical_roll <= critical_chance)
      critical_damage = 2 + critical_roll % 3;
  }
  damage *= critical_damage;
  damage /= 2;
  damage *= ((100 - (armor_resistance + resistance_modifier)) / 100);
  return (damage);
}

bool Shoot(Item@ item, Character@ user, Character@ target)
{
  Data   item_data = item.AsData();
  Data   action    = item_data["actions"]["Shoot"];
  float  range     = action["range"].AsFloat();
  float  distance  = user.GetDistance(target.AsObject());
  string user_name = user.GetName() == "self" ? "You" : user.GetName();

  if (distance > range)
    level.AppendToConsole("Out of range");
  else
  {
    int   ammo      = GetAmmoAmount(item_data);
    int   roll      = Random() % 100;
    int   success_rate;

    SetAmmoAmount(item_data, ammo - 1);
    level.PlaySound("shoot/shotgun");
    success_rate   = ShootSuccessChance(item, user, target);
    if (roll <= success_rate)
    {
      int  critical_roll = Random() % 100;
      int  damage        = ceil(ComputeDamage(item, "Shoot", user, target, critical_roll));
      bool is_critical   = critical_roll <= user.GetStatistics()["Statistics"]["Critical Chance"].AsInt();

      target.SetHitPoints(target.GetHitPoints() - damage);
      if (is_critical)
        level.AppendToConsole(user_name + " critically hit " + target.GetName() + " for " + damage + " Hit Points.");
      else
        level.AppendToConsole(user_name + " hit " + target.GetName() + " for " + damage + " Hit Points.");
      return (true);
    }
    else
      level.AppendToConsole("You missed");
  }
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
  string     currentAmmo = GetAmmoType(itemData);
  int        ammountAmmo = GetAmmoAmount(itemData);
  int        maximumAmmo = itemData["ammo"]["maximum"].AsInt();
  Item@      nextAmmunition;

  if (ammountAmmo == maximumAmmo)
  {
    level.AppendToConsole(item.GetName() + " is already fully loaded.");
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
    level.AppendToConsole("Out of ammo");
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
    level.AppendToConsole(item.GetName() + " now loaded with " + ammountAmmo + "/" + maximumAmmo + " rounds.");
  level.PlaySound("reload/pistol");
  return (true);
}
