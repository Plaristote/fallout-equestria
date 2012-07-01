//
// CHARACTER CREATION STUFF
//

// This must contain every modification to the SPECIAL attributes when a trait is activated/disactivated
bool ActivateTraits(Data sheet, string trait, bool setActivated)
{
  if      (trait == "Bruiser")
    sheet["Special"]["STR"] = sheet["Special"]["STR"].AsInt() + (setActivated ? 2 : -2);
  else if (trait == "Gifted")
  {
    sheet["Special"]["STR"] = sheet["Special"]["STR"].AsInt() + (setActivated ? 1 : -1);
    sheet["Special"]["PER"] = sheet["Special"]["PER"].AsInt() + (setActivated ? 1 : -1);
    sheet["Special"]["END"] = sheet["Special"]["END"].AsInt() + (setActivated ? 1 : -1);
    sheet["Special"]["CHA"] = sheet["Special"]["CHA"].AsInt() + (setActivated ? 1 : -1);
    sheet["Special"]["INT"] = sheet["Special"]["INT"].AsInt() + (setActivated ? 1 : -1);
    sheet["Special"]["AGI"] = sheet["Special"]["AGI"].AsInt() + (setActivated ? 1 : -1);
    sheet["Special"]["LUC"] = sheet["Special"]["LUC"].AsInt() + (setActivated ? 1 : -1);
  }
  else if (trait == "Small Frame")
    sheet["Special"]["AGI"] = sheet["Special"]["AGI"].AsInt() + (setActivated ? 1 : -1);
}

// This must contains all the modifications to derived statistics and skills from SPECIAL and Traits changes
void SetInitialValues(Data sheet)
{
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

  derivedStatistics["Action Points"]        = (agility < 5 ? 5 ? agility);
  derivedStatistics["Armor Class"]          = agility;
  derivedStatistics["Bonus Damage"]         = 0;
  derivedStatistics["Carry Weight"]         = 25 + strength * 25;
  derivedStatistics["Critical Chance"]      = luck;
  derivedStatistics["Damage Resistance"]    = 0;
  derivedStatistics["Healing Rate"]         = (endurance / 3 > 1 ? endurance / 3 : 1);
  derivedStatistics["Hit Points"]           = 15 + strength + 2 * endurance;
  derivedStatistics["Melee Damage"]         = (strength - 5  > 1 ? strength - 5 : 1);
  derivedStatistics["Perk Rate"]            = 3;
  derivedStatistics["Poison Resistance"]    = endurance * 5;
  derivedStatistics["Radiation Resistance"] = (endurance - 1) * 2;
  derivedStatistics["Skill Rate"]           = 5 + intelligence * 2;

  skills["Small Guns"]    = 35 + agility;
  skills["Big Guns"]      = 10 + agility;
  skills["Energy Guns"]   = 10 + agility;
  skills["Explosives"]    = 20 + (5 * perception) + (5 * agility);
  skills["Unarmed"]       = 40 + (5 * agility) + (5 * strength);
  skills["Lockpick"]      = 20 + (5 * perception) + (5 * agility);
  skills["Melee Weapons"] = 55 + (5 * agility) + (5 * strength);
  skills["Medicine"]      = 15 + (5 * perception) + (5 * intelligence);
  skills["Repair"]        = 20 + intelligence;
  skills["Science"]       = 25 + 2 * intelligence;
  skills["Sneak"]         = 25 + agility;
  skills["Spellcasting"]  = 25 + (2 * intelligence) + luck;
  skills["Steal"]         = 20 + agility;
  skills["Barter"]        = 20 + 2 * charisma;
  skills["Outdoorspony"]  = 5 + (5 * endurance) + (5 * intelligence);
  skills["Speech"]        = 25 + 2 * charisma;
  skills["Gambling"]      = 20 + 3 * luck;

  if (!(traits["Bloody Mess"].Nil()))
  {
    derivedStatistics["Damage Resistance"]  = derivedStatistics["Damage Resistance"].AsInt() + 5;
  }
  if (!(traits["Bruiser"].Nil()))
  {
    derivedStatistics["Action Points"]   = derivedStatistics["Action Points"].AsInt() - 2;
  }
  if (!(traits["Finesse"].Nil()))
  {
    derivedStatistics["Bonus Damage"]    = derivedStatistics["Bonus Damage"].AsInt() - 25;
    derivedStatistics["Critical Chance"] = derivedStatistics["Critical Chance"].AsInt() + 10;
  }
  if (!(traits["Gifted"].Nil()))
  {
    derivedStatistics["Skill Rate"]      = derivedStatistics["Skill Rate"].AsInt() - 6;
  }
  if (!(traits["Heavy Hoofed"].Nil()))
  {
    derivedStatistics["Melee Damage"]    = derivedStatistics["Melee Damage"].AsInt() + 4;
    derivedStatistics["Critical Chance"] = derivedStatistics["Critical Chance"].AsInt() - 20;
  }
  if (!(traits["Kamikaze"].Nil()))
  {
    derivedStatistics["Bonus Damage"]    = derivedStatistics["Bonus Damage"].AsInt() + 25;
    derivedStatistics["Armor Class"]     = derivedStatistics["Armor Class"].AsInt() - agility;
  }
  if (!(traits["Skilled"].Nil()))
  {
    derivedStatistics["Skill Rate"]      = derivedStatistics["Skill Rate"].AsInt() + 5;
    derivedStatistics["Perk Rate"]       = derivedStatistics["Perk Rate"].AsInt() + 1;
  }
  if (!(traits["Small Frame"].Nil()))
  {
    derivedStatistics["Carry Weight"]   = derivedStatistics["Carry Weight"].AsInt() - 25;
  }
}
