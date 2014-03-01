#include "encounter_spawn.hpp"
#include <sstream>

using namespace std;

EncounterSpawn::EncounterSpawn(const string& type): Party(type, type)
{
  SetName(type);
}

void EncounterSpawn::LoadFromDataEngine(DataEngine& data_engine)
{
  Data spawn_data = data_engine["random-encounters"][GetName()];
  
  for (unsigned int i = 0 ; i < spawn_data.Count() ; ++i)
  {
    Data         character = spawn_data[i];
    stringstream unique_name;

    unique_name << GetName() << "-" << i;
    character["name"] = unique_name.str();
    Join(character);
  }
}
