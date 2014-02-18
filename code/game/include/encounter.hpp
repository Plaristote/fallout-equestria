#ifndef  ENCOUNTER_HPP
# define ENCOUNTER_HPP

# include "globals.hpp"
# include "cmap/statcontroller.hpp"
# include "worldmap/worldmap.hpp"

class Encounter
{
public:
  struct NoAvailableEncounters : public std::exception
  {
  };

  Encounter(StatController* player_statistics);

  void               SetCoordinates(int x, int y);
  void               SetAsEvent(bool is_event) { this->is_event = is_event; }
  void               Initialize(DataEngine&, WorldMap*);

  bool               IsDetectedByParty(void) const { return (is_detected);    }
  bool               IsEvent(void) const           { return (is_event);       }
  bool               IsSpecial(void) const         { return (is_special);     }
  const std::string& GetMapName(void) const        { return (map_name);       }
  const std::string& GetEncounterName(void) const  { return (encounter_name); }

private:
  void               InitializeSpecialEncounter(DataEngine&, WorldMap*);
  void               InitializeEncounter(DataEngine&, WorldMap*);

  StatController*    player_statistics;
  int                pos_x, pos_y;
  bool               is_event, is_detected, is_special;
  std::string        map_name;
  std::string        encounter_name;
};

#endif
