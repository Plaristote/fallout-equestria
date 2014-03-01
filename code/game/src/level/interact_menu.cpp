#include "level/interact_menu.hpp"
#include <panda3d/mouseButton.h>
#include <panda3d/mouseWatcher.h>
#include "level/level.hpp"

using namespace std;

InteractMenu::InteractMenu(WindowFramework* window, Rocket::Core::Context* context, InstanceDynamicObject& object) : UiBase(window, context)
{
  Interactions::InteractionList& interactions = object.GetInteractions();

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
        
        for_each(interactions.begin(), interactions.end(), [&rml](Interactions::Interaction& interaction)
        {
          rml << "<div id='interaction-" << interaction.name << "'>";
          rml << "<button id='" << interaction.name << "' class='interact_button'>";
          rml << "<img src='../textures/buttons/" + interaction.name + "-normal.png' />";
          rml << "</button></div>";
        });

        element->SetInnerRML(rml.str().c_str());
      }

      int it = 0;
      _listeners.resize(interactions.size());

      for_each(interactions.begin(), interactions.end(), [this, &it](Interactions::Interaction& interaction)
      {
        Rocket::Core::Element* button = _root->GetElementById(interaction.name.c_str());

        ToggleEventListener(true, interaction.name, "click",     _buttonListener);
        ToggleEventListener(true, interaction.name, "mouseover", _buttonHover);
        ToggleEventListener(true, interaction.name, "mouseout",  _buttonHover);
        ToggleEventListener(true, interaction.name, "mousedown", _buttonClick);
        ToggleEventListener(true, interaction.name, "mouseup",   _buttonClick);
        _listeners[it] = &interaction;
        _obs.Connect(_buttonListener.EventReceived, *this, &InteractMenu::ButtonClicked);
        _obs.Connect(_buttonHover.EventReceived,    *this, &InteractMenu::ButtonHovered);
        _obs.Connect(_buttonClick.EventReceived,    *this, &InteractMenu::MouseButton);
        ++it;
      });
    }
  }
  Show();
}

void InteractMenu::ExecuteForButtonId(Rocket::Core::Event& event, std::function<bool (Rocket::Core::Event&, const std::string&, Interactions::Interaction*)> callback)
{
  if (!_done)
  {
    string id         = event.GetCurrentElement()->GetId().CString();
    string event_type = event.GetType().CString();

    for (unsigned int i = 0 ; i < _listeners.size() ; ++i)
    {
      if (id == _listeners[i]->name && (callback(event, event_type, _listeners[i])))
        break ;
    }
  }
}

void InteractMenu::ButtonHovered(Rocket::Core::Event& event)
{
  ExecuteForButtonId(event, [this](Rocket::Core::Event& event, const string& event_type, Interactions::Interaction* interaction) -> bool
  {
    bool                   mouse_over = event_type == "mouseover";
    Rocket::Core::Element* img        = event.GetCurrentElement()->GetChild(0);
    string                 id         = event.GetCurrentElement()->GetId().CString();
    string                 src        = "../textures/buttons/" + id + '-' + (mouse_over ? "active" : "normal") + ".png";

    img->SetAttribute("src", src.c_str());
    return (true);
  });
}

void InteractMenu::MouseButton(Rocket::Core::Event& event)
{
  ExecuteForButtonId(event, [this](Rocket::Core::Event& event, const string& event_type, Interactions::Interaction* interaction) -> bool
  {
    bool                   mouse_over = event_type == "mousedown";
    Rocket::Core::Element* img        = event.GetCurrentElement()->GetChild(0);
    string                 id         = event.GetCurrentElement()->GetId().CString();
    string                 src        = "../textures/buttons/" + id + '-' + (mouse_over ? "pressed" : "normal") + ".png";

    img->SetAttribute("src", src.c_str());
    return (true);
  });
}

InteractMenu::~InteractMenu()
{
  for_each(_buttons.begin(), _buttons.end(), [this](Rocket::Core::Element* button)
  {
    button->RemoveEventListener("click",     &_buttonListener);
    button->RemoveEventListener("mouseover", &_buttonHover);
    button->RemoveEventListener("mouseout",  &_buttonHover);
    button->RemoveEventListener("mousedown", &_buttonClick);
    button->RemoveEventListener("mouseup",   &_buttonClick);
  });
  Hide();
  _obs.DisconnectAll();
}

void InteractMenu::ButtonClicked(Rocket::Core::Event& event)
{
  if (_done) return;
  for (unsigned int i = 0 ; i < _listeners.size() ; ++i)
  {
    if (event.GetCurrentElement()->GetId() == _listeners[i]->name.c_str())
    {
      _listeners[i]->signal->Emit(_listeners[i]->instance);
      _done = true;
      break ;
    }
  }
}
