#include "ui/gameui.hpp"
#include <Rocket/Controls.h>

using namespace std;
using namespace Rocket;

/*
 * GameUi
 */
GameUi::GameUi(WindowFramework* window, PT(RocketRegion) rocket) : _rocket(rocket)
{
  _menu      = new GameMenu      (window, _rocket->get_context());
  _inventory = new GameInventory (window, _rocket->get_context());
  _pers      = new StatViewRocket(window, _rocket->get_context());
  _menu->Hide();
  _inventory->Hide();
  _pers->Hide();
}

GameUi::~GameUi(void)
{
  _menu->Destroy();
  _inventory->Destroy();
  _pers->Destroy();
  delete _menu;
  delete _inventory;
  delete _pers;
}

void GameUi::OpenMenu(Rocket::Core::Event&)
{
  _menu->Show();
}

void GameUi::OpenInventory(Rocket::Core::Event&)
{
  if (!(_inventory->IsVisible()))
    _inventory->Show();
  else
    _inventory->Hide();
}

void GameUi::OpenPers(Rocket::Core::Event&)
{
  if (!(_pers->IsVisible()))
    _pers->Show();
  else
    _pers->Hide();
}
