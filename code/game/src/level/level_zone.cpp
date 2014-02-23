#include "level/level_zone.hpp"
#include "level/level.hpp"

using namespace std;

LevelZone::LevelZone(Level* level, Zone& zone) : _level(level), _zone(zone)
{
  _enabled = true;
  SetName(zone.name);
  ForEach(zone.waypoints, [this](Waypoint* waypoint)
  {
    LevelZone* self = this;

    ForEach(waypoint->arcs, [self](Waypoint::Arc& arc)
    {
      arc.observer = self;
    });
  });
}

LevelZone::~LevelZone()
{
}

void LevelZone::GoingThrough(void* object)
{
  auto it = std::find(_objects.begin(), _objects.end(), object);
  
  if (it == _objects.end())
  {
    InstanceDynamicObject* ptr = reinterpret_cast<InstanceDynamicObject*>(object);

    _objects.push_back(ptr);
    if (_enabled)
      Entered.Emit(ptr);
  }
}

void LevelZone::Refresh(void)
{
  auto it  = _objects.begin();
  auto end = _objects.end();
  
  while (it != end)
  {
    InstanceDynamicObject* object = *it;
    auto                   result = std::find(_zone.waypoints.begin(),
                                              _zone.waypoints.end(),
                                              object->GetOccupiedWaypoint());

    if (result == _zone.waypoints.end())
    {
      it = _objects.erase(it);
      if (_enabled)
        Exited.Emit(object);
    }
    else
      ++it;
  }
  _effects.clear();
}

#include "executor.hpp"
void LevelZone::AddEffect(Effect effect, unsigned short time)
{
  TimeManager&       time_manager = _level->GetTimeManager();
  TimeManager::Task* task         = time_manager.AddTask(1, DateTime::Seconds(time));

  task->Interval.Connect(effect);
}

bool LevelZone::IsInside(InstanceDynamicObject* object) const
{
  auto it  = _objects.begin();
  auto end = _objects.end();
  
  while (it != end)
  {
    if (*it == object)
      return (true);
    ++it;
  }
  return (false);
}
