//
// CHARACTER CREATION STUFF
//

// This must contain every modification to the SPECIAL attributes when a trait is activated/disactivated
bool DoActivateTrait(Data sheet, string trait, bool setActivated, int traitsLeft)
{
  Data derivedStatistics = sheet["Statistics Modifiers"];
  int modifier = 1;
  
  Write(sheet["Variables"]["Traits"].AsString());
  if (setActivated == false)
    modifier = -1;
  sheet["Traits"][trait]       = (setActivated ? 1 : 0);

  if (trait == "Bloody Mess")
  {
    derivedStatistics["Damage Resistance"]  = derivedStatistics["Damage Resistance"].AsInt() + (5 * modifier);
  }
  else if (trait == "Finesse")
  {
    derivedStatistics["Bonus Damage"]    = derivedStatistics["Bonus Damage"].AsInt() - (25 * modifier);
    derivedStatistics["Critical Chance"] = derivedStatistics["Critical Chance"].AsInt() + (10 * modifier);
  }
  else if (trait == "Heavy Hoofed")
  {
    derivedStatistics["Melee Damage"]    = derivedStatistics["Melee Damage"].AsInt() + (4 * modifier);
    derivedStatistics["Critical Chance"] = derivedStatistics["Critical Chance"].AsInt() - (20 * modifier);
  }
  else if (trait == "Kamikaze")
  {
    derivedStatistics["Bonus Damage"]  = derivedStatistics["Bonus Damage"].AsInt() + (25 * modifier);
    derivedStatistics["Armor Class"]   = derivedStatistics["Armor Class"].AsInt() - (10 * modifier);
  }
  else if (trait == "Bruiser")
  {
    sheet["Special"]["STR"]            = sheet["Special"]["STR"].AsInt() + (2 * modifier);
    derivedStatistics["Action Points"] = derivedStatistics["Action Points"].AsInt() - (2 * modifier);
  }
  else if (trait == "Gifted")
  {
    sheet["Special"]["STR"]         = sheet["Special"]["STR"].AsInt() + (1 * modifier);
    sheet["Special"]["PER"]         = sheet["Special"]["PER"].AsInt() + (1 * modifier);
    sheet["Special"]["END"]         = sheet["Special"]["END"].AsInt() + (1 * modifier);
    sheet["Special"]["CHA"]         = sheet["Special"]["CHA"].AsInt() + (1 * modifier);
    sheet["Special"]["INT"]         = sheet["Special"]["INT"].AsInt() + (1 * modifier);
    sheet["Special"]["AGI"]         = sheet["Special"]["AGI"].AsInt() + (1 * modifier);
    sheet["Special"]["LUC"]         = sheet["Special"]["LUC"].AsInt() + (1 * modifier);
    derivedStatistics["Skill Rate"] = derivedStatistics["Skill Rate"].AsInt() - (6 * modifier);
  }
  else if (trait == "Small Frame")
  {
    sheet["Special"]["AGI"]           = sheet["Special"]["AGI"].AsInt() + (1 * modifier);
    derivedStatistics["Carry Weight"] = derivedStatistics["Carry Weight"].AsInt() - (25 * modifier);	
  }
  else if (trait == "Skilled")
  {
    derivedStatistics["Skill Rate"]      = derivedStatistics["Skill Rate"].AsInt() + (5 * modifier);
    derivedStatistics["Perk Rate"]       = derivedStatistics["Perk Rate"].AsInt() + (1 * modifier);
  }

  if (setActivated)
    traitsLeft -= 1;
  else
    traitsLeft += 1;
  sheet["Variables"]["Traits"] = traitsLeft;
  return (true);
}

bool ActivateTraits(Data sheet, string trait, bool setActivated)
{
  Data dataTrait  = sheet["Traits"][trait];
  bool isActive   = !(dataTrait.Nil()) && dataTrait.AsInt() == 1;
  int  traitsLeft = sheet["Variables"]["Traits"].AsInt();

  dataTrait = setActivated ? 1 : 0;
  if (isActive != setActivated)
  {
    if ((setActivated && traitsLeft > 0) || (!setActivated && isActive))
      return (DoActivateTrait(sheet, trait, setActivated, traitsLeft));
  }
  sheet["Traits"][trait] = (isActive ? 1 : 0);
  return (false);
}

