#include "level/diplomacy.hpp"
#include <algorithm>

using namespace std;

WorldDiplomacy::WorldDiplomacy(DataEngine& de) : _data_engine(de)
{
}

WorldDiplomacy::~WorldDiplomacy()
{
}

void WorldDiplomacy::Initialize(void)
{
  Data factions = _data_engine["Factions"];

  factions.Output();
  _next_flag = 1;
  _factions.clear();
  for_each(factions.begin(), factions.end(), [this](Data faction)
  { AddFaction(faction.Key()); });
  for_each(factions.begin(), factions.end(), [this](Data faction)
  {
    Data enemies = faction["Enemies"];

	// MSVC DOES NOT SUPPORT LAMBDAS PROPERLY
	auto _this = this;
    for_each(enemies.begin(), enemies.end(), [_this, faction](Data array)
    {
      _this->SetAsEnemy(array.Value() == "1", array.Key(), faction.Key());

	  // MSVC DOES NOT SUPPORT LAMBDAS PROPERLY
	  auto __this   = _this;
	  Data _faction = faction;
      for_each(array.begin(), array.end(), [__this, _faction](Data enemy)
      {
        __this->SetAsEnemy(enemy.Value() == "1", enemy.Key(), _faction.Key()); cout << _faction.Key() << " enemies with " << enemy.Value() << endl;
      });
    });
  });
}

void WorldDiplomacy::AddFaction(const string& name)
{
  Faction faction;

  faction.flag      = _next_flag;
  faction.name      = name;
  faction.enemyMask = 0;
  _next_flag       *= 2;
  _factions.push_back(faction);
}

WorldDiplomacy::Faction* WorldDiplomacy::GetFaction(const string& name)
{
  auto it = find(_factions.begin(), _factions.end(), name);

  if (it != _factions.end())
    return (&(*it));
  return (0);
}

WorldDiplomacy::Faction* WorldDiplomacy::GetFaction(unsigned int flag)
{
  auto it = find(_factions.begin(), _factions.end(), flag);

  if (it != _factions.end())
    return (&(*it));
  return (0);
}

void WorldDiplomacy::SetAsEnemy(bool set, const string& name1, const string& name2)
{
  auto it_first  = find(_factions.begin(), _factions.end(), name1);
  auto it_second = find(_factions.begin(), _factions.end(), name2);

  if (it_first != _factions.end() && it_second != _factions.end())
  {  SetAsEnemy(set, *it_first, *it_second); }
}

void WorldDiplomacy::SetAsEnemy(bool set, unsigned int flag_1, unsigned int flag_2)
{
  auto it_first  = find(_factions.begin(), _factions.end(), flag_1);
  auto it_second = find(_factions.begin(), _factions.end(), flag_2);

  if (it_first != _factions.end() && it_second != _factions.end())
  {  SetAsEnemy(set, *it_first, *it_second); }
}

void WorldDiplomacy::SetAsEnemy(bool set, Faction& first, Faction& second)
{
  Data factions = _data_engine["Factions"];

  if (set)
  {
    first.enemyMask  |= second.flag;
    second.enemyMask |= first.flag;
  }
  else
  {
    if (first.enemyMask & second.flag)
      first.enemyMask -= second.flag;
    if (second.enemyMask & first.flag)
      second.enemyMask -= first.flag;
  }
  factions[first.name]["Enemies"][second.name] = (set ? 1 : 0);
  factions[second.name]["Enemies"][first.name] = (set ? 1 : 0);
}

