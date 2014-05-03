#include "level/floors.hpp"
#include "level/objects/instance_dynamic_object.hpp"
#include "level/level.hpp"

using namespace std;

unsigned char Floors::GetFloorFromObject(InstanceDynamicObject* object)
{
  Waypoint* wp = object->GetOccupiedWaypoint();

  if (wp && wp->floor != current_floor)
    return (wp->floor);
  return (0);
}

void Floors::SetCurrentFloorFromObject(InstanceDynamicObject* object)
{
  Waypoint* wp = object->GetOccupiedWaypoint();

  if (wp)
  {
    if (wp->id != last_waypoint)
      SetCurrentFloor(wp->floor);
    last_waypoint = wp->id;
  }
}

void Floors::ShowOnlyFloor(unsigned char floor)
{
  World&        world       = *level.GetWorld();
  unsigned char floor_above = floor + 1;

  for (unsigned int it = 0 ; it < floor ; ++it)
    FadeFloor(world.floors[it], false);
  for (unsigned int it = floor_above ; it < world.floors.size() ; ++it)
    FadeFloor(world.floors[it], false);
  FadeFloor(world.floors[floor], true);
}

void Floors::SetCurrentFloor(unsigned char floor)
{
  World&        world            = *level.GetWorld();
  unsigned char floorAbove       = floor + 1;
  bool          isInsideBuilding = IsInsideBuilding(floorAbove);

  if (floorAbove < floor)
    ShowOnlyFloor(floor);
  else
  {
    for (unsigned int it = 0 ; it < floor ; ++it)
      FadeFloor(world.floors[it], show_lower_floors);
    for (unsigned int it = floor ; it < floorAbove && it < world.floors.size() ; ++it)
      FadeFloor(world.floors[it], true);
    for (unsigned int it = floorAbove ; it < world.floors.size() ; ++it)
      FadeFloor(world.floors[it], !isInsideBuilding);
  }
  current_floor = floor;
}

void Floors::FadeFloor(NodePath floor, bool fade_in)
{
  list<HidingFloor>::iterator it = find(hiding_floors.begin(), hiding_floors.end(), floor);
  HidingFloor                 hiding_floor;

  if (floor.is_hidden() && !fade_in)
    return ;
  if (it == hiding_floors.end() && !floor.is_hidden() && fade_in)
    return ;
  hiding_floor.SetNodePath(floor);
  hiding_floor.SetFadingIn(fade_in);
  if (it != hiding_floors.end())
  {
    hiding_floor.ForceAlpha(it->Alpha());
    hiding_floors.erase(it);
  }
  hiding_floors.push_back(hiding_floor);
}

void Floors::RunFadingEffect(float elapsed_time)
{
  std::list<HidingFloor>::iterator cur, end;

  for (cur = hiding_floors.begin(), end = hiding_floors.end() ; cur != end ;)
  {
    cur->Run(elapsed_time);
    if (cur->Done())
      cur = hiding_floors.erase(cur);
    else
      ++cur;
  }
}

void Floors::HidingFloor::SetNodePath(NodePath np)
{
  floor = np;
  floor.set_transparency(TransparencyAttrib::M_alpha);
}

void Floors::HidingFloor::SetFadingIn(bool set)
{
  fadingIn = !set;
  alpha    = fadingIn ? 1.f : 0.f;
  if (!fadingIn)
    floor.show();
}

void Floors::HidingFloor::Run(float elapsedTime)
{
  alpha += (fadingIn ? -0.1f : 0.1f) * (elapsedTime * 10);
  done   = (fadingIn ? alpha <= 0.f : alpha >= 1.f);
  floor.set_alpha_scale(alpha);
  if (fadingIn && done)
    floor.hide();
}

bool Floors::IsInsideBuilding(unsigned char& floor)
{
  Waypoint* waypoint = level.GetPlayer()->GetOccupiedWaypoint();

  if (waypoint->floor_above != waypoint->floor)
  {
    floor = waypoint->floor_above;
    return (true);
  }
  return (false);
}
