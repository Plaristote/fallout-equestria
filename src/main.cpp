#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>

#include <panda3d/texturePool.h>
#include <panda3d/directionalLight.h>

PandaFramework       framework;
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject)      globalClock = ClockObject::get_global_clock();

# include "level.hpp"

using namespace std;

#include <panda3d/graphicsStateGuardianBase.h>
#include <panda3d/pStatClient.h>

int main(int argc, char *argv[])
{
    //open a new window framework
  framework.open_framework(argc, argv);
    //set the window title to My Panda3D Window
  framework.set_window_title("Fallout Equestria");
    //open the window
  WindowFramework *window = framework.open_window();

  window->enable_keyboard();
  window->get_render().set_shader_auto();

  if (!(PStatClient::connect("localhost", 5185)))
    cout << "Can't connect to PStat client" << endl;
  

  Level level(window, "test");

    //do the main loop, equal to run() in python
  framework.main_loop();
    //close the window framework
  framework.close_framework();
  return (0);
}