bool AddSpecialPoint(Data sheet, string stat, int value)
{
  int new_value = sheet["Special"][stat].AsInt() + value;

  if ((new_value > 0 && new_value < 11) && ((sheet["Variables"]["Special Points"].AsInt() - value) >= 0))
  {
    sheet["Special"][stat]               = new_value;
    sheet["Variables"]["Special Points"] = sheet["Variables"]["Special Points"].AsInt() - value;
    UpdateAllValues(sheet);
    return (true);
  }
  return (false);
}

bool AddExperience(Data sheet, int value)
{
  int px        = sheet["Variables"]["Experience"].AsInt() + value;
  int nxt_level = XpNextLevel(sheet);

  sheet["Variables"]["Experience"] = px;
  return (px >= nxt_level);
}

int  XpNextLevel(Data sheet)
{
  int level = sheet["Variables"]["Level"].AsInt() + 1;

  return ((level * (level - 1) / 2) * 1000);
}

void LevelUp(Data sheet)
{
  int skillRate = sheet["Statistics"]["Skill Rate"].AsInt();
  int perkRate  = sheet["Statistics"]["Perk Rate"].AsInt();
  int lastPerk  = sheet["Variables"]["Last Perk"].AsInt() + 1;
  int endurance = sheet["Special"]["END"].AsInt();

  if (lastPerk >= perkRate)
  {
    lastPerk = 0;
    sheet["Variables"]["Perks"] = sheet["Variables"]["Perks"].AsInt() + 1;
  }
  sheet["Variables"]["Skill Points"]          = sheet["Variables"]["Skill Points"].AsInt() + skillRate;
  sheet["Variables"]["Level"]                 = sheet["Variables"]["Level"].AsInt()        + 1;
  sheet["Statistics Modifiers"]["Hit Points"] = sheet["Statistics Modifiers"]["Hit Points"].AsInt() + (endurance / 3 > 0 ? 1 : endurance / 3);
}

bool IsReady(Data sheet)
{
  bool ready = true;

  if (sheet["Variables"]["Special Points"].AsInt() != 0)
    ready = false;
  if (sheet["Variables"]["Traits"].AsInt() != 0)
    ready = false;
  return (ready);
}

void AddToStatModifier(Data sheet, string statistic, int value)
{
  Data statModifier = sheet["Statistics Modifier"][statistic];
  
  statModifier = statModifier.AsInt() + value;
}

