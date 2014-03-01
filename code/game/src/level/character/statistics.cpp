#include "level/characters/statistics.hpp"
#include "options.hpp"

using namespace std;

CharacterStatistics::CharacterStatistics(Level* level, DynamicObject* object): User(level, object), controller(0), data_tree(0)
{
  character_sheet_name = object->charsheet;
  cout << "Trying to load " << object->charsheet << endl;
  if (!(TryToLoadSavedCharacterSheet() || TryToLoadCharacterSheet()))
    GenerateCharacterSheet();
}

CharacterStatistics::~CharacterStatistics(void)
{
  Cleanup();
}

bool CharacterStatistics::TryToLoadSavedCharacterSheet(void)
{
  string savepath = OptionsManager::Get()["savepath"].Value();

  return (TryToLoadFromFile(savepath + "/" + character_sheet_name + ".json"));
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
    controller = new StatController(data_tree);
    return (true);
  }
  return (false);
}

bool CharacterStatistics::TryToLoadFromString(const string& json)
{
  data_tree = DataTree::Factory::JSON(json);
  if (data_tree)
  {
    controller = new StatController(data_tree);
    return (true);
  }
  return (false);
}

void CharacterStatistics::GenerateCharacterSheet(void)
{
  data_tree  = new DataTree;
  controller = new StatController(data_tree);
}

void CharacterStatistics::Cleanup(void)
{
  if (data_tree != 0)
  {
    delete controller;
    delete data_tree;
    data_tree  = 0;
    controller = 0;
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

unsigned short CharacterStatistics::GetMaxActionPoints(void) const
{
  return (controller->GetData()["Statistics"]["Action Points"]);
}

unsigned short CharacterStatistics::GetActionPoints(void) const
{
  return (controller->GetData()["Variables"]["Action Points"]);
}

short CharacterStatistics::GetMaxHitPoints(void) const
{
  return (controller->GetData()["Statistics"]["Hit Points"]);
}

short CharacterStatistics::GetHitPoints(void) const
{
  return (controller->GetData()["Variables"]["Hit Points"]);
}

void CharacterStatistics::SetActionPoints(unsigned short value)
{
  controller->SetActionPoints(value);
}

void CharacterStatistics::SetHitPoints(short value)
{
  controller->SetCurrentHp(value);
}

void CharacterStatistics::Serialize(Utils::Packet& packet)
{
  Data        statistics = controller->GetData();
  std::string json;

  DataTree::Writers::StringJSON(statistics, json);
  packet << character_sheet_name << json;
  Pathfinding::User::Serialize(packet);
}

void CharacterStatistics::Unserialize(Utils::Packet& packet)
{
  std::string json;

  packet >> character_sheet_name >> json;
  Pathfinding::User::Unserialize(packet);
  {
    Cleanup();
    if (!(TryToLoadFromString(json)))
      GenerateCharacterSheet();
    RefreshStatistics();
  }
}
