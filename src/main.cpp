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

# include "pandagui.hpp"

using namespace PandaUi;

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
  /*Widget widget(window, "inventory");

  widget.SetBackground("data/inventory.png");
  widget.SetPosition(LVector2f(50.f, 100.f));

  Widget widget2(&widget, "inventory-items");

  widget2.SetBackground("data/inventory-items.png");
  widget2.SetPosition(LVector2f(10.f, 10.f));*/

  /*MeshDrawer2D mesh2d;

  PT(Texture) _background = TexturePool::load_texture("data/inventory.png");
  _background->set_magfilter(Texture::FT_nearest);
  _background->set_minfilter(Texture::FT_nearest);

  mesh2d.set_budget(1000);

  NodePath node = mesh2d.get_root();
  node.reparent_to(window->get_render_2d());
  node.set_depth_write(false);
  node.set_two_sided(true);
  node.set_light_off(true);
  node.set_texture(_background);*/
  
  //do the main loop, equal to run() in python
  framework.main_loop();
    //close the window framework
  framework.close_framework();
  return (0);
}
