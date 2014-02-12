#include "globals.hpp"
#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>
#include <panda3d/load_prc_file.h>
#include <panda3d/graphicsStateGuardianBase.h>
#include <panda3d/pStatClient.h>
//#define PSTAT_ENABLED

#include "mainmenu.hpp"
#include "options.hpp"
#include "scriptengine.hpp"
#include "dices.hpp"

void AngelScriptInitialize(void);
int  compile_statsheet(std::string);
int  compile_heightmap(const std::string& sourcefile, const std::string& out);

PandaFramework*      framework   = NULL;

using namespace std;

#ifndef UNIT_TESTER
int main(int argc, char *argv[])
{
  PandaFramework panda_framework;

  framework   = &panda_framework;

  Dices::Initialize();          // Randomness initialization
  Script::Engine::Initialize(); // Script Engine initialization (obviously)
  AngelScriptInitialize();      // Registering script API (see script_api.cpp)

  // With some options, game binary can also be used to compile statsheet or heightmaps.
  // If used as compiler of some sort
  if (argc == 3 && std::string(argv[1]) == "--compile-statsheet")
    return (compile_statsheet(argv[2]));
  if (argc == 4 && std::string(argv[1]) == "--compile-heightmap")
    return (compile_heightmap(argv[2], argv[3]));
  // Otherwise run the game
  {
    WindowFramework* window;
    ConfigPage*      config = load_prc_file("config.prc");

    cout << "[FoE] Loading configuration" << endl;
#ifdef PSTAT_ENABLED
    if (!(PStatClient::connect("localhost", 5185))) // Initialize client for profiling collector
      cout << "[FoE] Can't connect to PStat client" << endl;
#endif
    cout << "[FoE] Opening Panda3D Framework" << endl;
    panda_framework.open_framework(argc, argv);
    cout << "[FoE] Setting window's title" << endl;
    panda_framework.set_window_title("Fallout Equestria");
    cout << "[FoE] Opening Window" << endl;
    window = panda_framework.open_window();
    if (window == 0)
    {
      cout << "[FoE] Panda3D failed to create a display. Aborting now." << endl;
      return (-1);
    }
    cout << "[FoE] Enabling keyboard" << endl;
    //window->enable_keyboard();
    window->get_render().set_shader_auto();

    // Set Windows Properties
    cout << "[FoE] Setting window's properties" << endl;
    {
      WindowProperties props = window->get_graphics_window()->get_properties();

      cout << "[FoE] Hiding system cursor" << endl;
      props.set_cursor_hidden(true);
      cout << "[FoE] Setting window icon" << endl;
      props.set_icon_filename("data/icon.ico");
      cout << "[FoE] Applying changes" << endl;
      window->get_graphics_window()->request_properties(props);
    }

    cout << "[FoE] Loading configuration" << endl;
    OptionsManager::Initialize(); // Loads and handle conf.json file
    {
      MainMenu       mainMenu(window); // MainMenu will take over as a Panda3D async_task.

      cout << "[FoE] Starting Main Loop" << std::endl;
      panda_framework.main_loop();
    }
    unload_prc_file(config);
    cout << "[FoE] Properly wrapping up." << endl;
    OptionsManager::Finalize();
    Script::Engine::Finalize();
  }
  return (0);
}
#endif