// This must contains all the modifications to derived statistics and skills from SPECIAL and Traits changes
void UpdateAllValues(Data sheet)
{
  int level        = sheet["Variables"]["Level"].Nil() ? 1 : sheet["Variables"]["Level"].AsInt();
  int strength     = sheet["Special"]["STR"].AsInt();
  int perception   = sheet["Special"]["PER"].AsInt();
  int endurance    = sheet["Special"]["END"].AsInt();
  int charisma     = sheet["Special"]["CHA"].AsInt();
  int intelligence = sheet["Special"]["INT"].AsInt();
  int agility      = sheet["Special"]["AGI"].AsInt();
  int luck         = sheet["Special"]["LUC"].AsInt();

  Data derivedStatistics = sheet["Statistics"];
  Data skills            = sheet["Skills"];
  Data traits            = sheet["Traits"];
  Data skillPoints       = sheet["Skill Points"];
  Data statsModifiers    = sheet["Statistics Modifiers"];

  derivedStatistics["Action Points"]        = statsModifiers["Action Points"].AsInt()        + (agility < 5 ? 5 : agility);
  derivedStatistics["Armor Class"]          = statsModifiers["Armor Class"].AsInt()          + agility;
  derivedStatistics["Bonus Damage"]         = statsModifiers["Bonus Damage"].AsInt()         + 0;
  derivedStatistics["Carry Weight"]         = statsModifiers["Carry Weight"].AsInt()         + 25 + strength * 25;
  derivedStatistics["Critical Chance"]      = statsModifiers["Critical Chance"].AsInt()      + luck;
  derivedStatistics["Damage Resistance"]    = statsModifiers["Damage Resistance"].AsInt()    + 0;
  derivedStatistics["Healing Rate"]         = statsModifiers["Healing Rate"].AsInt()         + (endurance / 3 > 1 ? endurance / 3 : 1);
  derivedStatistics["Hit Points"]           = statsModifiers["Hit Points"].AsInt()           + 15 + strength + 2 * endurance + level * 3;
  derivedStatistics["Melee Damage"]         = statsModifiers["Melee Damage"].AsInt()         + (strength - 5  > 1 ? strength - 5 : 1);
  derivedStatistics["Perk Rate"]            = statsModifiers["Perk Rate"].AsInt()            + 3;
  derivedStatistics["Poison Resistance"]    = statsModifiers["Poison Resistance"].AsInt()    + endurance * 5;
  derivedStatistics["Radiation Resistance"] = statsModifiers["Radiation Resistance"].AsInt() + (endurance - 1) * 2;
  derivedStatistics["Skill Rate"]           = statsModifiers["Skill Rate"].AsInt()           + 5 + intelligence * 2;
  
  skills["Small Guns"]    = skillPoints["Small Guns"].AsInt()    + 5            + 4 * agility;
  skills["Big Guns"]      = skillPoints["Big Guns"].AsInt()      + strength     + 2 * agility;
  skills["Energy Guns"]   = skillPoints["Energy Guns"].AsInt()   + intelligence + 2 * agility;
  skills["Explosives"]    = skillPoints["Explosives"].AsInt()    + 10 + perception + agility;
  skills["Unarmed"]       = skillPoints["Unarmed"].AsInt()       + 30 + (2 * (agility + strength));
  skills["Lockpick"]      = skillPoints["Lockpick"].AsInt()      + 20 + (5 * perception) + (5 * agility);
  skills["Melee Weapons"] = skillPoints["Melee Weapons"].AsInt() + 20 + (2 * (agility + strength));
  skills["Medicine"]      = skillPoints["Medicine"].AsInt()      + 15 + perception + intelligence;
  skills["Repair"]        = skillPoints["Repair"].AsInt()        + perception + (3 * intelligence);
  skills["Science"]       = skillPoints["Science"].AsInt()       + 4 * intelligence;
  skills["Sneak"]         = skillPoints["Sneak"].AsInt()         + 5 + 3 * agility;
  skills["Spellcasting"]  = skillPoints["Spellcasting"].AsInt()  + 25 + (2 * intelligence) + luck;
  skills["Steal"]         = skillPoints["Steal"].AsInt()         + 3 * agility;
  skills["Barter"]        = skillPoints["Barter"].AsInt()        + 4 * charisma;
  skills["Outdoorspony"]  = skillPoints["Outdoorspony"].AsInt()  + 5 + (2 * endurance) + (2 * intelligence);
  skills["Speech"]        = skillPoints["Speech"].AsInt()        + 5 * charisma + intelligence;
  skills["Gambling"]      = skillPoints["Gambling"].AsInt()      + charisma + 4 * luck;
}

StringList AvailableTraits(Data sheet)
{
  StringList toRet;

  toRet.Add("Bloody Mess");
  toRet.Add("Bruiser");
  toRet.Add("Finesse");
  toRet.Add("Gifted");
  toRet.Add("Heavy Hoofed");
  toRet.Add("Kamikaze");
  toRet.Add("Skilled");
  toRet.Add("Small Frame");
  return (toRet);
}

string SelectRandomEncounter(Data sheet)
{
  Data data_engine        = worldmap.GetDataEngine();
  Data special_encounters = data_engine["special-encounters"];
  int  it                 = 0;

  special_encounters.Output();
  while (it < special_encounters.Count())
  {
    special_encounters[it].Output();
    return (special_encounters[it].AsString());
  }
  return ("");
}

int action_barter_value(Item@ item, Data stats_owner, Data stats_buyer)
{
  return (5);
}

int action_barter_deal(Data stats_first, int total_first, Data stats_second, int total_second)
{
  // TODO: Make this shit use the stats
  if (total_second >= total_first)
    return (0);
  return (1);
}

