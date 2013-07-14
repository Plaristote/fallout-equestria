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

void Level::SetEntryZone(ObjectCharacter* character, const std::string& name)
{
  EntryZone* zone = _world->GetEntryZoneByName(name);
  
  if (zone)
  {
    list<Waypoint*>::iterator it  = zone->waypoints.begin();
    list<Waypoint*>::iterator end = zone->waypoints.end();

    for (; it != end ; ++it)
    {
      if (MoveCharacterTo(character, *it))
        break ;
    }
    if (character == GetPlayer())
      _camera.CenterCameraInstant(GetPlayer()->GetNodePath().get_pos());
  }
}

bool Level::MoveCharacterTo(ObjectCharacter* character, unsigned int wp_id)
{
  Waypoint* wp = _world->GetWaypointFromId(wp_id);

  if (wp)
    return (MoveCharacterTo(character, wp));
  return (0);
}

bool Level::MoveCharacterTo(ObjectCharacter* character, Waypoint* wp)
{
  if (!(IsWaypointOccupied(wp->id)))
  {
    DynamicObject& object = *(character->GetDynamicObject());

    _world->DynamicObjectSetWaypoint(object, *wp);
    object.floor = -1;
    object.nodePath.set_alpha_scale(1.f);
    object.nodePath.show();
    character->SetOccupiedWaypoint(wp);
    _world->DynamicObjectChangeFloor(object, wp->floor);
    character->GetNodePath().set_pos(wp->nodePath.get_pos());
    character->TruncatePath(0);
    return (true);
  }
  return (false);
}

void Level::SetEntryZone(Party& party, const std::string& name)
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
    auto party_it  = party.GetObjects().begin();
    auto party_end = party.GetObjects().end();

    for (; party_it != party_end ; ++party_it)
    {
      cout << "[Level][SetEntryZone] Trying to insert character " << (*party_it)->nodePath.get_name() << endl;
      list<Waypoint*>::iterator it        = zone->waypoints.begin();
      list<Waypoint*>::iterator end       = zone->waypoints.end();
      ObjectCharacter*          character = GetCharacter(*party_it);

      for (; it != end ; ++it)
      {
        if (MoveCharacterTo(character, *it))
        {
          // Forcing the character to fade in
          LColor color = character->GetNodePath().get_color();

          color.set_w(0);
          character->GetNodePath().set_color(color);
          character->SetVisible(true);
          break ;
        }
      }
    }
  }
  _exitingZone = false;
  _camera.CenterCameraInstant(GetPlayer()->GetNodePath().get_pos());
  _floor_lastWp = 0;
}