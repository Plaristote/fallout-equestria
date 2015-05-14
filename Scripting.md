# Introduction #

This here documentation will introduce you to scripting with our game engine.

## Firt things first ##
### How do I AngelScript ? ###
First thing to know is that our scripting language is AngelScript: it has a syntax similar to C, with garbage collecting features and links directly to the engine's native C++ API.

Here's a sample of what it looks like
```
int absolute(int parameter)
{
  int result;

  if (parameter < 0)
    result = -parameter;
  else
    return = parameter;
  return (parameter);
}
```

You can learn how to write code in angelscript reading this documentation:

http://www.angelcode.com/angelscript/sdk/docs/manual/doc_script.html

The scripting langage is however fairly easy to apprehand, moreover if you're already familiar with other scripting languages.
It's object oriented. Our script engine is based on hooks only, so you don't have to write your objects, but you'll deal with ours.

Unlike many script language, the variable are typed, and there also is a notion of references (the @ character you'll see in some variable declarations).
I will explain how to use the references whenever it is relevant. I believe everything else about the language is quite self-explanatory.

### Glossary ###
A few expressions used in this document might need clarification. Here goes:

**realtime / in-game time**: realtime is simply the time that actually elapses, while in-game time points to the amount of time that elapses for the character in the game. Usually, they're both one and the same, but in-game time might be accelerated/decelerated or stopped in some circumstances (for instance, during a fight, each turn lasts for 10 seconds regardless of how much realtime elapsed).

### What parts of the game use script ? ###
Quite a few of them. Every scripts of each category is regrouped in a directory in the 'scripts' folder.

Here's a list of things that can be scripted:

- **Articial Intelligences**: they run when your a character is prompted to do something, allows you to implement behaviors into NPCs.

- **Buffs**: they're used to implement effects on a character that settles over time. They can be linked with a visual effect and are exectued every once in a while until the effect wears out.

- **Dialogs**: when writing dialogs, you have the liberty of setting hooks to the character's speeches. They can determine if the player can use an answer, or what happens if that answer is given.

- **Objects**: the objects can have hooks define for when they're used in specific ways. This is, for example, where weapons effect are scripted.

- **Quests**: whenever a condition or objective is completed in a quest, an 'update' hook is called so that the script (if there is one) can modify the quest and pick different endings for it.

- **Pipbuck**: it is possible to write your own pipbuck application. The pipbuck itself has an API that allow application to display an interface or to interact with the game whenever they need to.

- **Statistics** (CMAP): the Cutie Mark Acquisition Program lets you set hooks to react to common events (gained xp, level up) and to determine the rules of the statistic system.

- **Tasks**: Tasks are a way to ask for an AngelScript function to be executed only after a given amount of in-game time.

- **Zones**: Zones are created with the Level Editor. When initialized properly, these objects can be used to detect when a character enters or leaves a zone. They can also be used to trigger effects executed regularly (pink cloud damages over time for instance).
Note that zones **cannot** overlap with each other.

- **Lights**: it is possible to control the lights directly from scripts to change their color, range, or to turn them on and off.

- **Levels**: optionally, each level can use a script to execute code when loading and unloading the level. These scripts can be use for instance to initialize zone detections, or to execute 'global' code more often than a `Task` would.

# Scripting Dialogs #
## The data engine ##
We'll first start by learning how to script dialog. We'll use it to introduce ourselves to the data engine's API.

```
string HookInit()
{
  return ("SpeechKey");
}
```

Every dialog script starts with the HookInit function. The value it returned will be used to select the opening line of the dialog (pronounced by the NPC).
We'll see how we can use that to make a character introduce himself only when he hasn't been met before:

```
string HookInit()
{
  Data data_engine = level.GetDataEngine();
  Data have_talked = data_engine["dialogs"]["Sterling"]["happened"];

  if (have_talked.Nil())
  {
    have_talked = 1;
    return ("FristGreetings");
  }
  return ("RegularGreetings");
}
```

What are we seeing here ?
In the first line, you're introduced to the level global variable. It sums up everything that happens outsite of the world map. We use it here to get an instance of the DataEngine.

The DataEngine stores the game variables. There are many information that the game engine make available to you through it (character inventories, factions, pipbuck data and so on).
You can also use it to store persistent variable (persistent meaning they will be saved with the game).

The DataEngine uses the Data type. It's a dynamic wrapper for a string based data tree: you can dynamically create or access branches using the index operator.
We use this firstly to create our own variable 'have\_talked'. The condition then checks if this variable has already been set before: if not, we set it to '1' and return "FirstGreetings", which will be our character introducing himself to the player. If not, we will simply return "RegularGreetings", our character simply saying something of the like of "Hi".

## Visibility hooks ##
The dialog tool from the Game Editor allows you to add hooks to answers.
The simpler of those hooks is the visibility hook: it is used to select which of the current context's answers the player can pick.

A visibility hook looks like this:

```
bool CanPlayerSayThis()
{
  Data data_enine = level.GetDataEngine();
  Data knows_about_something = data_engine["stuff"]["knows_about_something"];

  if (knows_about_something.Nil())
    return (false); // The player can't say this cause he doesn't know about something
  return (true); // The player knows about something and can say this
}
```

Again we use the same features from the data engine to figure out if the player can or not pick one of our answers. The value can then be set from elsewhere when some NPC's make a revelation to the player's.

## Answers hooks ##
The second type of hook that can be set to an answer is executed when the player picks it: it will override the "GoTo" context configured in the dialog editor: the return value of your hook will be used instead, allowing you to chose what the NPC will answer to the player's pick.

In this exemple, we'll introduce the Character and Statistics object to make a skill check allowing a special answer to the player if he has spent enough skill points on speech:

```
string AnswerToQuestion()
{
  Character@ player      = level.GetPlayer();
  Data       statistics  = player.GetStatistics();

  if (statistics["Skills"]["Speech"].AsInt() > 75)
  {
    Data     data_engine = level.GetDataEngine();

    data_engine["stuff"]["knows about something"] = 1;
    return ("SpecialAnswer");
  }
  return ("RegularAnswer");
}
```

Here, we get a reference to the player's character object to get a handle on his statistics.
There is several ways to interact with the statistic system, but when no logic is involved, you can simply access its datastore. You can check out a character sheet json file to get a look at the setup.

In the condition, we access the Speech skill variable and convert it as an Int with the AsInt method. AsString and AsFloat are also available.
If the skill is superior to 75, we will return the special answer, but not before informing the data engine that the player 'knows about something' now.

## Inline scripts ##
It is possible to embed scripted content in any piece of text that a character says. It can be used, for instance, to display the character's name: the NPC's line will look like this `Howdy #{player_name} ! Mighty fine day we're having right ?`

The `#{player_name}` will be replaced by the return value of a function called `player_name` in your script. In our case, the function would look like this:

```
string player_name()
{
  return (level.GetPlayer().GetName());
}
```

Note that if you wish to use the `#{` characters in a dialog text, you can avoid the script replacement by escaping the sharp character like this `\#{`.

# Scripting Quests #
## The Quest System ##
It is possible to write quest without using scripts. The script however gives them the ability to evolve in non-linear ways.

The first thing to know is how quest are interpreted by the game engine.
Your quest can basially be summed up as a list of objectives that are completed only when all their conditions are set to true.
Considering your hook will be called whenever a condition state changes, you can set up branches in your condition, such as this:

```
Data MyQuestDescription()
{
  Data my_quest   = level.GetDataEngine()["MyQuests"]["DummyQuest"];
  Data objective  = my_quest["objectives"]["MyObjective"]
  Data condition1 = objective["conditions"]["MyCondition1"];
  Data condition2 = objective["conditions"]["MyCondition2"];

  condition1["type"]   = "CharacterKilled";
  condition1["target"] = "Sterling";

  condition2["type"]            = "InventoryContains";
  condition2["target"]          = "Sterling";
  condition2["object_type"]     = "Key";
  condition2["object_quantity"] = 2;
  return (my_quest);
}
```

Here we create and store a description for a quest named "DummyQuest".
This piece of code already helps us determine how quest, objectives and conditions are structured.

Here, the objective will be fulfilled when the following conditions are completed: "Sterling must be dead", and "Sterling's inventory must contain 2 'Keys'".

Onto the conditions: here we make two conditions and we set their "type" variaable to different values. The types indicate the game engine to watch a particular kind of event on a specific character.

### CharacterKilled ###
CharacterKilled is a condition that is set to true when a character is dead (it doesn't matter who specifically killed him).
The parameters it takes are very simple:
- target: the name of the character to check.

### InventoryContains ###
InventoryContains is a condition checked everytime the inventory's content of the target changes.
It takes a little bit more parameters:
- target: the name of the character whose inventory will be checked.
- object\_type: the name of the object we're looking for.
- object\_quantity: the quantity of the object that needs to be reached for the condition to complete.

### Custom conditions ###
It is also possible to set up custom conditions that the game engine will ignore: they can however be completed from script (we'll see how later) and generate all the events that a regular condition fires (that is, when the objective is completed, your Quest hook (if there is one) will be executed).

## The Quest Manager ##
We just created a quest from scratch in our scripts. The next step is to chose when will we add this quest to the player's active quests.
To do this, we will use the QuestManager, made available by the 'game' global variable:

```
void AddQuestToPlayer(Data quest_data)
{
  QuestManager@ quest_manager = game.GetQuestManager();

  quest_manager.AddQuest(quest_data);
}
```

Now whenever you want to add our previous quest to the player's active quests, you would just write ` AddQuestToPlayer(MyQuestDescription()); `.

Calling the 'AddQuest' method will instantly make your quest available in the Quest application from the Pipbuck.

The Quest Manager can also be for modifying quests or force the completion of conditions. We'll see that later by writing our first Quest hook:

## The Hook ##
Quest only have a single hook. It's called whenever the quest state has changed.
The hook takes a single parameter which is the quest datastore (Data type again). We'll write a hook that allows us to set different development for a quest.

```
work in progress
```

If you changed the objective or condition while being inside a level, you need to re-initialize the quest so that it is warned when the objectives targets states change.

```
void Update(Data quest_data)
{
  QuestManager@ quest_manager = game.GetQuestManager();
  Quest@        quest         = quest_manager[quest_data.Key()];

  //
  // Make changes to the quest here
  //
  quest.Finalize();
  quest.Initialize(level);
}
```

Here we introduce a ton of new stuff.
The second global variable for starters, 'game'. It contains every part of the game that are present both during the world map and level states. We use it to get a handle of the QuestManager, and then use the QuestManager to get a handle on the current Quest object.

To initialize the quest again, we call the Finalize method to clean up the observers that already were on the level, and Initialize to reset them while taking account of the last modifications.

The Initialize method takes a Level as a parameter (the global variable level here). If you are not inside a level, it is impossible (and irrelevant) to re-initialize a Quest.

# Scripting Characters #
Character's are part of a category of objects called DynamicObjects. Everything on the map that can be interacted with is a DynamicObject. They also have the ability to interact with each other and with the player on their own (if a script command them to, that is).

Specific types of dynamic objects are doors, shelfs, items dropped on the ground, and as previously mentioned, characters (both NPCs and player).

You need to know this because while scripting characters, you will very often be confronted to various specializations of DynamicObjects. Later, when scripting objects, you might have to filter those specializations in order to implement different behaviors for an object when used on different type of targets (ex: some pipbucky hacking device might unlock a closed door or locker, but when used on a computer, it would display an interface to crack the password).

Back to characters.
The characters hooks have a few specificities: unlike all the others, each of them is called on a regular basis.

## The Loop Hooks ##
Two of your character's hook will be loop hooks: one of them will determine the usual behavior of the character, the other one will control him during fights.

Here are the prototypes for the two hooks:
```
  void main(Character@ self, float elapsedTime);
  void combat(Character@ self);
```

Note that the first hook has an elapsedTime parameter, which allows you to know how much real time has elapsed since the last time the hook was called.

Work in progress

# Scheduling #
The script engine provides you with feature to schedule events. These features allow you to ask for one of your functions to be called after a certain amount of in-game time has elapsed.

Here, we will learn the scheduling feature along with the "AddTextBox" method that makes text pop above an object.
We'll use it to create a dialog happening out of the boundaries of the dialog interfaces, only with text boxes: the scheduling features will help us making the sentences appear one after another.

```
// Let's assume this function is called when a certain event happens:
void some_function()
{
  // The ApiDefineFunction is used to tell the script engine about a function
  // from the current script file that you wish to expose to the rest of the game.
  ApiDefineFunction("TheDialog", "void TheDialog()");

  // The PushLevelTask will create the scheduled task.
  PushLevelTask("TheDialog", 3);
}
```

Pretty simple right ? As you can see, the first parameter of the PushLevelTask is the name of the function that needs to be called. The second parameter is the amount of in-game time, in seconds, that will elapse before said function is called.

Let's use this to create a dialog:

```
// This variable will be used to count how many times the TheDialog function has been called
int dialog_state = 0;

void TheDialog(void)
{
  // We get the characters and convert them in DynamicObjects.
  DynamicObject@ self   = level.GetCharacter("Sterling").AsObject();
  DynamicObject@ player = level.GetPlayer().AsObject();

  if (dialog_state == 0) // This is the first time the callback is called
  {
    self.AddTextBox("Howdy partner, talk to me if you wanna buy some stuff", 0, 255, 100, 3);
    // We will call the same function in 4 seconds so that the "player" can
    // give an answer to Sterling.
    PushLevelTask("TheDialog", 4);
  }
  else
  {
    player.AddTextBox("Sure thing", 255, 255, 255, 3);
  }
  dialog_state++;
}
```

A bit more about the AddTextBox method: the first parameter, obviously, contains the text that the character is going to say. The three followings parameter are the RGB values used to color the text: it's pretty standard to have the text written in red when the text is pronounced angrily, to have it green when it is said kindly, or white for neutral. The last parameter is the realtime in seconds that will elapses before the text fades out.

# Scripting Level #
The Level script is a little thing that allows you to execute code at the beginning and the end of a level. It also has a hook that is called once for every frame rendered.

Here are the prototype for the three hooks:
```
  void initialize(); // Called right after level loading
  void finalize();   // Called right before destroying the level
  void run(float elapsed_time); // Called once per frame rendered
```

The elapsed\_time value sent to the run function corresponds to the amount of realtime that elapsed since the last call to the run function.

In the following demonstration, we're going to use `Zones` to demonstrate both the use of these hooks and the usage of the `Zone` object.

# Scripting Zone #
Zones aren't scripted with files: any script can create a `Zone` object that will observe a specific zone and callback a function of your choice when someone enters or leaves the zone.

A good practice is to setup zones using the hooks from the `Level` script. The following example will display the name of every object that enters a zone if it is a character. Check this out:

```
Zone@ entry_zone; // The handle to our Zone object

// This is the initialize hook from the Level script.
void initialize()
{
  // The Zone_Factory function creates a Zone object that observes the zone
  // designated by the first parameter. Here, we are monitoring the zone called
  // "worldmap", which is the name of the default entry zone for people coming
  // from the worldmap.
  @entry_zone = Zone_Factory("worldmap", "void entered_entry_zone(DynamicObject@)");
}

// This is the callback we described when creating the Zone object. It will
// be called each time a character enters the zone.
void entered_entry_zone(DynamicObject@ object)
{
  // AsCharacter is a method of DynamicObject that returns an ObjectCharacter if
  // the current object is a character, or null otherwise.
  ObjectCharacter@ character = object.AsCharacter();

  if (@character != null)
  {
    // Cout() is used to write text on the standard output of the game.
    // You can also use Write() to write on the game console instead.
    Cout("Someone entered the entry zone: " + character.GetName());
  }
}

// This is the finalize hook from the Level script:
void finalize()
{
  entry_zone.Delete(); // Cleaning up after you is a always a good practice
  @entry_zone = null;
}

```

The `Zone` object has of course more feature than that. We're going to enumerate them here:

### How do I get warned when a character exists the zone ###
Ok, so we saw how to get a warning when a character enters a zone. What if you want to do the opposite ? There's a method for that:

```
  entry_zone.SetExitCallback("void exited_entry_zone(DyanmicObject@)");
```

Now the exited\_entry\_zone function will be called each time a character

### Toggle the zone on and off ###
Zones can be toggled on and off, which prevents your callbacks from being called.

```
  entry_zone.SetEnabled(true);  // Enables the zone
  entry_zone.SetEnabled(false); // Disables the zone
```

You can also check if the zone is enabled or not by using
```
  if (entry_zone.IsEnabled())
    Cout("Zone is enabled");
  else
    Cout("Zone is not enabled");
```

### Checking if a character is in the zone ###
Very simple, take this for instance:
```
  if (entry_zone.IsInside(level.GetPlayer())
  {
    Cout("Yes he is in the zone !");
  }
```

### Performing effects on a regular basis ###
You might also want an effect to be executed at a defined interval. Here's how to do it:

```
Zone@ entry_zone;

void my_effect(Zone@ zone)
{
  if (zone.IsInside(level.GetPlayer())
  {
    Cout("Player is inside zone during the effect");
  }
}

void initialize()
{
  @entry_zone = Zone_Factory("worldmap", "void entered_entry_zone(DynamicObject@)");
  entry_zone.SetEffect("my_effect", 5);
  // The method my_effect will be called every 5 seconds until the effect is disabled
}
```

You may also disable the effect by using the DisableEffect method:
```
  entry_zone.DisableEffect();
```

# Scripting a Party #
## What's a party ##
The party is a set of characters that can be inserted and removed at any moment in a level. They can be saved separately, which allows characters to be temporarily stripped away from a level, or even to move them from one level to another.

There are several ways to make characters join a party:

**Using an already existing character from a level and make him join the party.** Generate a new party member from a Data structure. The party member will become a character whenever the party gets inserted in a level.

## Creating/Importing Party ##
Parties are identified by a unique name, and can be required during the execution of a level using that name.

If no party using the name provided is found, then a new Party will be created.

## Exporting Party ##
If you wish to move your party from level to level, you need to export it.
Removing your party from the level will create a distinct file to save the party.

In the case of characters who appear in different levels under certain conditions, I recommend you use the level script to import and export the party respectively in the initialize and finalize hooks, so as to always have the party available for all the levels.