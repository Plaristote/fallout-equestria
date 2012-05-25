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

# include "scriptengine.hpp"

void AngelCout(const std::string& str)
{
  std::cout << "[AngelScript][Output] " << str << std::endl;
}

std::string scriptTmpString;

namespace asData
{
  void Constructor(void* memory) { new(memory) Data();       }
  void Destructor (void* memory) { ((Data*)memory)->~Data(); }

  void        opAssignInt(Data* obj, int i)                       { obj->operator=(i);                 }
  void        opAssignFloat(Data* obj, float f)                   { obj->operator=(f);                 }
  void        opAssignString(Data* obj, const std::string& str)   { std::cout << "OP ASSIGN STR" << std::endl; obj->operator=(str); std::cout << "OP ENDED" << std::endl;              }

  bool        opEqualsInt(Data* obj, int i)                       { return (obj->operator==(i));       }
  bool        opEqualsFloat(Data* obj, float f)                   { return (obj->operator==(f));       }
  bool        opEqualsString(Data* obj, const std::string& str)   { return (obj->Value() == str);      }

  std::string getAsString(Data* obj)                              { return (obj->Value());             }
  int         getAsInt(Data* obj)                                 { return (*obj);                     }
  float       getAsFloat(Data* obj)                               { return (*obj);                     }
}

struct asConsoleOutput
{
  void OutputError(std::string str) { std::cout << str << std::endl; }
};
asConsoleOutput asConsole;

void AngelScriptInitialize(void)
{
  asIScriptEngine* engine = Script::Engine::Get();

  Script::Engine::ScriptError.Connect(asConsole, &asConsoleOutput::OutputError);

  engine->RegisterGlobalFunction("void Cout(string)", asFUNCTION(AngelCout), asCALL_CDECL);

  const char* dataClass = "Data";
  engine->RegisterObjectType     (dataClass, sizeof(Data), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR | asOBJ_APP_CLASS_ASSIGNMENT);
  engine->RegisterObjectBehaviour(dataClass, asBEHAVE_CONSTRUCT, "void f()",   asFUNCTION(asData::Constructor),    asCALL_CDECL_OBJLAST);
  engine->RegisterObjectBehaviour(dataClass, asBEHAVE_DESTRUCT,  "void f()",   asFUNCTION(asData::Destructor),     asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod   (dataClass, "const Data &opAssign(const Data &in)", asMETHODPR(Data,operator=, (const Data&), const Data&), asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "Data opIndex(const string &in)",      asMETHODPR(Data,operator[], (const std::string&), Data), asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "void opAssign(int)",             asFUNCTION(asData::opAssignInt),    asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opAssign(float)",           asFUNCTION(asData::opAssignFloat),  asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opAssign(string)",          asFUNCTION(asData::opAssignString), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opEquals(int)",             asFUNCTION(asData::opEqualsInt),    asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opEquals(float)",           asFUNCTION(asData::opEqualsFloat),  asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opEquals(string)",          asFUNCTION(asData::opEqualsString), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "bool Nil()",                     asMETHOD(Data,Nil),                 asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "string Key()",                   asMETHOD(Data,Key),                 asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "string AsString()",              asMETHOD(Data,Value),               asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "int    AsInt()",                 asFUNCTION(asData::getAsInt),       asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "float  AsFloat()",               asFUNCTION(asData::getAsFloat),     asCALL_CDECL_OBJFIRST);

  const char* posClass  = "MapPosition";
  engine->RegisterObjectType(posClass,   sizeof(MapElement::Position), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS);
  engine->RegisterObjectMethod(posClass, "int get_x()",     asMETHOD(MapElement::Position,get_x), asCALL_THISCALL);
  engine->RegisterObjectMethod(posClass, "int get_y()",     asMETHOD(MapElement::Position,get_y), asCALL_THISCALL);
  engine->RegisterObjectMethod(posClass, "void set_x(int)", asMETHOD(MapElement::Position,set_x), asCALL_THISCALL);
  engine->RegisterObjectMethod(posClass, "void set_y(int)", asMETHOD(MapElement::Position,set_y), asCALL_THISCALL);

  const char* charClass = "Character";
  engine->RegisterObjectType(charClass,   0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(charClass, "uint16 GoTo(int, int)",           asMETHOD(Character,GoTo),           asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool HasLineOfSight(Character@)", asMETHOD(Character,HasLineOfSight), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool IsMoving()",                 asMETHOD(Character,IsMoving),       asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "string GetName()",                asMETHOD(Character,GetName),        asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "MapPosition GetPosition()",       asMETHOD(Character,GetPosition),    asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void ShowCollision(bool)",        asMETHOD(Character,DebugShowCollisionSphere), asCALL_THISCALL);

  const char* levelClass = "Level";
  engine->RegisterObjectType(levelClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(levelClass, "Character@ FindCharacterByName(const string &in)", asMETHOD(Level,FindCharacterByName), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Data       GetDataEngine()",                       asMETHOD(Level,GetDataEngine),       asCALL_THISCALL);

  engine->RegisterGlobalProperty("Level@ level", &(Level::CurrentLevel));

  engine->RegisterGlobalProperty("string tmpString", &(scriptTmpString));
}

int main(int argc, char *argv[])
{
  //open a new window framework
  framework.open_framework(argc, argv);
  //set the window title to My Panda3D Window
  framework.set_window_title("Fallout Equestria");
  //open the window
  WindowFramework *window = framework.open_window();

  /*WindowProperties wp;
  window->get_graphics_window()->request_properties(wp);
  wp.set_fullscreen(true);
  window->get_graphics_window()->set_properties_now(wp);*/
  //wp.set_size();
  
  window->enable_keyboard();
  window->get_render().set_shader_auto();

  if (!(PStatClient::connect("localhost", 5185)))
    cout << "Can't connect to PStat client" << endl;

  Script::Engine::Initialize();

  AngelScriptInitialize();

  Level level(window, "test");
  /*Widget widget(window, "inventory");

  widget.SetBackground("data/inventory.png");
  widget.SetPosition(LVector2f(50.f, 100.f));

  Widget widget2(&widget, "inventory-items");

  widget2.SetBackground("data/inventory-items.png");
  widget2.SetPosition(LVector2f(10.f, 10.f));*/

//   MeshDrawer2D mesh2d;
// 
//   PT(Texture) _background = TexturePool::load_texture("data/inventory.png");
//   _background->set_magfilter(Texture::FT_nearest);
//   _background->set_minfilter(Texture::FT_nearest);
// 
//   mesh2d.set_budget(1000);
// 
//   NodePath node = mesh2d.get_root();
//   node.reparent_to(window->get_render_2d());
//   node.set_depth_write(false);
//   node.set_two_sided(true);
//   node.set_light_off(true);
//   node.set_texture(_background);
//   node.set_scale(10, 10, 10);

  //do the main loop, equal to run() in python
  framework.main_loop();
    //close the window framework
  framework.close_framework();
  Script::Engine::Finalize();
  return (0);
}
