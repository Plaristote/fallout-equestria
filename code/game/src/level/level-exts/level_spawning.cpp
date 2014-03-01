#include "level/level.hpp"

using namespace std;

void Level::SpawnEnemies(const std::string& type, unsigned short spawn_zone_number)
{
  cout << "[Level] spawning " << type << endl;
  Party          spawn_party;
  stringstream   entry_zone;
  Data           random_party = (*_dataEngine)["random-encounters"][type];

  entry_zone << "spawn_" << spawn_zone_number;
  for (unsigned short ii = 0 ; ii < random_party.Count() ; ++ii)
  {
    Data           critter       = random_party[ii];
    string         critter_type  = critter.Key();
    unsigned short critter_count = critter;

    for (unsigned short i = 0 ; i < critter_count ; ++i)
    {
      Data           data         = (*_dataEngine)["bestiary"][critter_type];
      DynamicObject* object;
      string         str_model    = data["model"].Value();
      string         str_texture  = data["texture"].Value();

      object = _world->AddDynamicObject(type, DynamicObject::Character, str_model, str_texture);
      if (object)
      {
        object->strModel  = str_model;
        object->strTexture= str_texture;
        object->charsheet = data["charsheet"].Value();
        object->dialog    = data["dialog"].Value();
        object->script    = data["script"].Value();
        InsertCharacter(new ObjectCharacter(this, object));
        spawn_party.Join(*_characters.rbegin());
      }
    }
  }
  zones.InsertPartyInZone(spawn_party, entry_zone.str());
}
