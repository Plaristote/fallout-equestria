#include "ui/ui_next_zone.hpp"

using namespace std;

UiNextZone::UiNextZone(WindowFramework* window, Rocket::Core::Context* context, const std::vector<std::string> zones)
  : UiBase(window, context)
{
  _root = context->LoadDocument("data/dialog_zone_selector.rml");
  if (_root)
  {
    Rocket::Core::Element*         eContainer = _root->GetElementById("choices");
    vector<string>::const_iterator it         = zones.begin();
    vector<string>::const_iterator end        = zones.end();
    Rocket::Core::String           lastRml;
    stringstream                   rml;
    
    for(short n = 1 ; it != end ; ++it, ++n)
    {
      rml << "<button id='choice-" << n << "' class='button_menu' zone='" << *it << "'>";
      rml << *it;
      rml << "</button>";
      rml << "<br />";
    }
    rml << "<button id='cancel' class='button_menu'>Stay here</button>";
    eContainer->GetInnerRML(lastRml);
    eContainer->SetInnerRML(lastRml + rml.str().c_str());

    {
      ToggleEventListener(true, "cancel", "click", CancelSelected);
      for (unsigned short n = 1 ; n <= zones.size() ; ++n)
      {
	stringstream           name;
	Rocket::Core::Element* zoneButton;

	name << "choice-" << n;
	zoneButton = _root->GetElementById(name.str().c_str());
	if (zoneButton)
	{
	  _elements.push_back(zoneButton);
	  zoneButton->AddEventListener("click", &LevelSelected);
	}
      }
      LevelSelected.EventReceived.Connect (*this, &UiNextZone::CallbackLevelSelected);
      CancelSelected.EventReceived.Connect(*this, &UiNextZone::CallbackCancel);
    }
    _root->Show();
  }
}

void UiNextZone::CallbackLevelSelected(Rocket::Core::Event& event)
{
  Rocket::Core::String name = event.GetCurrentElement()->GetId();
  Rocket::Core::String str  = event.GetCurrentElement()->GetAttribute("zone")->Get<Rocket::Core::String>();
  string               tmp = str.CString();

  NextZoneSelected.Emit(tmp);
  Done.Emit();
}

void UiNextZone::CallbackCancel(Rocket::Core::Event&)
{
  Cancel.Emit();
}

UiNextZone::~UiNextZone()
{
  ToggleEventListener(false, "cancel", "click", CancelSelected);
  for_each(_elements.begin(), _elements.end(), [this](Rocket::Core::Element* zoneButton)
  { zoneButton->RemoveEventListener("click", &LevelSelected); });
}
