# Introduction #

Our implementation of the SPECIAL system, a.k.a the Cutie Mark Aquisition Program, is a big deal for the game engine. We had to do something flexible, allowing to tweaks any statistic, implement new perks, traits with very much ease.

N.B.: We didn't give the possibility to have a statistic system that doesn't use the Fallout-like SPECIAL system (you can't use anything else than: Traits / SPECIAL / Statistics / Skills / Perks. You can however implement a lot of new behaviours using these, or part of these).

The Cutie Mark Acquisition Program (CMAP for short) has been designed so that on the long term, it can also be used on other engine. The only dependency is the scripting engine AngelScript, and the event management system from our engine (which can easily be used anywhere else).

The CMAP uses:
  * JSON for the character sheets design,
  * AngelScript for almost all data management,
  * Rocket for displaying character sheets in-game.

This means that you can tweak all the statistic mechanics directly from the special.as script (such as how much the strenght of a character should affect the melee damage he deals or his carry weight capacity).

It also means that you can change the content of each statistic category (you can add traits, special attributes, statistics, skills and perks as you wish: they will be properly displayed in the view and you will be able to handle the mechanics from scripts).

And it also means that the view is designed using RML/RCSS.
So you can change it to your needs as well without a sweat.

# Details #

We'll detail the structure of the charsheet model and how to modify it.
Then we'll go over the many hooks the the script file can implement.

### TODO: do what the two last lines says ###

## General Information ##
The statistic sheet starts with the primary informations:
```
"Name": "Character's name",
"Age": 16,
"Gender": "Male/Female",
"Race": "Changeling",
"Faction": "A faction name"
```

The faction system will be explained later in this document. It is used to determine who attacks you on sight and who doesn't.

## SPECIAL Points ##
The primary statistics are the SPECIAL values that we all came to know quite well: in order, Strength, Perception, Endurance, Charisma, Intelligence, Agility, Luck.
The statistic sheet depict them like this:
```
"Special": {
  "STR": 5,
  "PER": 5,
  "END": 5,
  "CHA": 5,
  "INT": 5,
  "AGI": 5,
  "LUC": 5
}
```

SPECIAL will have an impact on the Statistics and the Skills.
The SPECIAL points don't change as the player gains levels, but they can be influenced by modifiers.

## Skills ##
Skills are competences that improve over time. The skills are arbitrary and are left to the choice of the game developer.
Some of them though are directly required by the engine:

  * Outdoorspony, which is used in the worldmap to raise the chances of detecting encounters before they detect you.
  * Sneak, which cause enemy characters to make a test before detecting an enemy.

The data is stored in two parts: the first contains the current level for each skill, and the second stores the amount of skill points that the player spent on specific skills. It looks like this:

```
"Skills": {
  "Small Guns": 0,
  "Big Guns": 0,
  "Energy Guns": 15,
  "etc...": 0
},
"Skill Points": {
  "Small Guns": 0,
  "Big Guns": 0,
  "Energy Guns": 15,
  "etc...": 0
}
```

The skills can also be affected by Affinities. A character with an affinity to a skill sees all the skills changes multiplied by 2 without changing the cost in skill points.

Affinities are also stored in the stat sheet:

```
"Affinities": {
  "Small Guns": 1,
  "Unarmed": 1
}
```

## Statistics ##
The statistics are a set of values with a practical application in the game:

```
"Statistics": {
  "Action Points": 5,
  "Armor Class": 5,
  "Bonus Damage": 0
  "Carry Weight": 100,
  "Critical Chance": 7,
  "Damage Resistance": 0,
  "Healing Rate": 1,
  "Hit Points": 26,
  "Melee Damage": 1,
  "Perk Rate": 3,
  "Poison Resistance": 15,
  "Radiation Resistance": 6,
  "Skill Rate": 19
}
```

These fields are directly from the original Fallout. Only some of these are actually mandatory:

  * Action Points
  * Armor Class: define the chances of a character to dodge an attack
  * Carry Weight
  * Healing Rate: how many hit points are healed each day.
  * Hit Points
  * Perk Rate: how much level must pass for a new perk to become available
  * Skill Rate: how much skill points are gained each level

Just like for the skills, there's a second store called "Statistics Modifiers" which needs to be used when a perk, item or whatever, changes the value of a statistic.
It stores the amount of all the cumulated changes in a statistic, so that the CMAP can recompute correctly the statistic when needed.

## Traits ##
Traits are specific traits of a character picked at the creation of a statistic sheet. They can never change over time.
The usual philosophy behind a trait is that they are always both a curse and a blessing. For instance, if it makes you stronger, it also makes you slower.

The default amount of traits available is two. Though it is possible to have more (or less) as long as it is done during the statistic sheet creation step.

## Perks ##
Perks are gained sometimes when a characters level up. The amount of levels that need to pass before a perk is gained is defined in `Statistics/Perk Rate`.

The perks themselves are configured in the file `data/perks.json`. The file describe for each perk a set of requirements for the character to be able to pick the perk.

Here's a sample of `perks.json` file:

```
{
  "Ninja": {
    "Requirements": {
      "Special/AGI": {
        "Comp":  ">=",
        "Value": 3
      },
     "Skills/Unarmed": {
     },
    "Description": "You're a freaking ninja."
    }
  },
  "Wizardry": {
    "Requirements": {
      "Special/INT": {
        "Comp":  ">",
        "Value": 8
      },
      "Variables/Level": {
        "Comp": ">=",
        "Value": 3
      }
    },
    "Description": "You are a wizard."
  },
  "One Legged": {
    "Description": "Instead of picking of perk you cut three of your legs off."
  }
}
```

The perks can have a set of requirements: these are used to check a value from the player's statistic sheet.
Each block is named after the value it checks, and contains two variables:

  * `Comp` which defines the nature of the operation (==, !=, >=, <=, >, <)
  * `Value` which define the second operand of the operation

For instance, the first condition to get the Ninja perk is: `Special/AGI >= 3`.

When a perk is picked, the function "AddPerk" from the `special.as` script is called so that you can make changes to the statistics of the character:

```
bool AddPerk(Data statistics, string perk_name)
{
  int perk_points = statistics["Variables"]["Perks"].AsInt();

  // Check if the player still has perk points to spend
  if (perks_points > 0)
  {
    // Decrease the amount of perk points
    statistics["Variables"]["Perks"] = perks_points - 1;
    if (perk_name == "One Legged")
    {
      // Here goes the code to cut off the characters limbs.
    }
    return (true);
  }
  return (false);
}
```

## Faction ##
Factions define which character are hostile to which other characters. Each character is able to check another character and define whether its intention should be hostile, neutral or friendly.

The "friendly" attitude is used in fights to cooperate and in the interface to show the player which people he'd rather not shoot at.

Factions can be added and removed as the game goes on.

**Actually this is not yet possible. TODO**


## The special.as script ##
The special.as script, for some reason currently located in the script/ai directory, is used to react to events from the statistic interface sheet, or to other changes of the character (such as gaining a level or some experience).

The special.as script is also used to select which enemy are encountered when an encounter happens on the world map.