#include "gameui.hpp"

using namespace std;
using namespace Rocket;

GameMenu::GameMenu(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  Rocket::Core::ElementDocument* doc     = context->LoadDocument("data/main_menu.rml");

  _root = doc;
  if (doc)
  {
    doc->Show();

    ToggleEventListener(true, "continue", "click", _continueClicked);
    ToggleEventListener(true, "options",  "click", _optionsClicked);
    ToggleEventListener(true, "exit",     "click", _exitClicked);
    ToggleEventListener(true, "save",     "click", _saveClicked);
    ToggleEventListener(true, "load",     "click", _loadClicked);

    _continueClicked.EventReceived.Connect(*this, &GameMenu::MenuEventContinue);
    _exitClicked.EventReceived.Connect(ExitClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
    _saveClicked.EventReceived.Connect(SaveClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
    _loadClicked.EventReceived.Connect(LoadClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
    _optionsClicked.EventReceived.Connect(OptionsClicked, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);

    Translate();
  }
}

GameMenu::~GameMenu()
{
  ToggleEventListener(false, "continue", "click", _continueClicked);
  ToggleEventListener(false, "options",  "click", _optionsClicked);
  ToggleEventListener(false, "exit",     "click", _exitClicked);
  ToggleEventListener(false, "save",     "click", _saveClicked);
  ToggleEventListener(false, "load",     "click", _loadClicked);
}

