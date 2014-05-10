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
  
  spawn_data.Output();
  for (unsigned int i = 0 ; i < spawn_data.Count() ; ++i)
  {
    std::string  name      = spawn_data[i].Key();
    unsigned int clones    = 0;
    unsigned int quantity  = spawn_data[i].Or(0);

    for (; clones < quantity ; ++clones)
    {
      stringstream unique_name;

      unique_name << GetName() << '-' << i << '-' << clones;
      Join(name, unique_name.str());
    }
  }
}
