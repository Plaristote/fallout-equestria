#include "encounter.hpp"
#include "dices.hpp"

using namespace std;

Encounter::Encounter(StatController* player_statistics) : player_statistics(player_statistics)
{
  short luck        = player_statistics->Model().GetSpecial("LUC");
  short outdoorsman = player_statistics->Model().GetSkill("Outdoorspony");

  is_event    = false;
  pos_x       = pos_y = 0;
  is_detected = Dices::Throw(200) <= outdoorsman;
  is_special  = Dices::Throw(150) <= luck;
}

void Encounter::SetCoordinates(int x, int y)
{
  pos_x = x;
  pos_y = y;
}

void Encounter::Initialize(DataEngine& data_engine, WorldMap* worldmap)
{
  if (IsSpecial())
    InitializeSpecialEncounter(data_engine, worldmap);
  else
    InitializeEncounter(data_engine, worldmap);
}

void Encounter::InitializeEncounter(DataEngine &, WorldMap *worldmap)
{
  Data  case_data  = worldmap->GetCaseData(pos_x, pos_y);
  Data  encounters = case_data["type-encounters"];
  Data  maps       = case_data["map-encounters"];

  if (maps.Count() == 0 || (is_event && encounters.Count() == 0))
    throw NoAvailableEncounters();
  map_name         = maps[Dices::Throw(maps.Count()) - 1].Value();
  if (is_event)
    encounter_name = encounters[Dices::Throw(encounters.Count()) - 1].Value();
}

void Encounter::InitializeSpecialEncounter(DataEngine& data_engine, WorldMap* worldmap)
{
  //map_name = player_statistics->Model().SelectRandomEncounter();
  if (map_name == "")
  {
      Data special_encounters = data_engine["special-encounters"];

      if (special_encounters.Count() > 0)
        map_name = special_encounters[0].Value();
  }
  if (map_name == "" || worldmap->HasCity(map_name))
    throw NoAvailableEncounters();
}
