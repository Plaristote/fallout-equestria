#include "ui/general_ui.hpp"
#include <Rocket/Controls.h>
#include <iostream>
#define PATH_FONTS "data"

using namespace std;
using namespace Rocket;

extern PandaFramework* framework;

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
  window->get_mouse().attach_new_node(_ih);
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

