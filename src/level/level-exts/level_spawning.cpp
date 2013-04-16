#include "level/level.hpp"

using namespace std;

void Level::SpawnEnemies(const std::string& type, unsigned short quantity, unsigned short n_spawn)
{
  Party          spawn_party;
  stringstream   entry_zone;
  Data           random_party = (*_dataEngine)["random-encounters"][type];
  unsigned short i            = 0; 

  entry_zone << "spawn_" << n_spawn;
  for (unsigned short ii = 0 ; ii < random_party.Count() ; ++ii)
  {
    string         critter_type = random_party[ii].Key();
    unsigned short same_critter = 0;

    for (; same_critter < (unsigned short)random_party[ii] && i < quantity ; ++same_critter, ++i)
    {
      Data           data         = (*_dataEngine)["bestiary"][critter_type];
      DynamicObject* object;
      string         str_model    = data["model"].Value();
      string         str_texture  = data["texture"].Value();

      object = _world->AddDynamicObject(type, DynamicObject::Character, str_model, str_texture);
      object->strModel  = str_model;
      object->strTexture= str_texture;
      object->charsheet = data["charsheet"].Value();
      object->dialog    = data["dialog"].Value();
      object->script    = data["script"].Value();
      InsertCharacter(new ObjectCharacter(this, object));
      spawn_party.Join(object);

      ObjectCharacter* tmp = (*_characters.rbegin());
      tmp->PlayAnimation("Run");
      tmp->SetAsEnemy(GetPlayer(), true); // TODO remove that when enemies flag loading is fixed      
    }
  }
  SetEntryZone(spawn_party, entry_zone.str());
}

void Level::SetEntryZone(Party& player_party, const std::string& name)
{
  EntryZone* zone               = _world->GetEntryZoneByName(name);

  if (!zone && _world->entryZones.size() > 0)
    zone = &(_world->entryZones.front());
  else if (!zone)
  {
    cout << "[Map Error] This map has no entry zones. Generating a fake entry zone." << endl;  
    _world->AddEntryZone("FakeEntryZone");
    zone = _world->GetEntryZoneByName("FakeEntryZone");
    for (unsigned int i = 1 ; i < 10 ; ++i)
    {
      Waypoint* wp = _world->GetWaypointFromId(i);

      if (wp)
        zone->waypoints.push_back(wp);
      else
        break ;
    }
  }
  if (zone)
  {
    cout << "[Level][SetEntryZone] Inserting characters into entry zone '" << zone->name << "'" << endl;
    auto party_it  = player_party.GetObjects().begin();
    auto party_end = player_party.GetObjects().end();

    for (; party_it != party_end ; ++party_it)
    {
      cout << "[Level][SetEntryZone] Trying to insert character " << (*party_it)->nodePath.get_name() << endl;
      list<Waypoint*>::iterator it  = zone->waypoints.begin();
      list<Waypoint*>::iterator end = zone->waypoints.end();

      for (; it != end ; ++it)
      {
        if (!(IsWaypointOccupied((*it)->id)))
        {
          ObjectCharacter* character = GetCharacter(*party_it);

          if (character)
          {
            cout << "[Level][SetEntryZone][" << character->GetName() << " is now on waypoint " << (*it)->id << endl;
            (*party_it)->waypoint = *it;
            (*party_it)->floor    = -1;
            (*party_it)->nodePath.set_alpha_scale(1.f);
            (*party_it)->nodePath.show();
            character->SetOccupiedWaypoint(*it);
            _world->DynamicObjectChangeFloor(*character->GetDynamicObject(), (*it)->floor);
            character->GetNodePath().set_pos((*it)->nodePath.get_pos());
            character->TruncatePath(0);
          }
          break ;
        }
      }
    }
  }
  _exitingZone = false;
  _camera.CenterCameraInstant(GetPlayer()->GetNodePath().get_pos());
  //_camera.FollowObject(GetPlayer());
  _floor_lastWp = 0;
}