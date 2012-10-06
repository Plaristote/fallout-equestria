#include "level/interact_menu.hpp"
#include <panda3d/mouseButton.h>
#include <panda3d/mouseWatcher.h>
#include "level/level.hpp"

using namespace std;

InteractMenu::InteractMenu(WindowFramework* window, Rocket::Core::Context* context, InstanceDynamicObject& object) : UiBase(window, context)
{
  InstanceDynamicObject::InteractionList& interactions = object.GetInteractions();

  _done = false;
  _root = context->LoadDocument("data/interact_menu.rml");
  if (_root)
  {
    Rocket::Core::Element* element = _root->GetElementById("menu");
    
    if (element)
    {
      // Positioning the interact_menu
      {
	std::stringstream strTop, strLeft;
	MouseData    pointer = _window->get_graphics_window()->get_pointer(0);

	strTop  << (pointer.get_y());
	strLeft << (pointer.get_x());
	element->SetProperty("position", "absolute");
	element->SetProperty("top",  strTop.str().c_str());
	element->SetProperty("left", strLeft.str().c_str());
      }

      // Generating and setting the RML for the interact_menu
      {
	std::stringstream rml;
	
	for_each(interactions.begin(), interactions.end(), [&rml](InstanceDynamicObject::Interaction& interaction)
	{
	  rml << "<div id='interaction-" << interaction.name << "'>";
	  rml << "<button id='" << interaction.name << "'>";
	  rml << "<img src='../textures/buttons/" + interaction.name + "-normal.png' />";
	  rml << "</button></div>";
	});

	element->SetInnerRML(rml.str().c_str());
      }

      int it = 0;
      _listeners.resize(interactions.size());

      for_each(interactions.begin(), interactions.end(), [this, &it](InstanceDynamicObject::Interaction& interaction)
      {
	Rocket::Core::Element* button = _root->GetElementById(interaction.name.c_str());

	_buttons.push_back(button);
	button->AddEventListener("click", &_buttonListener);
	_listeners[it] = &interaction;
	_obs.Connect(_buttonListener.EventReceived, *this, &InteractMenu::ButtonClicked);
	++it;
      });
    }
  }
  Show();
}

InteractMenu::~InteractMenu()
{
  for_each(_buttons.begin(), _buttons.end(), [this](Rocket::Core::Element* button)
  { button->RemoveEventListener("click", &_buttonListener); });
  Hide();
  _obs.DisconnectAll();
}

void InteractMenu::ButtonClicked(Rocket::Core::Event& event)
{
  if (_done) return;
  for (int i = 0 ; i < _listeners.size() ; ++i)
  {
    if (event.GetCurrentElement()->GetId() == _listeners[i]->name.c_str())
    {
      _listeners[i]->signal->Emit(_listeners[i]->instance);
      _done = true;
      break ;
    }
  }
}
