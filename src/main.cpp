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

#include "serializer.hpp"

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
  InstanceDynamicObject* CharacterAsObject(ObjectCharacter* character)    { return (character); }
  ObjectCharacter*       DynObjAsCharacter(InstanceDynamicObject* object) { return (object->Get<ObjectCharacter>()); }
  Data                   ItemAsData(InventoryObject* item)                { return (*item); }

  void SerializeInt   (Utils::Packet* packet, int i)                      { (*packet) << i;     }
  void SerializeString(Utils::Packet* packet, const std::string& str)     { (*packet) << str;   }
  void SerializeStrArr(Utils::Packet* packet, std::list<string> array)    { (*packet) << array; }
  void UnserializeInt   (Utils::Packet* packet, int& i)                   { (*packet) >> i;     }
  void UnserializeString(Utils::Packet* packet, std::string& str)         { (*packet) >> str;   }
  void UnserializeStrArr(Utils::Packet* packet, std::list<string>& array) { (*packet) >> array; }
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
  engine->RegisterGlobalFunction("void Write(const string &in)", asFUNCTION(GameConsole::WriteOn), asCALL_CDECL);

  Script::StdList<string>::Register(engine, "StringList", "string");  

  const char* packetClass = "Serializer";
  engine->RegisterObjectType(packetClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(packetClass, "void Serialize(int)",               asFUNCTION(asUtils::SerializeInt),      asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(packetClass, "void Serialize(string)",            asFUNCTION(asUtils::SerializeString),   asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(packetClass, "void Serialize(StringList)",        asFUNCTION(asUtils::SerializeStrArr),   asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(packetClass, "int  Unserialize(int& out)",        asFUNCTION(asUtils::UnserializeInt),    asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(packetClass, "void Unserialize(string& out)",     asFUNCTION(asUtils::UnserializeString), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(packetClass, "void Unserialize(StringList& out)", asFUNCTION(asUtils::UnserializeStrArr), asCALL_CDECL_OBJFIRST);
  
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
  engine->RegisterObjectMethod   (dataClass, "const Data &opAssign(const Data &in)", asMETHODPR(Data,operator=,  (const Data&), const Data&), asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "Data opIndex(const string &in)",       asMETHODPR(Data,operator[], (const std::string&), Data), asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "Data opIndex(int)",                    asMETHODPR(Data,operator[], (unsigned int),       Data), asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "void opAssign(int)",             asFUNCTION(asData::opAssignInt),    asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opAssign(float)",           asFUNCTION(asData::opAssignFloat),  asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opAssign(string)",          asFUNCTION(asData::opAssignString), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opEquals(int)",             asFUNCTION(asData::opEqualsInt),    asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opEquals(float)",           asFUNCTION(asData::opEqualsFloat),  asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void opEquals(string)",          asFUNCTION(asData::opEqualsString), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "int    Count()",                 asMETHOD(Data,Count),               asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "bool   Nil()",                   asMETHOD(Data,Nil),                 asCALL_THISCALL);
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
  engine->RegisterObjectMethod(itemClass, "Data   AsData()",  asFUNCTION(asUtils::ItemAsData), asCALL_CDECL_OBJFIRST);

  const char* dynObjectClass = "DynamicObject";
  const char* charClass      = "Character";
  const char* doorClass      = "Door";
  engine->RegisterObjectType(dynObjectClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(charClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(doorClass,      0, asOBJ_REF | asOBJ_NOCOUNT);

  Script::StdList<ObjectCharacter*>::Register(engine, "CharacterList", "Character@");
  engine->RegisterObjectMethod(charClass, "CharacterList GetNearbyAllies()",  asMETHOD(ObjectCharacter,GetNearbyAllies),  asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "CharacterList GetNearbyEnemies()", asMETHOD(ObjectCharacter,GetNearbyEnemies), asCALL_THISCALL);

  engine->RegisterObjectMethod(dynObjectClass, "string GetName()",                    asMETHOD(InstanceDynamicObject,GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod(dynObjectClass, "Character@ AsCharacter()",            asFUNCTION(asUtils::DynObjAsCharacter), asCALL_CDECL_OBJLAST);
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
  //MSVC2010 strangeness for this function: Base/Derived thingymajig complications
  engine->RegisterObjectMethod(charClass, "int  GetCurrentWaypoint() const",          asMETHODPR(WaypointModifier,GetOccupiedWaypointAsInt, (void) const, int), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool IsMoving() const",                    asMETHOD(ObjectCharacter,IsMoving), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool IsAlive() const",                     asMETHOD(ObjectCharacter,IsAlive),  asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "DynamicObject@ AsObject()",                asFUNCTION(asUtils::CharacterAsObject), asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(charClass, "int  GetActionPoints()",                   asMETHOD(ObjectCharacter,GetActionPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void SetActionPoints(int)",                asMETHOD(ObjectCharacter,SetActionPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetHitPoints()",                      asMETHOD(ObjectCharacter,GetHitPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void SetHitPoints(int)",                   asMETHOD(ObjectCharacter,SetHitPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetArmorClass()",                     asMETHOD(ObjectCharacter,GetArmorClass), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "Item@ GetEquipedItem(int)",                asMETHOD(ObjectCharacter,GetEquipedItem), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void LookAt(Character@)",                  asMETHODPR(ObjectCharacter,LookAt,(InstanceDynamicObject*),void),         asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void LookAt(DynamicObject@)",              asMETHODPR(ObjectCharacter,LookAt,(InstanceDynamicObject*),void),         asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void PushBuff(Data, Character@)",          asMETHOD(ObjectCharacter,PushBuff), asCALL_THISCALL);

  engine->RegisterObjectMethod(doorClass, "void Unlock()",       asMETHOD(ObjectDoor,Unlock), asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool IsLocked()",     asMETHOD(ObjectDoor,IsLocked), asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool IsOpen()",       asMETHOD(ObjectDoor,IsOpen), asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "string GetKeyName()", asMETHOD(ObjectDoor,GetKeyName), asCALL_THISCALL);

  const char* worldClass = "World";
  engine->RegisterObjectType(worldClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(worldClass, "void SetWaypointsVisible(bool) const",      asMETHOD(World,SetWaypointsVisible),      asCALL_THISCALL);
  engine->RegisterObjectMethod(worldClass, "void SetMapObjectsVisible(bool) const",     asMETHOD(World,SetMapObjectsVisible),     asCALL_THISCALL);
  engine->RegisterObjectMethod(worldClass, "void SetDynamicObjectsVisible(bool) const", asMETHOD(World,SetDynamicObjectsVisible), asCALL_THISCALL);  
  
  const char* cameraClass = "Camera";
  engine->RegisterObjectType(cameraClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(cameraClass, "void CenterOn(DynamicObject@)", asMETHOD(SceneCamera,CenterOnObject), asCALL_THISCALL);
  engine->RegisterObjectMethod(cameraClass, "void Follow(DynamicObject@)",   asMETHOD(SceneCamera,FollowObject),   asCALL_THISCALL);
  engine->RegisterObjectMethod(cameraClass, "void StopFollowing()",          asMETHOD(SceneCamera,StopFollowingNodePath), asCALL_THISCALL);
  
  const char* levelClass = "Level";
  engine->RegisterObjectType(levelClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(levelClass, "Data           GetDataEngine()",                       asMETHOD(Level,GetDataEngine),       asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "const World@   GetWorld() const",                      asMETHOD(Level,GetWorld),            asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Camera@        GetCamera()",                           asMETHOD(Level,GetCamera),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Character@     GetCharacter(string)",                  asMETHOD(Level,GetCharacter),        asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Character@     GetPlayer()",                           asMETHOD(Level,GetPlayer),           asCALL_THISCALL);  
  engine->RegisterObjectMethod(levelClass, "DynamicObject@ GetObject(string)",                     asMETHOD(Level,GetObject),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUseWeaponOn(Character@, Character@, Item@, int)",     asMETHOD(Level,ActionUseWeaponOn), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUseObjectOn(Character@, DynamicObject@, Item@, int)", asMETHOD(Level,ActionUseObjectOn), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUse(Character@, DynamicObject@)",                     asMETHOD(Level,ActionUse),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionDropObject(Character@, Item@)",   asMETHOD(Level,ActionDropObject),    asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           PlayerLoot(Inventory@)",                asMETHOD(Level,PlayerLoot),          asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StartFight(Character@)",                asMETHOD(Level,StartFight),          asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StopFight()",                           asMETHOD(Level,StopFight),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           NextTurn()",                            asMETHOD(Level,NextTurn),            asCALL_THISCALL);

  engine->RegisterGlobalProperty("Level@ level", &(Level::CurrentLevel));
}

#include <fstream>
class WorldMap : public UiBase
{
public:
  WorldMap(WindowFramework* window, GameUi* gameUi) : UiBase(window, gameUi->GetContext()), _gameUi(*gameUi)
  {
    DataTree* mapTree = DataTree::Factory::JSON("saves/map.json");
    MapTileGenerator(mapTree);
    delete mapTree;

    if (_root)
    {
      //
      // Adds the known cities to the CityList.
      //
      DataTree* cityTree = DataTree::Factory::JSON("saves/cities.json");
      {
	Data      cities(cityTree);

	std::for_each(cities.begin(), cities.end(), [this](Data city)
	{
	  Rocket::Core::Element* elem = _root->GetElementById("city-list");
	  Rocket::Core::String   innerRml;
	  std::stringstream      rml;

	  rml << "<div class='city-entry'>";
	  rml << "<div class='city-button'><button>Go</button></div>";
	  rml << "<div class='city-name'>" << city.Value() << "</div>";
	  rml << "</div>";
	  elem->GetInnerRML(innerRml);
	  innerRml = innerRml + Rocket::Core::String(rml.str().c_str());
	  elem->SetInnerRML(innerRml);
	});
      }
      delete cityTree;
      
      _root->GetElementById("button-inventory")->AddEventListener("click", &ButtonInventory);
      _root->GetElementById("button-character")->AddEventListener("click", &ButtonCharacter);
      _root->GetElementById("button-menu")->AddEventListener("click", &ButtonMenu);
      ButtonInventory.EventReceived.Connect(_gameUi, &GameUi::OpenInventory);
      ButtonCharacter.EventReceived.Connect(_gameUi, &GameUi::OpenPers);
      ButtonMenu.EventReceived.Connect(_gameUi, &GameUi::OpenMenu);
      
      MapClickedEvent.EventReceived.Connect(*this, &WorldMap::MapClicked);
    }
  }
  
  void MapTileGenerator(Data map)
  {
    std::stringstream rml, rcss;
    Data              tiles   = map["tiles"];
    int               size_x  = map["size_x"];
    int               size_y  = map["size_y"];
    int               tsize_x = map["tile_size_x"];
    int               tsize_y = map["tile_size_y"];
    int               pos_x   = 0;
    int               pos_y   = 0;

    _tsize_x = tsize_x;
    _tsize_y = tsize_y;
    
    //
    // Generate RCSS and RML for the Tilemap
    //
    std::for_each(tiles.begin(), tiles.end(), [this, &rml, &rcss, &pos_x, &pos_y, size_x, size_y, tsize_x, tsize_y](Data tile)
    {
      rcss << "#tile" << pos_x << "-" << pos_y << "\n";
      rcss << "{\n" << "  top: "  << (pos_y * tsize_y) << "px;\n" << "  left: " << (pos_x * tsize_x) << "px;\n";
      rcss << "  height: " << tsize_y << "px; width: " << tsize_x << "px;\n";
      rcss << "  background-decorator: image;\n";
      rcss << "  background-image: worldmap.png " << (pos_x * tsize_x) << "px " << (pos_y * tsize_y) << "px ";
      rcss << (pos_x * tsize_x + tsize_x) << "px " << (pos_y * tsize_y + tsize_y) << "px;\n";
      rcss << "}\n\n";

      rml  << "<div id='tile" << pos_x << "-" << pos_y << "' class='tile' style='position: absolute;";
      rml  << "top: "  << (pos_y * tsize_y) << "px; ";
      rml  << "left: " << (pos_x * tsize_x) << "px; ";
      rml  << "height: " << tsize_y << "px; width: " << tsize_x << "px; ";
      rml  << "background-decorator: image; ";
      rml  << "background-image: worldmap.png " << (pos_x * tsize_x) << "px " << (pos_y * tsize_y) << "px ";
      rml  << (pos_x * tsize_x + tsize_x) << "px " << (pos_y * tsize_y + tsize_y) << "px;";
      rml  << "' data-pos-x='" << pos_x << "' data-pos-y='" << pos_y << "'>test</div>\n";

      if (++pos_x >= size_x)
      {
        ++pos_y;
        pos_x = 0;
      }
    });

    //
    // Load the worldmap rml template, replace the #{RML} and #{RCSS} bits with generated RML/RCSS,
    // create a temporary RML file with the result.
    //
    std::ifstream file("data/worldmap.rml");

    if (file.is_open())
    {
      std::string fileRml;
      long        begin, end;
      long        size;
      char*       raw;

      begin     = file.tellg();
      file.seekg (0, ios::end);
      end       = file.tellg();
      file.seekg(0, ios::beg);
      size      = end - begin;
      raw       = new char[size + 1];
      raw[size] = 0;
      file.read(raw, size);
      file.close();
      fileRml = raw;
      delete raw;

      size_t firstReplaceIt = fileRml.find("#{RCSS}");      
      fileRml.replace(firstReplaceIt, strlen("#{RCSS}"), rcss.str());
      size_t secReplaceIt   = fileRml.find("#{RML}");
      fileRml.replace(secReplaceIt, strlen("#{RML}"), rml.str());
      
      std::ofstream ofile("data/tmp-worldmap.rml");
      
      if (ofile.is_open())
      {
	ofile.write(fileRml.c_str(), fileRml.size());
	ofile.close();
      }

      //
      // Load the temporary file and link every click event from every tile
      // to the RocketListener MapClickedEvent.
      //
      _root = _context->LoadDocument("data/tmp-worldmap.rml");
      
      if (_root)
      {
	Rocket::Core::Element* mapElem = _root->GetElementById("pworldmap");
	mapElem->SetInnerRML(rml.str().c_str());
	for (pos_x = 0, pos_y = 0 ; pos_x < size_x && pos_y < size_y ;)
	{
	  Rocket::Core::Element* tileElem;
	  std::stringstream      idElem;
	  
	  idElem << "tile" << pos_x << "-" << pos_y;
	  tileElem = _root->GetElementById(idElem.str().c_str());
	  if (tileElem)
	    tileElem->AddEventListener("click", &MapClickedEvent);
	  if (++pos_x >= size_x)
	  {
	    ++pos_y;
	    pos_x = 0;
	  }
	}
      }
    }
  }
  
  void MapClicked(Rocket::Core::Event& event)
  {
    Rocket::Core::Element* tile = event.GetCurrentElement();
    
    if (tile)
    {
      Rocket::Core::Variant* v_pos_x = tile->GetAttribute("data-pos-x");
      Rocket::Core::Variant* v_pos_y = tile->GetAttribute("data-pos-y");
      int                    pos_x   = v_pos_x->Get<int>();
      int                    pos_y   = v_pos_y->Get<int>();
      int                    click_x = 0;
      int                    click_y = 0;

      click_x  = event.GetParameter("mouse_x", click_x);
      click_y  = event.GetParameter("mouse_y", click_y);
      click_x += (pos_x * _tsize_x);
      click_y += (pos_y * _tsize_y);
    }
  }
  
  void UpdatePartyCursor(float elapsedTime)
  {
    
  }
  
  void Run(void)
  {
    float elapsedTime = _timer.GetElapsedTime();
    
    if (_current_pos_x != _goal_x || _current_pos_y != _goal_y)
      UpdatePartyCursor(elapsedTime);
    _timer.Restart();
  }

  Observatory::Signal<void (std::string)> GoToPlace;

private:
  RocketListener MapClickedEvent, ButtonInventory, ButtonCharacter, ButtonMenu;
  GameUi&        _gameUi;
  Timer          _timer;

  int            _tsize_x, _tsize_y;
  
  int            _current_pos_x, _current_pos_y;
  int            _goal_x, _goal_y;
};

class LevelTask;

bool   SaveLevel(Level* level, const std::string& name);
Level* LoadLevel(WindowFramework* window, GameUi& gameUi, LevelTask& asyncTask, const std::string& name, bool isSaveFile = false);

class LevelTask : public AsyncTask
{
public:
  LevelTask(WindowFramework* window, PT(RocketRegion) rocket) : _gameUi(window, rocket)
  {
    _window   = window;
    _level    = 0;
    _savePath = "saves";
    _worldMap = new WorldMap(window, &_gameUi);
    _worldMap->GoToPlace.Connect(*this, &LevelTask::MapOpenLevel);
    AsyncTaskManager::get_global_ptr()->add(this);
  }
  
  ~LevelTask()
  {
    std::cout << "Deleting LevelTask" << std::endl;
    if (_level)    delete _level;
    if (_worldMap) delete _worldMap;
    std::cout << "Done" << std::endl;
    AsyncTaskManager::get_global_ptr()->remove(this);
  }
  
  void       MapOpenLevel(std::string name)
  {
    _worldMap->Hide();
    if (!(OpenLevel(_savePath, name)))
      _worldMap->Show();
  }

  void       SetLevel(Level* level)
  {
    _level = level;
  }

  DoneStatus do_task()
  {
    if (_level)
    {
      if (_level->do_task() == AsyncTask::DoneStatus::DS_done)
      {
	const string nextZone  = _level->GetNextZone();
	const string exitPoint = _level->GetExitZone();

	ExitLevel(_savePath);
	/*if (nextZone != "")
	{
	  OpenLevel(_savePath, nextZone);
	  if (_level)
	    _level->SetEntryZone(exitPoint);
	}*/
      }
      if (!_level)
      {
	_worldMap->Show();
      }
    }
    else
      _worldMap->Run();
    return (AsyncTask::DoneStatus::DS_cont);
  }
  
  bool SaveGame(const std::string& savepath)
  {
    bool success = true;

    if (_level)
    {
      _dataEngine["system"]["current-level"] = _levelName;
      success = success && SaveLevel(_level, savepath + "/" + _levelName + ".blob");
    }
    else
      _dataEngine["system"]["current-level"] = 0;
    _dataEngine.Save(savepath + "/dataengine.json");
    return (success);
  }

  bool LoadGame(const std::string& savepath)
  {
    Data currentLevel;

    _dataEngine.Load(savepath + "/dataengine.json");
    currentLevel = _dataEngine["system"]["current-level"];
    if (!(currentLevel.Nil()) && currentLevel.Value() != "0")
    {
      LoadLevel(_window, _gameUi, *this, savepath + "/" + currentLevel.Value() + ".blob", true);
    }
  }
  
  bool OpenLevel(const std::string& savepath, const std::string& level)
  {
    std::ifstream fileTest;

    fileTest.open((savepath + "/" + level + ".blob").c_str());
    if (fileTest.is_open())
    {
      fileTest.close();
      _level = LoadLevel(_window, _gameUi, *this, savepath + "/" + level + ".blob", true);
    }
    else
      _level = LoadLevel(_window, _gameUi, *this, "maps/" + level + ".blob", false);
    if (_level)
    {
      // WARNING This is only temporary
      _dataEngine.Load(savepath + "/dataengine.json");

      _levelName = level;
      _level->SetDataEngine(&_dataEngine);
      SetLevel(_level);
    }
    else
      cerr << "¡¡ Can't open level !!" << endl;
    return (_level != 0);
  }

  void ExitLevel(const std::string& savepath)
  {
    /*if (!(SaveGame(savepath)))
    {
      cerr << "¡¡ Couldn't save level state on ExitLevel !!" << endl;
    }*/
    // TODO Find why level destruction makes panda3d crash
    delete _level;
    _level = 0;
    cout << "Exited Level" << endl;
  }

  bool CopySave(const std::string& savepath, const std::string& slotPath)
  {
    // Copy the savepath directory to the slotpath directory
  }

  // LEVEL EVENTS
  void LevelExitZone(const std::string& toLevel)
  {
    ExitLevel(_savePath);
    if (toLevel != "")
    {
      if (!(OpenLevel(_savePath, toLevel)))
      {
	// Display Level Opening Error
      }
    }
    if (!_level)
    {
      // Display Map Interface
    }
  }

  void UiSaveGame(const std::string& slotPath)
  {
    SaveGame(_savePath);
    CopySave(_savePath, slotPath);
  }

  void UiLoadGame(const std::string& slotPath)
  {
    CopySave(_savePath, slotPath);
    LoadGame(_savePath);
  }

private:
  WindowFramework* _window;
  GameUi           _gameUi;
  DataEngine       _dataEngine;
  
  WorldMap*        _worldMap;
  
  std::string      _levelName;
  Level*           _level;

  std::string      _savePath;
};

bool   SaveLevel(Level* level, const std::string& name)
{
  Utils::Packet packet;
  std::ofstream file;

  level->UnprocessAllCollisions();
  level->GetWorld()->Serialize(packet);
  level->Save(packet);
  level->ProcessAllCollisions();
  file.open(name.c_str(), std::ios::binary);
  if (file.is_open())
    file.write(packet.raw(), packet.size());
  else
  {
    std::cerr << "¡¡ Failed to open file '" << name << "', save failed !!" << std::endl;
    return (false);
  }
  return (true);
}

Level* LoadLevel(WindowFramework* window, GameUi& gameUi, LevelTask& asyncTask, const std::string& name, bool isSaveFile)
{
  Level*        level = 0;
  std::ifstream file;
  
  file.open(name.c_str(), std::ios::binary);
  if (file.is_open())
  {
    Utils::Packet packet(file);

    try
    {
      level = new Level(window, gameUi, asyncTask, packet);
      if (isSaveFile)
	level->Load(packet);
      file.close();
    }
    catch (const char* error)
    {
      std::cerr << "¡¡ Failed to load file !! (" << error << ")" << std::endl;
      level = 0;
    }
  }
  else
    std::cerr << "¡¡ File not found !!" << std::endl;
  return (level);
}

static string GetPathFromString(string str)
{  
  int lastSlash = str.size();

  for (int i = 0 ; i < str.size() ; ++i)
  {
    if (str[i] == '/' || str[i] == '\\')
      lastSlash = i;
  }

  if (lastSlash != str.size())
    return (str.substr(0, lastSlash));
  return ("");
}

int main(int argc, char *argv[])
{
  std::cout << "Executing path is " << GetPathFromString(argv[0]) << std::endl;
  
  //int chdirsuccess = chdir(GetPathFromString(argv[0]).c_str());
  
  ConfigPage* config = load_prc_file("config.prc");

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

  Script::Engine::Initialize();

  AngelScriptInitialize();

  GeneralUi      generalUi(window);
  LevelTask      levelTask(window, generalUi.GetRocketRegion());
  std::string    filename = "test";
  std::string    fullpath = "maps/" + filename + ".blob";  
  Level*         level;
  
  //level = LoadLevel(window, levelTask,  fullpath);  
  levelTask.OpenLevel("saves", "test");
//   level = LoadLevel(window, levelTask, "saves/slot01.blob", true);
//   levelTask.SetLevel(level);

  framework.main_loop();
  framework.close_framework();
  
  unload_prc_file(config);
  
  Script::Engine::Finalize();
  return (0);
}
