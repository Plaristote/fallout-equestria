#include "ui/gameui.hpp"
#include <executor.hpp>
#include <executor.hpp>
#include <Rocket/Controls.h>

using namespace std;
using namespace Rocket;

/*
 * GameUi
 */
GameUi::GameUi(WindowFramework* window, PT(RocketRegion) rocket) : window(window), _rocket(rocket)
{
  ui_load_game = 0;
  ui_save_game = 0;
  _menu         = new GameMenu      (window, _rocket->get_context());
  _inventory    = new GameInventory (window, _rocket->get_context());
  _pers         = new StatViewRocket(window, _rocket->get_context());
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
  if (ui_load_game) { delete ui_load_game; }
  if (ui_save_game) { delete ui_save_game; }
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

UiLoad* GameUi::OpenLoadingInterface(const string& savepath)
{
  if (ui_load_game)
    delete ui_load_game;
  ui_load_game = new UiLoad(window, _rocket->get_context(), savepath);
  ui_load_game->Done.Connect([this](void)
  {
    UiLoad* to_garbage_collect = ui_load_game;
    
    Executor::ExecuteLater([to_garbage_collect]() { delete to_garbage_collect; });
    ui_load_game = 0;
  });
  ui_load_game->Show();
  return (ui_load_game);
}

UiSave* GameUi::OpenSavingInterface(const string& savepath)
{
  if (ui_save_game)
    delete ui_save_game;
  ui_save_game = new UiSave(window, _rocket->get_context(), savepath);
  ui_save_game->Done.Connect([this](void)
  {
    UiSave* to_garbage_collect = ui_save_game;
    
    Executor::ExecuteLater([to_garbage_collect]() { delete to_garbage_collect; });
    ui_save_game = 0;
  });
  ui_save_game->Show();
  return (ui_save_game);
}