#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>

#include <panda3d/texturePool.h>
#include <panda3d/directionalLight.h>

PandaFramework       framework;
PT(AsyncTaskManager) taskMgr     = AsyncTaskManager::get_global_ptr();
PT(ClockObject)      globalClock = ClockObject::get_global_clock();

# include "mainmenu.hpp"
# include "options.hpp"

using namespace std;

#include <panda3d/load_prc_file.h>
#include <panda3d/graphicsStateGuardianBase.h>
#include <panda3d/pStatClient.h>
#include <panda3d/audioManager.h>

# include "scriptengine.hpp"

#include "serializer.hpp"
#include <i18n.hpp>
#include <level/objects/shelf.hpp>
#include <level/objects/locker.hpp>
#include <level/divide_and_conquer.hpp>

#include "musicmanager.hpp"
#include <soundmanager.hpp>
#include <dices.hpp>

template<typename TYPE>
struct RecursiveChecker
{
  typedef std::queue<RecursiveChecker>     Queue;
  typedef std::function<bool (TYPE, TYPE)> CompFunc;
  
  RecursiveChecker(TYPE first, TYPE second, CompFunc comp) : first(first), second(second), comp(comp)
  {
  }
  
  bool Execute(std::queue<RecursiveChecker> checkers)
  {
    if (first == second)
    {
      checkers.pop();
      if (!(checkers.empty()))
        return (checkers.front().Execute(checkers));
      return (true);
    }
    return (comp(first, second));
  }

  TYPE                             first, second;
  std::function<bool (TYPE, TYPE)> comp;
};

class DateTime
{
public:
  
  bool operator>(const DateTime& time)
  {
    RecursiveChecker<int>::CompFunc comp([](int a, int b) { return (a < b); });

    return (recursive_compare(comp, time));
  }
  
  bool operator<(const DateTime& time)
  {
    RecursiveChecker<int>::CompFunc comp([](int a, int b) { return (a > b); });

    return (recursive_compare(comp, time));
  }
  
private:
  bool recursive_compare(RecursiveChecker<int>::CompFunc comp, const DateTime& time)
  {
    RecursiveChecker<int>::Queue    checkers;

    checkers.push(RecursiveChecker<int>(year,   time.year,   comp));
    checkers.push(RecursiveChecker<int>(month,  time.month,  comp));
    checkers.push(RecursiveChecker<int>(day,    time.day,    comp));
    checkers.push(RecursiveChecker<int>(hour,   time.hour,   comp));
    checkers.push(RecursiveChecker<int>(minute, time.minute, comp));
    checkers.push(RecursiveChecker<int>(second, time.second, comp));    
    return (checkers.front().Execute(checkers));
  }
  
  unsigned short year;
  unsigned char  month, day, hour, minute, second;
};

string humanize(const std::string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if (i == 0 || str[i - 1] == '_')
      ret += str[i] - 'a' + 'A';
    else if (str[i] == '_')
      ret += ' ';
    else
      ret += str[i];
  }
  return (ret);
}

string underscore(const std::string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if      (str[i] >= 'A' && str[i] <= 'Z')
      ret += str[i] - 'A' + 'a';
    else if (str[i] == ' ')
      ret += '_';
    else
      ret += str[i];
  }
  return (ret);
}

void AngelScriptInitialize(void);
int  compile_statsheet(std::string);

#include <panda3d/geoMipTerrain.h>

LPoint3 NodePathSize(NodePath);

void Earthscultor2Bam(Data heightmap, const std::string& out)
{
  // Experimental code for cutting maps in parts. Didn't work. Unsuccessfull texturing. No performance improvements.
  /*NodePath     result = new PandaNode("EarthScultor Heightmap");
  LPoint3f     size_part;
  bool         size_set = false;
  LPoint2      map_it(0, 0);
  unsigned int dimensions;

  dimensions = sqrt(heightmap.Count());
  for_each(heightmap.begin(), heightmap.end(), [&result, &size_part, &size_set, &map_it, dimensions](Data map_part)
  {
    NodePath       terrain;
    GeoMipTerrain* mip_terrain = new GeoMipTerrain("Terrain");

    mip_terrain->set_heightfield(map_part["heightmap"].Value());
    mip_terrain->set_block_size(map_part["block_size"]);
    mip_terrain->set_factor(map_part["factor"]);
    mip_terrain->set_min_level(map_part["min_level"]);

    mip_terrain->get_root().set_sz(map_part["max_height"]);

    mip_terrain->generate();
    terrain = mip_terrain->get_root();
    terrain.reparent_to(result);

    if (!size_set)
    {
      size_part = NodePathSize(terrain);
      size_set  = true;
      terrain.set_pos(LPoint3(0, 0, 0));
      std::cout << "Map part size: (" << size_part.get_x() << ',' << size_part.get_y() << ',' << size_part.get_z() << ')' << std::endl;
    }
    else
    {
      terrain.set_x((map_it.get_x() * size_part.get_x()));
      terrain.set_y((map_it.get_y() * size_part.get_y()));
    }
    map_it.set_x(map_it.get_x() + 1);
    if (map_it.get_x() == dimensions)
    {
      map_it.set_x(0);
      map_it.set_y(map_it.get_y() + 1);
    }
  });
  result.write_bam_file(out);*/

  NodePath terrain;

  GeoMipTerrain* mip_terrain = new GeoMipTerrain("Terrain");

  mip_terrain->set_heightfield(heightmap["heightmap"].Value());
  mip_terrain->set_block_size(heightmap["block_size"]);
  mip_terrain->set_factor(heightmap["factor"]);
  mip_terrain->set_min_level(heightmap["min_level"]);

  mip_terrain->get_root().set_sz(heightmap["max_height"]);

  mip_terrain->generate();
  mip_terrain->get_root().write_bam_file(out);

  delete mip_terrain;
}

int compile_heightmap(const std::string& sourcefile, const std::string& out)
{
  DataTree* datatree = DataTree::Factory::JSON(sourcefile);

  if (datatree)
  {
    Earthscultor2Bam(datatree, out);
    delete datatree;
    return (0);
  }
  return (-1);
}

#ifndef UNIT_TESTER
int main(int argc, char *argv[])
{
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
    if (!(PStatClient::connect("localhost", 5185))) // Initialize client for profiling collector
      cout << "[FoE] Can't connect to PStat client" << endl;
    cout << "[FoE] Opening Panda3D Framework" << endl;
    framework.open_framework(argc, argv);
    cout << "[FoE] Setting window's title" << endl;
    framework.set_window_title("Fallout Equestria");
    cout << "[FoE] Opening Window" << endl;
    window = framework.open_window();
    cout << "[FoE] Enabling keyboard" << endl;
    //window->enable_keyboard();
    //window->get_render().set_shader_auto();

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
      framework.main_loop();
      framework.close_framework();

      unload_prc_file(config);
    }
    cout << "[FoE] Properly wrapping up." << endl;
    OptionsManager::Finalize();
    Script::Engine::Finalize();
  }
  return (0);
}
#endif

