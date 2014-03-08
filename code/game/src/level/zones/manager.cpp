#include "level/zones/manager.hpp"
#include <level/zones/exception.hpp>
#include "level/level.hpp"
#include <ui/alert_ui.hpp>
 
using namespace std;

Zones::Manager::Manager(Level& level) : level(level)
{
}

Zones::Manager::~Manager()
{
  UnregisterAllZones();
  UnregisterAllPassageways();
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

Zones::PassageWay* Zones::Manager::RegisterPassageway(const std::vector<Waypoint*>& waypoints)
{
  PassageWay* passage_way = new PassageWay;
  
  passage_way->waypoints          = waypoints;
  passage_way->CanGoThrough       = [](InstanceDynamicObject*) -> bool { return (true); };
  passage_ways.push_back(passage_way);
  InitializePassagewayObservers(*passage_way);
  return (passage_way);
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

Zones::Observer* Zones::Manager::InitializeObserver(Waypoint* waypoint)
{
  Observer* observer = FindObserver(waypoint);

  if (!observer)
  {
    observer = new Observer(waypoint);

    observers.push_back(observer);
  }
  return (observer);
}

void Zones::Manager::InitializeZoneObservers(Controller& entry)
{
  auto     it     = entry.zone.waypoints.begin();
  auto     end    = entry.zone.waypoints.end();

  for (; it != end ; ++it)
  {
    Observer* observer = InitializeObserver(*it);

    observer->AddObserver(&entry);
  }
}

void Zones::Manager::InitializePassagewayObservers(PassageWay& entry)
{
  auto it  = entry.waypoints.begin();
  auto end = entry.waypoints.end();
  
  for (; it != end ; ++it)
  {
    Observer* observer = InitializeObserver(*it);

    observer->AddObserver(&entry);
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
      if (!observer.HasObservers())
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

void Zones::Manager::UnregisterPassageway(Zones::PassageWay* passage_way)
{
  auto it = find(passage_ways.begin(), passage_ways.end(), passage_way);
  
  if (it != passage_ways.end())
  {
    auto observers_it  = observers.begin();
    auto observers_end = observers.end();

    while (observers_it != observers_end)
    {
      Observer& observer = **observers_it;

      std::remove(observer.passage_ways.begin(), observer.passage_ways.end(), passage_way);
      if (!observer.HasObservers())
      {
        observers_it = observers.erase(observers_it);
      }
      else
        observers_it++;
    }
    passage_ways.erase(it);
    delete passage_way;
  }
}

void Zones::Manager::UnregisterAllZones(void)
{
  while (zones.begin() != zones.end())
    UnregisterZone((*zones.begin())->zone.name);
}

void Zones::Manager::UnregisterAllPassageways(void)
{
  while (passage_ways.begin() != passage_ways.end())
    UnregisterPassageway(*passage_ways.begin());
}

void Zones::Manager::InsertPartyInZone(Party& party, const string& zone)
{
  try
  {
    Controller& controller = GetZone(zone);
    auto        iterator   = party.GetPartyMembers().begin();
    auto        end        = party.GetPartyMembers().end();

    for (; iterator != end ; ++iterator)
    {
      Party::Member*   member    = *iterator;
      ObjectCharacter* character = level.GetCharacter(member->GetDynamicObject().name);

      if (character)
        controller.InsertObject(character);
    }
  }
  catch (ZoneException exception)
  {
    cerr << "Inserting party '" << party.GetName() << "': " << exception.what() << endl;
    exception.Display();
  }
}

void Zones::Manager::InsertObjectInZone(InstanceDynamicObject* object, const string& zone)
{
  try
  {
    Controller& controller = GetZone(zone);

    controller.InsertObject(object);
  }
  catch (ZoneException exception)
  {
    cerr << "Inserting object '" << object->GetName() << "': " << exception.what() << endl;
  }
}

bool Zones::Manager::ZoneExists(const string& name) const
{
  return (find_if(zones.begin(), zones.end(), [name](Controller* controller) -> bool { return (controller->zone.name == name); }) != zones.end());
}

bool Zones::Manager::IsInsideExitZone(Waypoint* waypoint)
{
  auto it  = zones.begin();
  auto end = zones.end();
  
  for (; it != end ; ++it)
  {
    Controller* zone_controller = *it;
    
    if (zone_controller->IsEnabled() && zone_controller->IsExitZone() && zone_controller->IsInZone(waypoint))
      return (true);
  }
  return (false);
}

Zones::Controller& Zones::Manager::GetZone(const string& name)
{
  auto it = find_if(zones.begin(), zones.end(), [name](Controller* controller) -> bool { return (controller->zone.name == name); });
  
  if (it == zones.end())
    throw ZoneNotFound(name, "Could not find such zone");
  return (**it);
}

const Zones::Controller& Zones::Manager::GetZone(const string& name) const
{
  auto it = find_if(zones.begin(), zones.end(), [name](Controller* controller) -> bool { return (controller->zone.name == name); });

  if (it == zones.end())
    throw ZoneNotFound(name, "Could not find such zone");
  return (**it);
}
