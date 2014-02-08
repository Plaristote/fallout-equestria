#include "gameui.hpp"
#include <Rocket/Controls.h>

using namespace std;
using namespace Rocket;

extern PandaFramework* framework;

NodePath c_attach_new_node(NodePath, PandaNode*);

/*
 * GeneralUi
 */
#define PATH_FONTS "data"

GeneralUi::GeneralUi(WindowFramework* window) : _window(window)
{
  //HAIL MICROSOFT!!!
  string fonts[] = { "JH_FALLOUT.TTF", // Fallout Console Font
                     "fallout.ttf",    // Fallout Regular Font
                     "Delicious-Roman.otf",
                     "Delicious-Italic.otf",
                     "Delicious-Bold.otf",
                     "Delicious-BoldItalic.otf" };

  string fonts_name[] = { "", "Fallout", "", "", "", "" };

  _rocket = RocketRegion::make("interface", window->get_graphics_output());
  _rocket->set_active(true);
  Core::Initialise();
  for (unsigned int i=0; i<GET_ARRAY_SIZE(fonts); i++)
  {
    string path = string(PATH_FONTS) + "/" + fonts[i];
	
	cout << "[UI] Loading font '" << fonts_name[i] << "' " << path << endl;
    if (fonts_name[i] == "")
      Core::FontDatabase::LoadFontFace(path.c_str());
    else
      Core::FontDatabase::LoadFontFace(path.c_str(), fonts_name[i].c_str(), Core::Font::STYLE_NORMAL, Rocket::Core::Font::WEIGHT_NORMAL);
  }
  cout << "[UI] Fonts loaded" << endl;

  Rocket::Controls::Initialise();

  _ih = new RocketInputHandler();
  cout << "[UI] Binding mouse and Rocket" << endl;
  c_attach_new_node(window->get_mouse(), _ih);
  cout << "[UI] Set rocket input handler" << endl;
  _rocket->set_input_handler(_ih);
  cout << "[UI] Input handler initialized" << endl;

  _console = new GameConsole(window, _rocket->get_context());
  _console->Hide();
  cout << "[UI] GameConsole ready" << endl;
  
  cout << "Game Options creating" << endl;
  _options = new GameOptions(window, _rocket->get_context());
  _options->Hide();
  cout << "[UI] GameOptions ready" << endl;

  _window->enable_keyboard();
  //framework->define_key("tab", "ConsoleHandle", GameConsole::Toggle, (void*)_console);  
  cout << "[UI] Keyboard shortcut initialized" << endl;
}

GeneralUi::~GeneralUi(void)
{
  //framework->define_key("tab", "ConsoleHandle", 0, 0);
  delete _console;
  delete _options;
}

/*
 * LevelUi
 */
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
