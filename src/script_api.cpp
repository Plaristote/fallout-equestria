#include "scriptengine.hpp"
#include "dataengine.hpp"

#include "gametask.hpp"
#include "quest_manager.hpp"

#include "level/level.hpp"
#include "level/objects/door.hpp"
#include "level/objects/locker.hpp"
#include "level/objects/shelf.hpp"

#include "worldmap/worldmap.hpp"

asIScriptContext* as_current_context;
asIScriptModule*  as_current_module;

/*
 * Script Interface for Rocket Listeners
 */
class RocketAsListener : public Rocket::Core::EventListener
{
public:
  RocketAsListener(DataEngine& de, Rocket::Core::Element* elem, const std::string& event, const std::string& func_name) : _element(elem), _event(event), _de(de)
  {
    cout << "RocketAsListener::RocketAsListener" << endl;
    if (_element)
      _element->AddEventListener(_event.c_str(), this);
    else
      cout << "RocketAsListener constructed with null RmlElement" << endl;
    _callback = "void " + func_name + "(Data, RmlElement@, string)";
    _context  = as_current_context;
    _module   = as_current_module;
  }

  ~RocketAsListener()
  {
    if (_element)
      _element->RemoveEventListener(_event.c_str(), this);
  }

  void ProcessEvent(Rocket::Core::Event& event)
  {
    cout << "RocketAsListener::ProcessEvent" << endl;
    if (_module && _element)
    {
      asIScriptFunction* callback = _module->GetFunctionByDecl(_callback.c_str());

      // WARNING About the present bug
      // GetFunctionByDecl doesn't find the function declaration for some unknown reason.
      // The function declaration is valid and should be present in the module, unless
      // we're using the wrong module. Which is unlikely, but is the last explanation.
      // TODO isn't this bug fixed ? investigate

      if (_context && callback)
      {
        _context->Prepare(callback);
        _context->SetArgObject(0, &_de);
        _context->SetArgObject(1, event.GetCurrentElement());
        _context->SetArgObject(2, &_event);
        _context->Execute();
      }
    }
  }

private:
  Rocket::Core::Element* _element;
  std::string            _event;
  DataEngine&            _de;
  asIScriptContext*      _context;
  asIScriptModule*       _module;
  std::string            _callback;
};

class RocketAsManager
{
  struct ListenerPair
  {
    ListenerPair(Rocket::Core::Element* f, RocketAsListener* s) : element(f), listener(s) {}
    Rocket::Core::Element* element;
    RocketAsListener*      listener;
    bool operator==(Rocket::Core::Element* comp) { return (element == comp); }
  };
  typedef std::list<ListenerPair> ListenerPairs;
public:
  static RocketAsManager self;
  
  void          AddEventListener(Rocket::Core::Element* e, RocketAsListener* l)
  {
    _event_listeners.push_back(ListenerPair(e, l));
  }
  
  void          DelEventListener(Rocket::Core::Element* e)
  {
    bool keep_searching;

    do
    {
      auto it = std::find(_event_listeners.begin(), _event_listeners.end(), e);

      keep_searching = it != _event_listeners.end();
      if (keep_searching)
      {
        cout << "Deleted an event listener" << endl;
        delete it->listener;
        _event_listeners.erase(it);
      }
    } while (keep_searching);
  }
  
private:
  ListenerPairs _event_listeners;
};

RocketAsManager RocketAsManager::self;

void AngelCout(const std::string& str)
{
  std::cout << "[AngelScript][Output] " << str << std::endl;
}

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

  std::string getAsString(Data* obj)                              { return (obj ? obj->Value() : "");  }
  int         getAsInt(Data* obj)                                 { return (obj ? (int)*obj   : 0);    }
  float       getAsFloat(Data* obj)                               { return (obj ? (float)*obj : 0.f);  }
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

  std::string            RocketGetId(Rocket::Core::Element* self)                            { return (self->GetId().CString());          }
  Rocket::Core::Element* RocketGetElementById(Rocket::Core::Element* self, const string& id) { return (self->GetElementById(id.c_str())); }
  void                   RocketSetInnerRML(Rocket::Core::Element* self, const string& rml)   { return (self->SetInnerRML(rml.c_str()));   }
  void                   RocketClearListeners(Rocket::Core::Element* self) { cout << "RocketClearListeners" << endl; RocketAsManager::self.DelEventListener(self); }
  void                   RocketSetListener(Rocket::Core::Element* self, const string& event, const string& callback)
  {
    DataEngine&       data_engine = WorldMap::CurrentWorldMap->GetDataEngine();
    RocketAsListener* listener    = new RocketAsListener(data_engine, self, event, callback);

    RocketAsManager::self.AddEventListener(self, listener);
  }
}

