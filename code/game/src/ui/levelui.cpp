#include "ui/level_ui.hpp"
#include <iostream>

using namespace std;

LevelUi::LevelUi(WindowFramework* window, GameUi& gameUi) : _gameUi(gameUi)
{
  cout << "- Initializing Level Ui" << endl;
  _mainBar = new GameMainBar(window, _gameUi.GetRocketRegion()->get_context());

  _obs.Connect(_mainBar->MenuButtonClicked.EventReceived,      _gameUi,             &GameUi::OpenMenu);
  _obs.Connect(_mainBar->InventoryButtonClicked.EventReceived, _gameUi,             &GameUi::OpenInventory);
  _obs.Connect(_mainBar->PersButtonClicked.EventReceived,      _gameUi,             &GameUi::OpenPers);
  _obs.Connect(_mainBar->PipbuckButtonClicked.EventReceived,   _gameUi.OpenPipbuck, &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
  _obs.Connect(_gameUi.GetInventory().VisibilityToggled, InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  _obs.Connect(_gameUi.GetMenu().VisibilityToggled,      InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  _obs.Connect(_gameUi.GetPers().VisibilityToggled,      InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  cout << "-> Done." << endl;
}

LevelUi::~LevelUi(void)
{
  _obs.DisconnectAll();
  delete _mainBar;
}

