#ifndef  ENCOUNTER_SPAWN_HPP
# define ENCOUNTER_SPAWN_HPP

#include "party.hpp"
#include "dataengine.hpp"

class EncounterSpawn : public Party
{
public:
  EncounterSpawn(const std::string& type);
  
  void LoadFromDataEngine(DataEngine& data_engine);
};

#endif