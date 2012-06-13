#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>

#include <panda3d/texturePool.h>
#include <panda3d/directionalLight.h>

PandaFramework       framework;
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject)      globalClock = ClockObject::get_global_clock();

# include "level.hpp"

using namespace std;

#include <panda3d/load_prc_file.h>
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
  void        opAssignString(Data* obj, const std::string& str)   { obj->operator=(str);               }

  bool        opEqualsInt(Data* obj, int i)                       { return (obj->operator==(i));       }
  bool        opEqualsFloat(Data* obj, float f)                   { return (obj->operator==(f));       }
  bool        opEqualsString(Data* obj, const std::string& str)   { return (obj->Value() == str);      }

  std::string getAsString(Data* obj)                              { return (obj->Value());             }
  int         getAsInt(Data* obj)                                 { return (*obj);                     }
  float       getAsFloat(Data* obj)                               { return (*obj);                     }
}

namespace asUtils
{
  InstanceDynamicObject* CharacterAsObject(ObjectCharacter* character) { return (character); }
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
  engine->RegisterGlobalFunction("void LF()", asFUNCTION( GameConsole::ListFunctions ), asCALL_CDECL);
  engine->RegisterGlobalFunction("void PrintScenegraph()", asFUNCTION( GameConsole::PrintScenegraph ), asCALL_CDECL);
  engine->RegisterGlobalFunction("void Write(string)", asFUNCTION(GameConsole::WriteOn), asCALL_CDECL);

  const char* timerClass = "Timer";
  engine->RegisterObjectType     (timerClass, sizeof(Timer), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR);
  engine->RegisterObjectBehaviour(timerClass, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Timer::asConstructor), asCALL_CDECL_OBJLAST);
  engine->RegisterObjectBehaviour(timerClass, asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(Timer::asDestructor),  asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod   (timerClass, "float GetElapsedTime()",       asMETHOD(Timer,GetElapsedTime), asCALL_THISCALL);
  engine->RegisterObjectMethod   (timerClass, "void  Restart()",              asMETHOD(Timer,Restart),        asCALL_THISCALL);
  
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

  const char* itemClass      = "Item";
  const char* inventoryClass = "Inventory";
  engine->RegisterObjectType(itemClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(inventoryClass, 0, asOBJ_REF | asOBJ_NOCOUNT);

  engine->RegisterObjectMethod(inventoryClass, "void  DelObject(Item@)", asMETHOD(Inventory,DelObject), asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "Item@ GetObject(string)",  asMETHODPR(Inventory,GetObject, (const string&), InventoryObject*), asCALL_THISCALL);
  
  engine->RegisterObjectMethod(itemClass, "string GetName()", asMETHOD(InventoryObject,GetName), asCALL_THISCALL);

  const char* dynObjectClass = "DynamicObject";
  const char* charClass      = "Character";
  const char* doorClass      = "Door";
  engine->RegisterObjectType(dynObjectClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(charClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(doorClass,      0, asOBJ_REF | asOBJ_NOCOUNT);

  engine->RegisterObjectMethod(dynObjectClass, "string GetName()",                    asMETHOD(InstanceDynamicObject,GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "string GetName()",                         asMETHOD(ObjectCharacter,GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool HasLineOfSight(DynamicObject@)",      asMETHOD(ObjectCharacter,HasLineOfSight), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void GoTo(int)",                           asMETHODPR(ObjectCharacter,GoTo, (unsigned int), void), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void GoTo(DynamicObject@, int)",           asMETHODPR(ObjectCharacter,GoTo, (InstanceDynamicObject*, int), void), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void GoToRandomWaypoint()",                asMETHOD(ObjectCharacter,GoToRandomWaypoint), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void TruncatePath(int)",                   asMETHOD(ObjectCharacter,TruncatePath), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetNearestWaypoint(DynamicObject@)",  asMETHOD(ObjectCharacter,GetNearestWaypoint), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetFarthestWaypoint(DynamicObject@)", asMETHOD(ObjectCharacter,GetFarthestWaypoint), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetPathDistance(DynamicObject@)",     asMETHODPR(ObjectCharacter,GetPathDistance, (InstanceDynamicObject*), unsigned short), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "float GetDistance(DynamicObject@)",        asMETHOD(ObjectCharacter,GetDistance), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "Inventory@ GetInventory()",                asMETHOD(ObjectCharacter,GetInventory), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "Data GetStatistics()",                     asMETHOD(ObjectCharacter,GetStatistics), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetCurrentWaypoint() const",          asMETHOD(ObjectCharacter,GetOccupiedWaypointAsInt), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool IsMoving() const",                    asMETHOD(ObjectCharacter,IsMoving), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "DynamicObject@ AsObject()",                asFUNCTION(asUtils::CharacterAsObject), asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(charClass, "int  GetActionPoints()",                   asMETHOD(ObjectCharacter,GetActionPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void SetActionPoints(int)",                asMETHOD(ObjectCharacter,SetActionPoints), asCALL_THISCALL);

  engine->RegisterObjectMethod(doorClass, "void Unlock()",       asMETHOD(ObjectDoor,Unlock), asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool IsLocked()",     asMETHOD(ObjectDoor,IsLocked), asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool IsOpen()",       asMETHOD(ObjectDoor,IsOpen), asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "string GetKeyName()", asMETHOD(ObjectDoor,GetKeyName), asCALL_THISCALL);

  const char* worldClass = "World";
  engine->RegisterObjectType(worldClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(worldClass, "void SetWaypointsVisible(bool) const",      asMETHOD(World,SetWaypointsVisible),      asCALL_THISCALL);
  engine->RegisterObjectMethod(worldClass, "void SetMapObjectsVisible(bool) const",     asMETHOD(World,SetMapObjectsVisible),     asCALL_THISCALL);
  engine->RegisterObjectMethod(worldClass, "void SetDynamicObjectsVisible(bool) const", asMETHOD(World,SetDynamicObjectsVisible), asCALL_THISCALL);  
  
  const char* levelClass = "Level";
  engine->RegisterObjectType(levelClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(levelClass, "Data           GetDataEngine()",                       asMETHOD(Level,GetDataEngine),       asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "const World@   GetWorld() const",                      asMETHOD(Level,GetWorld),            asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Character@     GetCharacter(string)",                  asMETHOD(Level,GetCharacter),        asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Character@     GetPlayer()",                           asMETHOD(Level,GetPlayer),           asCALL_THISCALL);  
  engine->RegisterObjectMethod(levelClass, "DynamicObject@ GetObject(string)",                     asMETHOD(Level,GetObject),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUse(Character@, DynamicObject@)", asMETHOD(Level,ActionUse),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUseObjectOn(Character@, DynamicObject@, Item@)", asMETHOD(Level,ActionUseObjectOn), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionDropObject(Character@, Item@)",   asMETHOD(Level,ActionDropObject),    asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StartFight(Character@)",                asMETHOD(Level,StartFight),          asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StopFight()",                           asMETHOD(Level,StopFight),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           NextTurn()",                            asMETHOD(Level,NextTurn),            asCALL_THISCALL);

  engine->RegisterGlobalProperty("Level@ level", &(Level::CurrentLevel));
}

int main(int argc, char *argv[])
{
  ConfigPage* config = load_prc_file("config.prc");

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

  framework.main_loop();
  framework.close_framework();
  
  unload_prc_file(config);
  
  Script::Engine::Finalize();
  return (0);
}
