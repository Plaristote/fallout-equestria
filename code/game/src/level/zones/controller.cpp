#include "level/zones/controller.hpp"
#include "level/zones/manager.hpp"
#include <level/zones/exception.hpp>
#include "level/level.hpp"
#include <ui/ui_next_zone.hpp>

using namespace std;

Zones::Controller::Controller(Zone& zone) : manager(0), zone(zone), enabled(true), can_move_through(true)
{
}

void Zones::Controller::ObjectMovesWithinZone(InstanceDynamicObject* object)
{
  if (enabled)
  {
    if (IsExitZone())
      ExitingZone(object);
    else
      RegisterResident(object);
  }
}

void Zones::Controller::RegisterResident(InstanceDynamicObject* object)
{
  auto it = find(residents.begin(), residents.end(), object);

  if (it == residents.end())
    InsertResident(object);
  else
    RefreshResident(*it);
}

void Zones::Controller::GoFromHereTo(const string& destination)
{
  Level::Exit& exit_data = manager->level.GetExit();

  exit_data.level = destination;
  exit_data.zone  = zone.name; // Current zone name is used as entry zone name for the destination
}

void Zones::Controller::ExitingZone(InstanceDynamicObject* object)
{
  if (starts_with(zone.name, "LocalExit"))
    LocalExit(object);
  else if (manager->level.GetPlayer() == object)
    LevelExit(object);
}

void Zones::Controller::LocalExit(InstanceDynamicObject* object)
{
  ObjectCharacter* character = object->Get<ObjectCharacter>();

  if (zone.destinations.size() == 1)
    manager->InsertObjectInZone(object, zone.destinations.front());
  else if (character->IsPlayer())
  {
    UiNextZone* ui = manager->level.GetLevelUi().OpenZonePicker(zone.destinations);

    ui->Done.Connect            (manager->level.GetLevelUi(), &LevelUi::CloseRunningUi<LevelUi::UiItNextZone>);
    ui->NextZoneSelected.Connect([this, object](const string& destination)
    {
      manager->InsertObjectInZone(object, destination);
    });
  }
}

void Zones::Controller::LevelExit(InstanceDynamicObject* object)
{
  if (zone.destinations.size() == 1)
    GoFromHereTo(zone.destinations.front());
  else
  {
    UiNextZone* ui = manager->level.GetLevelUi().OpenZonePicker(zone.destinations);

    ui->Done.Connect            (manager->level.GetLevelUi(), &LevelUi::CloseRunningUi<LevelUi::UiItNextZone>);
    ui->NextZoneSelected.Connect(*this,                       &Zones::Controller::GoFromHereTo);
  }
}

void Zones::Controller::InsertObject(InstanceDynamicObject* object)
{
  auto it      = zone.waypoints_ids.begin();
  auto end     = zone.waypoints_ids.end();
  bool success = false;

  for (; it != end ; ++it)
  {
    Waypoint* waypoint = manager->level.GetWorld()->GetWaypointFromId(*it);

    if (!(manager->level.IsWaypointOccupied((*it))))
    {
      InsertObjectOnWaypoint(object, waypoint);
      success  = true;
      break ;
    }
  }
  if (success)
    InsertResident(object);
  else
    throw ZoneIsFull(zone.name, "Failed to insert " + object->GetName() + " into zone.");
}

void Zones::Controller::InsertObjectOnWaypoint(InstanceDynamicObject* object, Waypoint* waypoint)
{
  ObjectCharacter* character      = object->Get<ObjectCharacter>();
  NodePath         nodepath       = object->GetNodePath();
  DynamicObject&   dynamic_object = *object->GetDynamicObject();
  World*           world          = manager->level.GetWorld();

  world->DynamicObjectSetWaypoint(dynamic_object, *waypoint);
  object->SetOccupiedWaypoint(waypoint);
  object->GetNodePath().set_pos(waypoint->nodePath.get_pos());
  if (character)
  {
    character->TruncatePath(0);
    if (character == manager->level.GetPlayer())
      manager->level.GetCamera().CenterCameraInstant(nodepath.get_pos());
  }
}

void Zones::Controller::InsertResident(Resident resident)
{
  residents.push_back(resident);
  EnteredZone.Emit(resident.object);
}

void Zones::Controller::RefreshResident(Resident& resident)
{
  Waypoint* current_waypoint = resident.object->GetOccupiedWaypoint();
  
  if (current_waypoint != resident.waypoint)
  {
    resident.waypoint = current_waypoint;
    MovedWithinZone.Emit(resident.object);
  }
}

void Zones::Controller::Refresh(void)
{
  auto it  = residents.begin();
  auto end = residents.end();
  
  while (it != end)
  {
    Resident& resident = *it;

    if (resident.HasMoved())
    {
      if (IsInZone(resident.object))
        resident.waypoint = resident.object->GetOccupiedWaypoint();
      else
      {
        ExitedZone.Emit(resident.object);
        it = residents.erase(it);
        continue ;
      }
    }
    ++it;
  }
}

void Zones::Controller::SetEnabled(bool enabled)
{
  if (enabled == false && this->enabled == true)
    DisableZone();
  this->enabled = enabled;
}

void Zones::Controller::DisableZone(void)
{
  for_each(residents.begin(), residents.end(), [this](Resident& resident)
  {
    ExitedZone.Emit(resident.object);
  });
  residents.clear();
}

void Zones::Controller::SetZoneBlocked(bool blocked)
{
  can_move_through = !blocked;
}

bool Zones::Controller::CanGoThrough(InstanceDynamicObject* object)
{
  if (enabled == true)
    return (can_move_through);
  return (true);
}

bool Zones::Controller::IsInZone(Waypoint* waypoint) const
{
  auto      match    = find(zone.waypoints_ids.begin(), zone.waypoints_ids.end(), waypoint->id);

  return (match != zone.waypoints_ids.end());
}

bool Zones::Controller::IsInZone(InstanceDynamicObject* object) const
{
  return (IsInZone(object->GetOccupiedWaypoint()));
}
