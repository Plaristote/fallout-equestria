#include "level/characters/statistics.hpp"
#include <level/level.hpp>
#include "options.hpp"

using namespace std;

CharacterStatistics::CharacterStatistics(Level* level, DynamicObject* object): User(level, object), controller(0), metabolism(0), data_tree(0)
{
  character_sheet_name = object->charsheet;
  cout << "Trying to load " << object->charsheet << endl;
  if (character_sheet_name == "" || !(TryToLoadCharacterSheet()))
    GenerateCharacterSheet();
  if (controller)
  {
    metabolism = new Metabolism(controller);
    CharacterFirstLoading();
  }
}

CharacterStatistics::~CharacterStatistics(void)
{
  Cleanup();
}

void CharacterStatistics::CharacterFirstLoading(void)
{
  Data behaviour = controller->GetData()["Behaviour"];
  Data inventory = controller->GetData()["Inventory"];

  if (behaviour.NotNil() && _object->interactions == 0)
  {
    _object->script       = behaviour["script"].Value();
    _object->dialog       = behaviour["dialog"].Value();
    _object->interactions = (int)behaviour["interactions"].Or(54);
    SetInteractionsFromDynamicObject(_object);
  }
  else
    _object->interactions = Interactions::LookAt;
  behaviour.Remove();
  inventory.Remove();
}

bool CharacterStatistics::TryToLoadCharacterSheet(void)
{
  return (TryToLoadFromFile("data/charsheets/" + character_sheet_name + ".json"));
}

bool CharacterStatistics::TryToLoadFromFile(const string& path)
{
  data_tree = DataTree::Factory::JSON(path);
  if (data_tree)
  {
    cout << "Loading stat sheet from file for character " << GetDynamicObject()->name << endl;
    controller = new StatController(data_tree);
    return (true);
  }
  return (false);
}

bool CharacterStatistics::TryToLoadFromString(const string& json)
{
  data_tree = DataTree::Factory::StringJSON(json);
  if (data_tree)
  {
    cout << "Loading statsheet from string for character " << GetDynamicObject()->name << endl;
    controller = new StatController(data_tree);
    return (true);
  }
  return (false);
}

void CharacterStatistics::GenerateCharacterSheet(void)
{
  cout << "Generating stat sheet for character " << GetDynamicObject()->name << endl;
  data_tree  = new DataTree;
  controller = new StatController(data_tree);
  {
    Data default_data(data_tree);

    default_data["Name"]                        = GetDynamicObject()->name;
    default_data["Special"]["STR"]              = 5;
    default_data["Special"]["PER"]              = 5;
    default_data["Special"]["END"]              = 5;
    default_data["Special"]["CHA"]              = 5;
    default_data["Special"]["INT"]              = 5;
    default_data["Special"]["AGI"]              = 5;
    default_data["Special"]["LUC"]              = 5;
    default_data["Statistics"]["Hit Points"]    = 15;
    default_data["Statistics"]["Action Points"] = 5;
  }
}

void CharacterStatistics::Cleanup(void)
{
  if (data_tree != 0)
  {
    observers.DisconnectAll();
    if (metabolism) delete metabolism;
    delete controller;
    delete data_tree;
    data_tree  = 0;
    controller = 0;
    metabolism = 0;
  }
}

void CharacterStatistics::ForceStatController(StatController* controller)
{
  if (this->controller != controller)
    Cleanup();
  data_tree                  = 0;
  this->controller           = controller;
  RefreshStatistics();
}

void  CharacterStatistics::RefreshStatistics()
{
  if (controller)
    controller->Died.DisconnectAll();
  if (controller)
  {
    observers.Connect(controller->Died, [this]()
    {
      GetLevel()->CharacterDied.Emit((ObjectCharacter*)this);
    });
  }
}

short CharacterStatistics::GetMaxHitPoints(void) const
{
  return (controller->GetData()["Statistics"]["Hit Points"].Or(0));
}

short CharacterStatistics::GetHitPoints(void) const
{
  return (controller->GetData()["Variables"]["Hit Points"].Or(GetMaxHitPoints()));
}

void CharacterStatistics::SetHitPoints(short value)
{
  controller->SetCurrentHp(value);
}

void CharacterStatistics::SetMetabolism(Metabolism* _metabolism)
{
  if (metabolism)
    delete metabolism;
  metabolism = _metabolism;
}

void CharacterStatistics::Serialize(Utils::Packet& packet)
{
  Data        statistics = controller->GetData();
  std::string json;
  char        has_metabolism = (metabolism != 0 ? 1 : 0);

  DataTree::Writers::StringJSON(statistics, json);
  packet << character_sheet_name << json << has_metabolism;
  if (has_metabolism)
    metabolism->Serialize(packet);
  Pathfinding::User::Serialize(packet);
}

void CharacterStatistics::Unserialize(Utils::Packet& packet)
{
  std::string json;
  char        has_metabolism;

  packet >> character_sheet_name >> json >> has_metabolism;
  {
    Cleanup();
    if (!(TryToLoadFromString(json)))
      GenerateCharacterSheet();
    RefreshStatistics();
  }
  if (has_metabolism)
  {
    metabolism = new Metabolism(controller);
    metabolism->Unserialize(packet);
  }
  Pathfinding::User::Unserialize(packet);
}
