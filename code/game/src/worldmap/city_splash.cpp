#include "worldmap/city_splash.hpp"
#include <sstream>

using namespace std;

CitySplash::CitySplash(Data data, WindowFramework* window, Rocket::Core::Context* context, Rocket::Core::ElementDocument* root) : UiBase(window, context)
{
  this->root                = root;
  root_outlives_this_object = true;

  Rocket::Core::Element* splash      = root->GetElementById("city-splash-image");
  Data                   entry_zones = data["zones"];
  Rocket::Core::Element* zoneroot   = root->GetElementById("entry-zones");

  if (splash)
  {
    string path = "../textures/" + data["splashscreen"].Value();

    splash->SetAttribute("src", path.c_str());
  }
  if (zoneroot)
  {
    for_each(entry_zones.begin(), entry_zones.end(), [this, zoneroot](Data zone)
    {
      if (zone == '1')
      {
        Rocket::Core::String rml;
        stringstream         stream;
        std::string          zone_name = "zone-" + underscore(zone.Key());

        stream << "<div class='zone-entry'>";
        stream <<   "<button class='long_button zone-button' id='" << zone_name << "'>";
        stream <<     zone.Key();
        stream <<   "</button>";
        stream << "</div>";
        rml = stream.str().c_str();
        zoneroot->SetInnerRML(rml);
        {
          RocketListener* listener = new RocketListener;

          ToggleEventListener(true, zone_name, "click", *listener);
          listener->EventReceived.Connect(*this, &CitySplash::ZonePicked);
          zone_listeners.insert(CitySplash::ZoneListeners::value_type(zone.Key(), listener));
        }
      }
    });
  }
  ToggleEventListener(true, "button-cancel", "click", CancelClicked);
  CancelClicked.EventReceived.Connect([this](Rocket::Core::Event&)
  {
    Canceled.Emit();
  });
}

CitySplash::~CitySplash()
{
  for_each(zone_listeners.begin(), zone_listeners.end(), [this](ZoneListeners::value_type listener)
  {
    string name = "zone-" + humanize(listener.first);

    ToggleEventListener(false, name, "click", *listener.second);
  });
}

void CitySplash::Show(void)
{
  SetSplashVisible(true);
  SetZonePickerVisible(true);
}

void CitySplash::Hide(void)
{
  SetSplashVisible(false);
  SetZonePickerVisible(false);
}

void CitySplash::SetSplashVisible(bool visible)
{
  Rocket::Core::Element* map_frame   = root->GetElementById("map-frame");
  Rocket::Core::Element* city_splash = root->GetElementById("city-splash");
  
  if (map_frame && city_splash)
  {
    map_frame->SetProperty  ("display", visible ? "none"  : "block");
    city_splash->SetProperty("display", visible ? "block" : "none");
  }
}

void CitySplash::SetZonePickerVisible(bool visible)
{
  Rocket::Core::Element* city_list = root->GetElementById("city-container");
  Rocket::Core::Element* zone_list = root->GetElementById("entry-zone-container");
  
  if (city_list && zone_list)
  {
    city_list->SetProperty("display", visible ? "none"  : "block");
    zone_list->SetProperty("display", visible ? "block" : "none");
  }
}

void CitySplash::ZonePicked(Rocket::Core::Event& event)
{
  Rocket::Core::Element*  element   = event.GetCurrentElement();
  std::string             zone_name = element->GetId().CString();
  ZoneListeners::iterator item;

  zone_name = zone_name.substr(5);
  item      = zone_listeners.find(zone_name);
  if (item != zone_listeners.end())
    EntryZonePicked.Emit(zone_name);
}
