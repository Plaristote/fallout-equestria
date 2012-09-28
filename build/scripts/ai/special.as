//
// CHARACTER CREATION STUFF
//

// This must contain every modification to the SPECIAL attributes when a trait is activated/disactivated
bool ActivateTraits(Data sheet, string trait, bool setActivated)
{
  Data derivedStatistics = sheet["Statistics Modifiers"];
  Data dataTrait  = sheet["Traits"][trait];
  bool isActive   = !(dataTrait.Nil()) && dataTrait.AsInt() == 1;
  int  traitsLeft = sheet["Variables"]["Traits"].AsInt();

  dataTrait = setActivated ? 1 : 0;
  if ((isActive) != setActivated)
  {
    traitsLeft = sheet["Variables"]["Traits"].AsInt();
    if (setActivated && traitsLeft > 0 || !setActivated)
    {
      int modifier = 1;
     
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
        string tmp1 = "";
        
        tmp1 = derivedStatistics["Critical Chance"].AsString();
      
	derivedStatistics["Melee Damage"]    = derivedStatistics["Melee Damage"].AsInt() + (4 * modifier);
	derivedStatistics["Critical Chance"] = derivedStatistics["Critical Chance"].AsInt() - (20 * modifier);
	
	tmp1 += " now is ";
	tmp1 += derivedStatistics["Critical Chance"].AsString();
	Cout(tmp1);
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
  }
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
  return (true);
}

void AddToStatModifier(Data sheet, string statistic, int value)
{
  Data statModifier = sheet["Statistics Modifier"][statistic];
  
  statModifier = statModifier.AsInt() + value;
}

// This must contains all the modifications to derived statistics and skills from SPECIAL and Traits changes
void UpdateAllValues(Data sheet)
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
  Data skillPoints       = sheet["Skill Points"];
  Data statsModifiers    = sheet["Statistics Modifiers"];

  derivedStatistics["Action Points"]        = statsModifiers["Action Points"].AsInt()        + (agility < 5 ? 5 : agility);
  derivedStatistics["Armor Class"]          = statsModifiers["Armor Class"].AsInt()          + agility;
  derivedStatistics["Bonus Damage"]         = statsModifiers["Bonus Damage"].AsInt()         + 0;
  derivedStatistics["Carry Weight"]         = statsModifiers["Carry Weight"].AsInt()         + 25 + strength * 25;
  derivedStatistics["Critical Chance"]      = statsModifiers["Critical Chance"].AsInt()      + luck;
  derivedStatistics["Damage Resistance"]    = statsModifiers["Damage Resistance"].AsInt()    + 0;
  derivedStatistics["Healing Rate"]         = statsModifiers["Healing Rate"].AsInt()         + (endurance / 3 > 1 ? endurance / 3 : 1);
  derivedStatistics["Hit Points"]           = statsModifiers["Hit Points"].AsInt()           + 15 + strength + 2 * endurance;
  derivedStatistics["Melee Damage"]         = statsModifiers["Melee Damage"].AsInt()         + (strength - 5  > 1 ? strength - 5 : 1);
  derivedStatistics["Perk Rate"]            = statsModifiers["Perk Rate"].AsInt()            + 3;
  derivedStatistics["Poison Resistance"]    = statsModifiers["Poison Resistance"].AsInt()    + endurance * 5;
  derivedStatistics["Radiation Resistance"] = statsModifiers["Radiation Resistance"].AsInt() + (endurance - 1) * 2;
  derivedStatistics["Skill Rate"]           = statsModifiers["Skill Rate"].AsInt()           + 5 + intelligence * 2;
  
  skills["Small Guns"]    = skillPoints["Small Guns"].AsInt()    + 35 + agility;
  skills["Big Guns"]      = skillPoints["Big Guns"].AsInt()      + 10 + agility;
  skills["Energy Guns"]   = skillPoints["Energy Guns"].AsInt()   + 10 + agility;
  skills["Explosives"]    = skillPoints["Explosives"].AsInt()    + 20 + (5 * perception) + (5 * agility);
  skills["Unarmed"]       = skillPoints["Unarmed"].AsInt()       + 40 + (5 * agility) + (5 * strength);
  skills["Lockpick"]      = skillPoints["Lockpick"].AsInt()      + 20 + (5 * perception) + (5 * agility);
  skills["Melee Weapons"] = skillPoints["Melee Weapons"].AsInt() + 55 + (5 * agility) + (5 * strength);
  skills["Medicine"]      = skillPoints["Medicine"].AsInt()      + 15 + (5 * perception) + (5 * intelligence);
  skills["Repair"]        = skillPoints["Repair"].AsInt()        + 20 + intelligence;
  skills["Science"]       = skillPoints["Science"].AsInt()       + 25 + 2 * intelligence;
  skills["Sneak"]         = skillPoints["Sneak"].AsInt()         + 25 + agility;
  skills["Spellcasting"]  = skillPoints["Spellcasting"].AsInt()  + 25 + (2 * intelligence) + luck;
  skills["Steal"]         = skillPoints["Steal"].AsInt()         + 0 + agility;
  skills["Barter"]        = skillPoints["Barter"].AsInt()        + 20 + 2 * charisma;
  skills["Outdoorspony"]  = skillPoints["Outdoorspony"].AsInt()  + 5 + (5 * endurance) + (5 * intelligence);
  skills["Speech"]        = skillPoints["Speech"].AsInt()        + 25 + 2 * charisma;
  skills["Gambling"]      = skillPoints["Gambling"].AsInt()      + 20 + 3 * luck;
  
//   if (!(traits["Bloody Mess"].Nil()))
//   {
//     derivedStatistics["Damage Resistance"]  = derivedStatistics["Damage Resistance"].AsInt() + 5;
//   }
//   if (!(traits["Bruiser"].Nil()))
//   {
//     derivedStatistics["Action Points"]   = derivedStatistics["Action Points"].AsInt() - 2;
//   }
//   if (!(traits["Finesse"].Nil()))
//   {
//     derivedStatistics["Bonus Damage"]    = derivedStatistics["Bonus Damage"].AsInt() - 25;
//     derivedStatistics["Critical Chance"] = derivedStatistics["Critical Chance"].AsInt() + 10;
//   }
//   if (!(traits["Gifted"].Nil()))
//   {
//     derivedStatistics["Skill Rate"]      = derivedStatistics["Skill Rate"].AsInt() - 6;
//   }
//   if (!(traits["Heavy Hoofed"].Nil()))
//   {
//     derivedStatistics["Melee Damage"]    = derivedStatistics["Melee Damage"].AsInt() + 4;
//     derivedStatistics["Critical Chance"] = derivedStatistics["Critical Chance"].AsInt() - 20;
//   }
//   if (!(traits["Kamikaze"].Nil()))
//   {
//     derivedStatistics["Bonus Damage"]    = derivedStatistics["Bonus Damage"].AsInt() + 25;
//     derivedStatistics["Armor Class"]     = derivedStatistics["Armor Class"].AsInt() - 10;
//   }
//   if (!(traits["Skilled"].Nil()))
//   {
//     derivedStatistics["Skill Rate"]      = derivedStatistics["Skill Rate"].AsInt() + 5;
//     derivedStatistics["Perk Rate"]       = derivedStatistics["Perk Rate"].AsInt() + 1;
//   }
//   if (!(traits["Small Frame"].Nil()))
//   {
//     derivedStatistics["Carry Weight"]   = derivedStatistics["Carry Weight"].AsInt() - 25;
//   }
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