struct asConsoleOutput
{
  void OutputError(std::string str) { std::cout << "[asConsoleOutput]" << str << std::endl; }
};

asConsoleOutput asConsole;

#define OBJ_REF_REGISTER_METHOD(klass,retval,method,params) \
  engine->RegisterObjectMethod(#klass, std::string(std::string(#retval) + ' ' + std::string(#method) + std::string(#params)).c_str(), asMETHOD(klass,method), asCALL_THISCALL)

void AngelScriptInitialize(void)
{
  asIScriptEngine* engine = Script::Engine::Get();

  Script::Engine::ScriptError.Connect(asConsole, &asConsoleOutput::OutputError);
  
  engine->RegisterGlobalFunction("void Cout(string)", asFUNCTION(AngelCout), asCALL_CDECL);
  engine->RegisterGlobalFunction("void LF()", asFUNCTION( GameConsole::ListFunctions ), asCALL_CDECL);
  engine->RegisterGlobalFunction("void PrintScenegraph()", asFUNCTION( GameConsole::PrintScenegraph ), asCALL_CDECL);
  engine->RegisterGlobalFunction("void Write(const string &in)", asFUNCTION(GameConsole::WriteOn), asCALL_CDECL);
  engine->RegisterGlobalFunction("void SetLanguage(const string& in)", asFUNCTION(i18n::Load), asCALL_CDECL);
  engine->RegisterGlobalFunction("int  Random()", asFUNCTION(rand), asCALL_CDECL);

  Script::StdList<string>::Register(engine, "StringList", "string");

  const char* musicmanagerClass = "Music";
  engine->RegisterObjectType(musicmanagerClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(musicmanagerClass, "void Play(string)",         asMETHODPR(MusicManager,Play, (const std::string&), void), asCALL_THISCALL);
  engine->RegisterObjectMethod(musicmanagerClass, "void Play(string, string)", asMETHODPR(MusicManager,Play, (const std::string&, const std::string&), void), asCALL_THISCALL);
  engine->RegisterObjectMethod(musicmanagerClass, "void PlayNext()",           asMETHOD(MusicManager,PlayNext),  asCALL_THISCALL);
  engine->RegisterObjectMethod(musicmanagerClass, "void SetVolume(float)",     asMETHOD(MusicManager,SetVolume), asCALL_THISCALL);
  engine->RegisterGlobalFunction("Music@ MusicHandle(void)", asFUNCTION(MusicManager::Get), asCALL_CDECL);
  
  const char* soundInstanceClass = "Sound";
  engine->RegisterObjectType(soundInstanceClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(soundInstanceClass, "void  Play()",  asMETHOD(ISampleInstance,Start), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundInstanceClass, "void  Stop()",  asMETHOD(ISampleInstance,Stop),  asCALL_THISCALL);
  engine->RegisterObjectMethod(soundInstanceClass, "void  Pause()", asMETHOD(ISampleInstance,Pause), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundInstanceClass, "void  SetVolume(float)", asMETHOD(ISampleInstance,SetVolume), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundInstanceClass, "float GetVolume() const", asMETHOD(ISampleInstance,GetVolume), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundInstanceClass, "bool  IsPlaying()", asMETHOD(ISampleInstance,IsPlaying), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundInstanceClass, "void  AddReference()", asMETHOD(ISampleInstance,AddReference), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundInstanceClass, "void  DelReference()", asMETHOD(ISampleInstance,DelReference), asCALL_THISCALL);
  
  const char* soundManagerClass = "SoundManager";
  engine->RegisterObjectType(soundManagerClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterGlobalFunction("SoundManager@ NewSoundManager()", asFUNCTION(SoundManager::NewSoundManager), asCALL_CDECL);
  engine->RegisterObjectMethod(soundManagerClass, "void Release()", asMETHOD(SoundManager,Release), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundManagerClass, "bool   RequireSound(string)",   asMETHOD(SoundManager,Require),        asCALL_THISCALL);
  engine->RegisterObjectMethod(soundManagerClass, "Sound@ CreateInstance(string)", asMETHOD(SoundManager,CreateInstance), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundManagerClass, "void   DeleteInstance(Sound@)", asMETHOD(SoundManager,DeleteInstance), asCALL_THISCALL);
  engine->RegisterObjectMethod(soundManagerClass, "void   SetVolume(float)",       asMETHOD(SoundManager,SetVolume),      asCALL_THISCALL);
  engine->RegisterObjectMethod(soundManagerClass, "float   GetVolume(void) const", asMETHOD(SoundManager,GetVolume),      asCALL_THISCALL);
  
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
  engine->RegisterObjectMethod   (dataClass, "void       Duplicate(Data)",           asMETHOD(Data, Duplicate),          asCALL_THISCALL);
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
  engine->RegisterObjectMethod   (dataClass, "bool   NotNil()",                asMETHOD(Data,NotNil),              asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "string Key()",                   asMETHOD(Data,Key),                 asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "string AsString()",              asMETHOD(Data,Value),               asCALL_THISCALL);
  engine->RegisterObjectMethod   (dataClass, "int    AsInt()",                 asFUNCTION(asData::getAsInt),       asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "float  AsFloat()",               asFUNCTION(asData::getAsFloat),     asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod   (dataClass, "void   Output()",                asMETHOD(Data,Output),              asCALL_THISCALL);

  const char* statsheetClass = "Special";
  engine->RegisterObjectType(statsheetClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(statsheetClass, "void SetCurrentHp(int)",         asMETHOD(StatController,SetCurrentHp),  asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "void AddExperience(int)",        asMETHOD(StatController,AddExperience), asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "void SetStatistic(string, int)", asMETHOD(StatController,SetStatistic),  asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "Data GetData()",                 asMETHOD(StatController,GetData),       asCALL_THISCALL);

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
  const char* shelfClass     = "Shelf";
  engine->RegisterObjectType(dynObjectClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(charClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(doorClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(shelfClass,     0, asOBJ_REF | asOBJ_NOCOUNT);

  Script::StdList<ObjectCharacter*>::Register(engine, "CharacterList", "Character@");
  engine->RegisterObjectMethod(charClass, "CharacterList GetNearbyAllies()",  asMETHOD(ObjectCharacter,GetNearbyAllies),  asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "CharacterList GetNearbyEnemies()", asMETHOD(ObjectCharacter,GetNearbyEnemies), asCALL_THISCALL);

  engine->RegisterObjectMethod(dynObjectClass, "string GetName()",                    asMETHOD(InstanceDynamicObject,GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod(dynObjectClass, "Character@ AsCharacter()",            asFUNCTION(asUtils::DynObjAsCharacter), asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(dynObjectClass, "void AddTextBox(string, int, int, int, float)", asMETHOD(InstanceDynamicObject,AddTextBox), asCALL_THISCALL);
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
  engine->RegisterObjectMethod(charClass, "int  GetPathSize() const",                 asMETHOD(ObjectCharacter,GetPathSize), asCALL_THISCALL);
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
  engine->RegisterObjectMethod(charClass, "string GetFactionName()",                  asMETHOD(ObjectCharacter,GetFactionName), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void   SetFaction(string)",                asMETHODPR(ObjectCharacter,SetFaction,(const string&), void),     asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void   SetAsEnemy(Character@, bool)",      asMETHOD(ObjectCharacter,SetAsEnemy),     asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool   IsEnemy(const Character@) const",   asMETHOD(ObjectCharacter,IsEnemy),        asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool   IsAlly(const Character@) const",    asMETHOD(ObjectCharacter,IsAlly),         asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "Special@ GetStatController()",             asMETHOD(ObjectCharacter,GetStatController), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void   SetFurtive(bool)",                  asMETHOD(ObjectCharacter,SetFurtive),        asCALL_THISCALL);

  engine->RegisterObjectMethod(doorClass, "void   Unlock()",     asMETHOD(Lockable,Unlock),         asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool   IsLocked()",   asMETHOD(Lockable,IsLocked),       asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool   IsOpen()",     asMETHOD(Lockable,IsOpen),         asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "string GetKeyName()", asMETHOD(Lockable,GetKeyName),     asCALL_THISCALL);
  
  engine->RegisterObjectMethod(shelfClass,  "Inventory@ GetInventory()", asMETHOD(ObjectShelf,GetInventory), asCALL_THISCALL);
  
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
  engine->RegisterObjectMethod(levelClass, "Character@     GetCharacter(string)",                  asMETHODPR(Level,GetCharacter,(const std::string&),ObjectCharacter*), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Character@     GetPlayer()",                           asMETHOD(Level,GetPlayer),           asCALL_THISCALL);  
  engine->RegisterObjectMethod(levelClass, "DynamicObject@ GetObject(string)",                     asMETHOD(Level,GetObject),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUseWeaponOn(Character@, Character@, Item@, int)",     asMETHOD(Level,ActionUseWeaponOn), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUseObjectOn(Character@, DynamicObject@, Item@, int)", asMETHOD(Level,ActionUseObjectOn), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUseSkillOn(Character@, DynamicObject@, string)",      asMETHOD(Level,ActionUseSkillOn),  asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionUse(Character@, DynamicObject@)",                     asMETHOD(Level,ActionUse),         asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           ActionDropObject(Character@, Item@)",   asMETHOD(Level,ActionDropObject),    asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           PlayerLoot(Inventory@)",                asMETHOD(Level,PlayerLoot),          asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StartFight(Character@)",                asMETHOD(Level,StartFight),          asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StopFight()",                           asMETHOD(Level,StopFight),           asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           NextTurn()",                            asMETHOD(Level,NextTurn),            asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Sound@         PlaySound(string)",                     asMETHOD(Level,PlaySound),           asCALL_THISCALL);

  const char* worldmapClass = "WorldMap";
  engine->RegisterObjectType(worldmapClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(worldmapClass, "void SetCityVisible(string)", asMETHOD(WorldMap,SetCityVisible), asCALL_THISCALL);
  engine->RegisterObjectMethod(worldmapClass, "Data GetDataEngine()",        asMETHOD(WorldMap,GetDataEngine),  asCALL_THISCALL);

  const char* questClass = "Quest";
  engine->RegisterObjectType(questClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  OBJ_REF_REGISTER_METHOD(Quest, void, CompleteCondition,    (string, string));
  OBJ_REF_REGISTER_METHOD(Quest, bool, IsConditionCompleted, ());
  OBJ_REF_REGISTER_METHOD(Quest, bool, IsOver,               ());
  OBJ_REF_REGISTER_METHOD(Quest, Data, GetData,              ());
  OBJ_REF_REGISTER_METHOD(Quest, void, Finalize,             ());
  engine->RegisterObjectMethod(questClass, "void Initialize(Level@)", asMETHOD(Quest,Initialize), asCALL_THISCALL);

  const char* questmanagerClass = "QuestManager";
  engine->RegisterObjectType(questmanagerClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(questmanagerClass, "void AddQuest(Data)", asMETHOD(QuestManager,AddQuest), asCALL_THISCALL);
  engine->RegisterObjectMethod(questmanagerClass, "Quest@ opIndex(const string &in)", asMETHODPR(QuestManager,operator[], (const std::string&), Quest*), asCALL_THISCALL);
  
  const char* partyClass = "Party";
  engine->RegisterObjectType(partyClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(partyClass, "void Join(Character@)",  asMETHODPR(Party,Join,(InstanceDynamicObject*),void), asCALL_THISCALL);
  engine->RegisterObjectMethod(partyClass, "void Leave(Character@)", asMETHODPR(Party,Join,(InstanceDynamicObject*),void), asCALL_THISCALL);

  const char* gametaskClass = "Game";
  engine->RegisterObjectType(gametaskClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(gametaskClass, "void PushBuff(Character@, Data)", asMETHODPR(GameTask,PushBuff,(ObjectCharacter*,Data),void), asCALL_THISCALL);
  engine->RegisterObjectMethod(gametaskClass, "void PushBuff(string,     Data)", asMETHODPR(GameTask,PushBuff,(const string&,Data),   void), asCALL_THISCALL);
  engine->RegisterObjectMethod(gametaskClass, "QuestManager@ GetQuestManager()", asMETHOD(GameTask,GetQuestManager), asCALL_THISCALL);
  engine->RegisterObjectMethod(gametaskClass, "Party@ GetPlayerParty()", asMETHOD(GameTask,GetPlayerParty), asCALL_THISCALL);

  engine->RegisterGlobalProperty("Level@    level",    &(Level::CurrentLevel));
  engine->RegisterGlobalProperty("WorldMap@ worldmap", &(WorldMap::CurrentWorldMap));
  engine->RegisterGlobalProperty("Game@     game",     &(GameTask::CurrentGameTask));
  
  const char* rmlDocumentClass = "RmlElement";
  engine->RegisterObjectType(rmlDocumentClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(rmlDocumentClass, "string      GetId()",                          asFUNCTION(asUtils::RocketGetId),          asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(rmlDocumentClass, "RmlElement@ GetElementById(string)",           asFUNCTION(asUtils::RocketGetElementById), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(rmlDocumentClass, "void        SetInnerRML(string)",              asFUNCTION(asUtils::RocketSetInnerRML),    asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(rmlDocumentClass, "void        ClearEventListeners()",            asFUNCTION(asUtils::RocketClearListeners), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(rmlDocumentClass, "void        AddEventListener(string, string)", asFUNCTION(asUtils::RocketSetListener),    asCALL_CDECL_OBJFIRST);
}
