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
void Earthscultor2Bam(Data heightmap, const std::string& out)
{
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
  Dices::Initialize();
  Script::Engine::Initialize();
  AngelScriptInitialize();

  // If used as compiler of some sort
  if (argc == 3 && std::string(argv[1]) == "--compile-statsheet")
    return (compile_statsheet(argv[2]));
  if (argc == 4 && std::string(argv[1]) == "--compile-heightmap")
    return (compile_heightmap(argv[2], argv[3]));
  // Otherwise run the game

  WindowFramework* window;
  ConfigPage*      config = load_prc_file("config.prc");

  if (!(PStatClient::connect("localhost", 5185)))
    cout << "Can't connect to PStat client" << endl;
  framework.open_framework(argc, argv);
  framework.set_window_title("Fallout Equestria");
  window = framework.open_window();
  window->enable_keyboard();
  window->get_render().set_shader_auto();

  // Set Windows Properties
  {
    WindowProperties props = window->get_graphics_window()->get_properties();

    props.set_cursor_hidden(true);
    props.set_icon_filename("data/icon.ico");
    window->get_graphics_window()->request_properties(props);
  }

  OptionsManager::Initialize();
  {
    MainMenu       mainMenu(window);

    framework.main_loop();
    framework.close_framework();

    unload_prc_file(config);
  }
  OptionsManager::Finalize();
  Script::Engine::Finalize();
  return (0);
}
#endif

