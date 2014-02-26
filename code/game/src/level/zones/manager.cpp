#include "level/zones/manager.hpp"
#include "level/level.hpp"
 
using namespace std;

Zones::Manager::Manager(Level& level) : level(level)
{
}

Zones::Manager::~Manager()
{
  UnregisterAllZones();
}

void Zones::Manager::Refresh(void)
{
  for_each(zones.begin(), zones.end(), [](Controller* entry)
  {
    entry->Refresh();
  });
}

void Zones::Manager::RegisterZone(Zone& zone)
{
  Controller* controller = new Controller(zone);

  zones.push_back(controller);  
  controller->SetManager(this);
  InitializeZoneObservers(*controller);
}

Zones::Observer* Zones::Manager::FindObserver(Waypoint* waypoint) const
{
  auto it  = observers.begin();
  auto end = observers.end();

  for (; it != end ; ++it)
  {
    if (**it == waypoint)
      return (*it);
  }
  return (0);
}

void Zones::Manager::InitializeZoneObservers(Controller& entry)
{
  auto     it     = entry.zone.waypoints.begin();
  auto     end    = entry.zone.waypoints.end();

  for (; it != end ; ++it)
  {
    Waypoint* waypoint = *it;
    Observer* observer = FindObserver(waypoint);

    if (!observer)
    {
      observer = new Observer(waypoint);
      
      observers.push_back(observer);
    }
    observer->zones.push_back(&entry);
  }
}

void Zones::Manager::UnregisterZone(const string& name)
{
  auto it = find_if(zones.begin(), zones.end(), [name](Controller* controller) -> bool { return (name == controller->zone.name); });

  if (it != zones.end())
  {
    Controller* entry = *it;
    auto        observers_it  = observers.begin();
    auto        observers_end = observers.end();
    
    while (observers_it != observers_end)
    {
      Observer& observer = **observers_it;

      std::remove(observer.zones.begin(), observer.zones.end(), entry);
      if (observer.zones.size() == 0)
      {
        delete &observer;
        observers_it = observers.erase(observers_it);
      }
      else
        observers_it++;
    }
    delete entry;
    zones.erase(it);
  }
}

void Zones::Manager::UnregisterAllZones(void)
{
  for_each(zones.begin(),     zones.end(),     [](Controller* controller) { delete controller; });
  for_each(observers.begin(), observers.end(), [](Observer* observer)     { delete observer;   });
  zones.clear();
  observers.clear();
}

void Zones::Manager::InsertPartyInZone(Party& party, const string& zone)
{
  try
  {
    Controller& controller = GetZone(zone);
    auto        iterator   = party.ConstGetObjects().begin();
    auto        end        = party.ConstGetObjects().end();

    for (; iterator != end ; ++iterator)
    {
      const DynamicObject* object    = *iterator;
      ObjectCharacter*     character = level.GetCharacter(object);

      controller.InsertObject(character);
    }
  }
  catch (...)
  {
    // TODO proper exception handling
  }
}

void Zones::Manager::InsertObjectInZone(InstanceDynamicObject* object, const string& zone)
{
  try
  {
    Controller& controller = GetZone(zone);

    controller.InsertObject(object);
  }
  catch (...)
  {
    // TODO proper exception handling
  }
}

bool Zones::Manager::ZoneExists(const string& name) const
{
  return (find_if(zones.begin(), zones.end(), [name](Controller* controller) -> bool { return (controller->zone.name == name); }) != zones.end());
}

Zones::Controller& Zones::Manager::GetZone(const string& name)
{
  auto it = find_if(zones.begin(), zones.end(), [name](Controller* controller) -> bool { return (controller->zone.name == name); });
  
  if (it == zones.end())
    throw "bite"; // TODO throw some real exception
  return (**it);
}

const Zones::Controller& Zones::Manager::GetZone(const string& name) const
{
  auto it = find_if(zones.begin(), zones.end(), [name](Controller* controller) -> bool { return (controller->zone.name == name); });

  if (it == zones.end())
    throw "bite"; // TODO throw some real exception
  return (**it);
}
