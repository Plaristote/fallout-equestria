#include "scriptengine.hpp"
#include "dataengine.hpp"

#include "gametask.hpp"
#include "quest_manager.hpp"

#include "level/level.hpp"
#include "level/objects/door.hpp"
#include "level/objects/locker.hpp"
#include "level/objects/shelf.hpp"

#include "worldmap/worldmap.hpp"

#include "level/interactions/action_use.hpp"
#include "level/interactions/action_use_object.hpp"
#include "level/interactions/action_use_skill.hpp"
#include "level/interactions/action_use_spell.hpp"
#include "level/interactions/action_use_weapon.hpp"

/*
 * Script Interface for Tasks
 */
#include "timer.hpp"
#include "executor.hpp"

//#include "gameui.hpp"
#include "ui/alert_ui.hpp"
#include "debuginfo.hpp"

DebugInfo warning, info, debug;

void ScriptApiDeclareFunction(const std::string& name, const std::string& decl)
{
  std::cout << "ScriptApiDefiningMethod" << std::endl;
  AngelScript::Object* object = AngelScript::ContextLock::CurrentObject();

  if (object)
    object->asDefineMethod(name, decl);
}

#include "scheduled_task.hpp"

class ScheduledScriptTask : public ScheduledTask
{
  ScheduledScriptTask();
  virtual ~ScheduledScriptTask();
public:
  static void  asConstructor(void* memory) { new(memory) ScheduledScriptTask();                      }
  static void  asDestructor (void* memory) { ((ScheduledScriptTask*)memory)->~ScheduledScriptTask(); }

  void         SetCallback(const std::string& function)
  {
    object   = AngelScript::ContextLock::CurrentObject();
    callback = function;
    if (!(object->IsDefined(callback)))
      object->asDefineMethod(callback, "void " + callback + "()");
  }

protected:
  virtual void Run();

private:
  AngelScript::Object* object;
  std::string          callback;
};

ScheduledScriptTask::ScheduledScriptTask() : object(0)
{
}

ScheduledScriptTask::~ScheduledScriptTask()
{
}

void ScheduledScriptTask::Run()
{
  cout << "Running task" << endl;
  try
  {
    if (object)
      object->Call(callback);
  }
  catch (const AngelScript::Exception& exception)
  {
    const std::string message = "A script crashed: ";

    AlertUi::NewAlert.Emit(message + exception.what());
  }
  ScheduledTask::Run();
  cout << "Has task been deleted ?" << endl;
}

/*
 * Script Interface for Rocket Listeners
 */
namespace ScriptApi
{
  class Character
  {
  public:
    static bool HasEnemiesInSight(ObjectCharacter* character)
    {
      return (character != 0 && character->GetFieldOfView().HasLivingEnemiesInSight());
    }

    static void GoTo(ObjectCharacter* character, int waypoint_id)
    {
      if (character)
      {
        Waypoint* waypoint = character->GetLevel()->GetWorld()->GetWaypointFromId(waypoint_id);
        
        if (waypoint)
          character->GoTo(waypoint);
      }
    }

    static int GetCurrentWaypoint(ObjectCharacter* character)
    {
      if (character)
      {
        Waypoint* waypoint = character->GetOccupiedWaypoint();

        return (waypoint == 0 ? 0 : waypoint->id);
      }
      return (0);
    }
    
    static void TeleportTo(ObjectCharacter* character, int waypoint_id)
    {
      if (character)
      {
        Waypoint* waypoint = character->GetLevel()->GetWorld()->GetWaypointFromId(waypoint_id);
        
        if (waypoint)
          character->TeleportTo(waypoint);
      }
    }
    
    static int GetPathSize(ObjectCharacter* character)
    {
      if (character)
        return (character->GetPath().Size());
      return (0);
    }
    
    static void MoveTowards(ObjectCharacter* character, InstanceDynamicObject* object)
    {
      if (character && object)
      {
        Waypoint* waypoint = character->GetClosestWaypointFrom(object);
        
        if (waypoint && waypoint != character->GetOccupiedWaypoint())
          character->GoTo(waypoint);
      }
    }
    
    static void MoveAwayFrom(ObjectCharacter* character, InstanceDynamicObject* object)
    {
      if (character && object)
      {
        Waypoint* waypoint = character->GetFarthestWaypointFrom(object);

        if (waypoint && waypoint != character->GetOccupiedWaypoint())
          character->GoTo(waypoint);
      }
    }

  private:
  };

  class Inventory
  {
  public:
    static int GetItemCount(::Inventory* inventory)
    {
      return (inventory->GetContent().Size());
    }

    static InventoryObject* opIndex(::Inventory* inventory, int i)
    {
      return (inventory->GetContent()[i]);
    }
  };
  
  class Item
  {
  public:
    static int            HitSuccessRate(InventoryObject* item, ObjectCharacter* user, ObjectCharacter* target, const std::string& use_type)
    {
      if (item)
        return (item->HitSuccessRate(user, target, GetUseIteratorForType(*item, use_type)));
      return (0);
    }

    static unsigned short GetActionPointCost(InventoryObject* item, ObjectCharacter* user, const std::string& use_type)
    {
      if (item)
        return (item->GetActionPointCost(user, GetUseIteratorForType(*item, use_type)));
      return (0);
    }

    static int            GetActionFromName(InventoryObject* item, const std::string& name)
    {
      if (item)
        return (GetUseIteratorForType(*item, name));
      return (0);
    }

    static bool           UseAsWeapon(InventoryObject* item, ObjectCharacter* user, ObjectCharacter* target, const std::string& use_type)
    {
      if (item)
        return (item->UseAsWeapon(user, target, GetUseIteratorForType(*item, use_type)));
      return (false);
    }

    static bool           UseOn(InventoryObject* item, ObjectCharacter* user, InstanceDynamicObject* target, const std::string& use_type)
    {
      if (item)
        return (item->UseOn(user, target, GetUseIteratorForType(*item, use_type)));
      return (false);
    }

    static bool           Use(InventoryObject* item, ObjectCharacter* user, const std::string& use_type)
    {
      if (item)
        return (item->Use(user, GetUseIteratorForType(*item, use_type)));
      return (false);
    }

  private:
    static unsigned char  GetUseIteratorForType(InventoryObject& item, const std::string& use_type)
    {
      auto iterator = item["actions"].begin();
      auto over     = item["actions"].end();
      
      for (unsigned char action_count = 0 ; iterator != over ; ++iterator, ++action_count)
      {
        Data action = *iterator;
        
        if (action.Key() == use_type)
          return (action_count);
      }
      cout << "[InventoryObject] Action " << use_type << " not found for item " << item.GetName() << endl;
      return (0);
    }
  };

  class RocketUi : public UiBase
  {
    struct EventListener : public RocketListener
    {
      EventListener()
      {
        EventReceived.Connect(*this, &EventListener::Callback);
      }
      
      void Callback(Rocket::Core::Event& event)
      {
        try
        {
          if (object->IsDefined(callback))
          {
            std::string                               event_name = event.GetType().CString();
            AngelScript::Type<Data*>                  param1(&GameTask::CurrentGameTask->GetDataEngine());
            AngelScript::Type<Rocket::Core::Element*> param2(event.GetCurrentElement());
            AngelScript::Type<std::string*>           param3(&event_name);

            object->Call(callback, 3, &param1, &param2, &param3);
          }
          else
            cout << "[AngelScript][UserInterface] Cannot react to '" << this->event << "' on '" << id << "': function '" << callback << "' does not exist." << endl;
        }
        catch (const AngelScript::Exception& e)
        {
          AlertUi::NewAlert.Emit(std::string("Script crashed: ") + e.what());
        }
      }

      std::string          callback, id, event;
      AngelScript::Object* object;
    };
    
  public: 
    RocketUi(GameUi& game_ui, AngelScript::Object* object, Rocket::Core::Element* root) : UiBase(game_ui.GetWindowFramework(), game_ui.GetRocketRegion()->get_context()), object(object)
    {
      root                      = root->GetOwnerDocument();
      root_outlives_this_object = true;
      observer_object_destroyed = object->ObjectDestroyed.Connect([this](void) { delete this; });
    }

    ~RocketUi()
    {
      object->ObjectDestroyed.Disconnect(observer_object_destroyed);
      for_each(listeners.begin(), listeners.end(), [](EventListener* i) { delete i; });
    }
    
    void AddEventListener(const std::string& id, const std::string& event, const std::string& callback)
    {
      EventListener* new_listener = new EventListener;

      DelEventListener(id, event, callback);
      if (!(object->IsDefined(callback)))
        object->asDefineMethod(callback, "void " + callback + "(Data, RmlElement@, string)");
      new_listener->id       = id;
      new_listener->event    = event;
      new_listener->callback = callback;
      new_listener->object   = object;
      ToggleEventListener(true, id, event, *new_listener);
      listeners.push_back(new_listener);
    }
    
    void DelEventListener(const std::string& id, const std::string& event, const std::string& callback)
    {
      auto it = find_if(listeners.begin(), listeners.end(), [id, event, callback](EventListener* listener) -> bool
      {
        return (listener->id == id && listener->event == event && listener->callback == callback);
      });

      if (it != listeners.end())
      {
        ToggleEventListener(false, id, event, **it);
        delete *it;
        listeners.erase(it);
      }
    }
    
    static RocketUi* Factory(Rocket::Core::Element* root)
    {
      AngelScript::Object* object = AngelScript::ContextLock::CurrentObject();

      if (object && root)
      {
        RocketUi* rocket_ui = new RocketUi(GameTask::CurrentGameTask->GetGameUi(), object, root);

        return (rocket_ui);
      }
      return (0);
    }
    
    static void      Destroy(RocketUi* rocket_ui)
    {
      delete rocket_ui;
    }

  private:
    AngelScript::Object*      object;
    std::list<EventListener*> listeners;
    Sync::ObserverId          observer_object_destroyed;
  };
}

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

  std::string getAsString(Data* obj)                              { return (obj && obj->NotNil() ? obj->Value() : "");  }
  int         getAsInt(Data* obj)                                 { return (obj && obj->NotNil() ? (int)*obj   : 0);    }
  float       getAsFloat(Data* obj)                               { return (obj && obj->NotNil() ? (float)*obj : 0.f);  }
}

namespace asUtils
{
  void SetDebugOutputEnabled(bool enabled) { debug.set_enabled(enabled); }

  InstanceDynamicObject* CharacterAsObject(ObjectCharacter* character)    { return (character); }
  ObjectCharacter*       DynObjAsCharacter(InstanceDynamicObject* object) { return (object->Get<ObjectCharacter>()); }
  ObjectDoor*            DynObjAsDoor(InstanceDynamicObject* object)      { return (object->Get<ObjectDoor>()); }
  Data                   ItemAsData(InventoryObject* item)                { return (*item); }

  list<InstanceDynamicObject*> GetObjectsInRadius(Level* self, float x, float y, float z, float radius)
  {
    return (self->GetObjectsInRadius(LPoint3f(x, y, z), radius));
  }

  void SerializeInt   (Utils::Packet* packet, int i)                      { (*packet) << i;     }
  void SerializeString(Utils::Packet* packet, const std::string& str)     { (*packet) << str;   }
  void SerializeStrArr(Utils::Packet* packet, std::list<string> array)    { (*packet) << array; }
  void UnserializeInt   (Utils::Packet* packet, int& i)                   { (*packet) >> i;     }
  void UnserializeString(Utils::Packet* packet, std::string& str)         { (*packet) >> str;   }
  void UnserializeStrArr(Utils::Packet* packet, std::list<string>& array) { (*packet) >> array; }

  std::string            RocketGetId(Rocket::Core::Element* self)                            { return (self->GetId().CString());          }
  Rocket::Core::Element* RocketGetElementById(Rocket::Core::Element* self, const string& id) { return (self->GetElementById(id.c_str())); }
  void                   RocketSetInnerRML(Rocket::Core::Element* self, const string& rml)   { return (self->SetInnerRML(rml.c_str()));   }

  void GoTo(int x, int y, int z, ObjectCharacter* character)
  {
    //character->GoTo(LPoint3f(x, y, z));
  }

  int GetPathDistance(ObjectCharacter* self, InstanceDynamicObject* target)
  {
    return (target->GetPathTowardsObject(self).Size());
  }
  
  void AppendToConsole(Level* level, const std::string& message)
  {
    level->GetLevelUi().GetMainBar().AppendToConsole(message);
  }

  void ActionUse(InstanceDynamicObject* obj, InstanceDynamicObject* user)
  {
    if (obj != 0)
      obj->ActionUse(user);
  }

  void TalkTo(InstanceDynamicObject* obj, ObjectCharacter* user)
  {
    cout << "asUtils::ActionTalkTo" << endl;
    if (obj != 0)
      obj->ActionTalkTo(user);
  }

  namespace LevelUtils
  {
    void SunlightNearFar(Level* level, float a, float b)
    {
      Sunlight* sunlight = level->GetSunlight();

      if (sunlight)
        sunlight->SetNearFar(a, b);
    }
  }
  
  namespace Combat
  {
    void StartFight(Level* level, ObjectCharacter* character)
    {
      if (level)
        level->GetCombat().Start(character);
    }
    
    void NextTurn(Level* level)
    {
      if (level)
        level->GetCombat().NextTurn();
    }
    
    bool StopFight(Level* level)
    {
      if (level)
      {
        ::Combat& combat = level->GetCombat();

        if (combat.CanStop())
        {
          combat.Stop();
          return (true);
        }
      }
      return (false);
    }
  }
  
  int ceil(float value)
  {
#ifdef __llvm__
    return (::ceil(value));
#else
    return (std::ceil(value));
#endif
  }
  
  int floor(float value)
  {
#ifdef __llvm__
    return (::floor(value));
#else
    return (std::floor(value));
#endif
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
  
  engine->RegisterGlobalFunction("void SetDebugOutputEnabled(bool)", asFUNCTION(asUtils::SetDebugOutputEnabled), asCALL_CDECL);
  engine->RegisterGlobalFunction("void Cout(string)", asFUNCTION(AngelCout), asCALL_CDECL);
  engine->RegisterGlobalFunction("void LF()", asFUNCTION( GameConsole::ListFunctions ), asCALL_CDECL);
  engine->RegisterGlobalFunction("void PrintScenegraph()", asFUNCTION( GameConsole::PrintScenegraph ), asCALL_CDECL);
  engine->RegisterGlobalFunction("void Write(const string &in)", asFUNCTION(GameConsole::WriteOn), asCALL_CDECL);
  engine->RegisterGlobalFunction("void SetLanguage(const string& in)", asFUNCTION(i18n::Load), asCALL_CDECL);
  engine->RegisterGlobalFunction("int  Random()", asFUNCTION(rand), asCALL_CDECL);
  engine->RegisterGlobalFunction("int ceil(float)", asFUNCTION(asUtils::ceil), asCALL_CDECL);
  engine->RegisterGlobalFunction("int floor(float)", asFUNCTION(asUtils::floor), asCALL_CDECL);

  engine->RegisterGlobalFunction("void ApiDefineFunction(string, string)", asFUNCTION(ScriptApiDeclareFunction), asCALL_CDECL);

  Script::StdList<string>::Register(engine, "StringList", "string");

#define SCHEDULED_TASK_BINDINGS(target, klass) \
  engine->RegisterObjectMethod(target, "void Launch()",                       asMETHOD(klass,Launch),                       asCALL_THISCALL); \
  engine->RegisterObjectMethod(target, "void Interrupt()",                    asMETHOD(klass,Interrupt),                    asCALL_THISCALL); \
  engine->RegisterObjectMethod(target, "void SetAsRepetitiveTask(bool)",      asMETHOD(klass,SetAsRepetitiveTask),          asCALL_THISCALL); \
  engine->RegisterObjectMethod(target, "void SetIntervalDuration(int)",       asMETHOD(klass,SetIntervalDurationInSeconds), asCALL_THISCALL); \
  engine->RegisterObjectMethod(target, "void SetAsDailyTask(int, int)",       asMETHOD(klass,SetAsDailyTask),               asCALL_THISCALL); \
  engine->RegisterObjectMethod(target, "void SetAsHourlyTask(int)",           asMETHOD(klass,SetAsHourlyTask),              asCALL_THISCALL); \
  engine->RegisterObjectMethod(target, "void SetAsWeeklyTask(int, int, int)", asMETHOD(klass,SetAsWeeklyTask),              asCALL_THISCALL);

  const char* scriptTaskClass = "ScheduledTask";
  engine->RegisterObjectType(scriptTaskClass, sizeof(ScheduledScriptTask), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR);
  engine->RegisterObjectBehaviour(scriptTaskClass, asBEHAVE_CONSTRUCT, "void f()",     asFUNCTION(ScheduledScriptTask::asConstructor),             asCALL_CDECL_OBJLAST);
  engine->RegisterObjectBehaviour(scriptTaskClass, asBEHAVE_DESTRUCT,  "void f()",     asFUNCTION(ScheduledScriptTask::asDestructor),              asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(scriptTaskClass, "void SetCallback(string)",            asMETHOD(ScheduledScriptTask,SetCallback),                  asCALL_THISCALL);
  SCHEDULED_TASK_BINDINGS(scriptTaskClass, ScheduledScriptTask);

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
  engine->RegisterObjectMethod   (dataClass, "void   Remove()",                asMETHOD(Data,Remove),              asCALL_THISCALL);

  const char* statsheetClass = "Special";
  engine->RegisterObjectType(statsheetClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(statsheetClass, "void SetCurrentHp(int)",          asMETHOD(StatController,SetCurrentHp),    asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "void SetArmorClass(int)",         asMETHOD(StatController,SetArmorClass),   asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "void SetActionPoints(int)",       asMETHOD(StatController,SetActionPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "void AddExperience(int)",         asMETHOD(StatController,AddExperience),   asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "void AddReputation(string, int)", asMETHOD(StatController,AddReputation),   asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "void SetStatistic(string, int)",  asMETHOD(StatController,SetStatistic),    asCALL_THISCALL);
  engine->RegisterObjectMethod(statsheetClass, "Data GetData()",                  asMETHOD(StatController,GetData),         asCALL_THISCALL);

  const char* itemClass      = "Item";
  const char* inventoryClass = "Inventory";
  const char* partyClass     = "Party";
  const char* dynObjectClass = "DynamicObject";
  const char* charClass      = "Character";
  const char* doorClass      = "Door";
  const char* shelfClass     = "Shelf";
  const char* taskClass      = "Task";
  const char* tasksClass     = "Tasks";
  engine->RegisterObjectType(itemClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(inventoryClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(dynObjectClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(charClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(doorClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(shelfClass,     0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(partyClass,     0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(taskClass,      0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectType(tasksClass,     0, asOBJ_REF | asOBJ_NOCOUNT);

  Script::StdList<string>::Register(engine, "ItemList", "Item@");
  engine->RegisterObjectMethod(inventoryClass, "void      AddObject(string)",             asMETHODPR(Inventory,AddObject, (const string&), InventoryObject*), asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "void      DelObject(Item@)",              asMETHOD(Inventory,DelObject),                                      asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "Item@     GetObject(string)",             asMETHODPR(Inventory,GetObject, (const string&), InventoryObject*), asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "int       ContainsHowMany(string) const", asMETHOD(Inventory,ContainsHowMany),                                asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "void      ResetFromFixtures()",           asMETHOD(Inventory,ResetItemsFromFixtures),                         asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "int       GetItemCount() const",          asFUNCTION(ScriptApi::Inventory::GetItemCount), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(inventoryClass, "Item@     opIndex(int)",                  asFUNCTION(ScriptApi::Inventory::opIndex),      asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(inventoryClass, "ItemList  GetContent()",                  asMETHODPR(Inventory,GetContent, (void), Script::StdList<InventoryObject*>&), asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "Item@     GetEquipedItem(string, int)",   asMETHOD(Inventory,GetEquipedItem), asCALL_THISCALL);
  engine->RegisterObjectMethod(inventoryClass, "void      SetEquipedItem(string, int, Item@, string)", asMETHODPR(Inventory,SetEquipedItem, (const string&, unsigned int, InventoryObject*, const string&), void), asCALL_THISCALL);

  engine->RegisterObjectMethod(dynObjectClass, "Tasks@ GetTaskSet()", asMETHOD(InstanceDynamicObject,GetTaskSet), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass,      "Tasks@ GetTaskSet()", asMETHOD(ObjectCharacter,GetTaskSet),       asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass,      "Tasks@ GetTaskSet()", asMETHOD(ObjectDoor,GetTaskSet),            asCALL_THISCALL);
  engine->RegisterObjectMethod(shelfClass,     "Tasks@ GetTaskSet()", asMETHOD(ObjectShelf,GetTaskSet),           asCALL_THISCALL);

  engine->RegisterObjectMethod(tasksClass, "Task@ PushTask(string)",   asMETHOD(TaskSet,PushTask),   asCALL_THISCALL);
  engine->RegisterObjectMethod(tasksClass, "Task@ GetTask(string)",    asMETHOD(TaskSet,GetTask),    asCALL_THISCALL);
  engine->RegisterObjectMethod(tasksClass, "void  RemoveTask(string)", asMETHOD(TaskSet,RemoveTask), asCALL_THISCALL);

  engine->RegisterObjectMethod(taskClass, "void AddGfx(string, string)", asMETHOD(ScriptedTask,AddGfx),    asCALL_THISCALL);
  engine->RegisterObjectMethod(taskClass, "void DeleteGfx(string)",      asMETHOD(ScriptedTask,DeleteGfx), asCALL_THISCALL);
  engine->RegisterObjectMethod(taskClass, "Data GetData()",              asMETHOD(ScriptedTask,GetData),   asCALL_THISCALL);
  engine->RegisterObjectMethod(taskClass, "DynamicObject@ GetTarget()",  asMETHOD(ScriptedTask,GetTarget), asCALL_THISCALL);
  SCHEDULED_TASK_BINDINGS(taskClass, ScriptedTask);

  engine->RegisterObjectMethod(itemClass, "string GetName() const",                                asMETHOD(InventoryObject,GetName),               asCALL_THISCALL);
  engine->RegisterObjectMethod(itemClass, "Data   AsData()",                                       asFUNCTION(asUtils::ItemAsData),                 asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(itemClass, "bool   IsWeapon() const",                               asMETHOD(InventoryObject,IsWeapon),              asCALL_THISCALL);
  engine->RegisterObjectMethod(itemClass, "int    HitSuccessRate(Character@, Character@, string)", asFUNCTION(ScriptApi::Item::HitSuccessRate),     asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(itemClass, "bool   UseWeaponOn(Character@, Character@, string)",    asFUNCTION(ScriptApi::Item::UseAsWeapon),        asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(itemClass, "bool   UseOn(Character@, DynamicObject@, string)",      asFUNCTION(ScriptApi::Item::UseOn),              asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(itemClass, "bool   Use(Character@, string)",                        asFUNCTION(ScriptApi::Item::Use),                asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(itemClass, "int    GetActionPointCost(Character@, string)",         asFUNCTION(ScriptApi::Item::GetActionPointCost), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(itemClass, "int    GetActionFromName(string)",                      asFUNCTION(ScriptApi::Item::GetActionFromName),  asCALL_CDECL_OBJFIRST);

  engine->RegisterObjectMethod(partyClass, "void   SetName(string)",     asMETHOD(Party,SetName), asCALL_THISCALL);
  engine->RegisterObjectMethod(partyClass, "string GetName()",           asMETHOD(Party,GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod(partyClass, "void Join(string)",  asMETHODPR(Party,Join,(const std::string&, const std::string&),void), asCALL_THISCALL);
  engine->RegisterObjectMethod(partyClass, "void Join(Character@)",      asMETHODPR(Party,Join,(ObjectCharacter*),void), asCALL_THISCALL);
  engine->RegisterObjectMethod(partyClass, "void Leave(Character@)",     asMETHODPR(Party,Leave,(ObjectCharacter*),void), asCALL_THISCALL);
  engine->RegisterObjectMethod(partyClass, "bool IsInParty(Character@)", asMETHOD(Party,IsInParty), asCALL_THISCALL);

  Script::StdList<ObjectCharacter*>::Register(engine, "CharacterList", "Character@");
  engine->RegisterObjectMethod(charClass, "CharacterList GetNearbyAllies()",  asMETHOD(ObjectCharacter,GetNearbyAllies),  asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "CharacterList GetNearbyEnemies()", asMETHOD(ObjectCharacter,GetNearbyEnemies), asCALL_THISCALL);

  Script::StdList<ObjectCharacter*>::Register(engine, "ObjectList", "DynamicObject@");
  engine->RegisterObjectMethod(dynObjectClass, "ObjectList GetObjectsInRadius(float)",          asMETHOD(InstanceDynamicObject,GetObjectsInRadius), asCALL_THISCALL);
  engine->RegisterObjectMethod(dynObjectClass, "float GetDistance()",                           asMETHOD(InstanceDynamicObject,GetDistance),        asCALL_THISCALL);
  engine->RegisterObjectMethod(dynObjectClass, "Character@ AsCharacter()",                      asFUNCTION(asUtils::DynObjAsCharacter),       asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(dynObjectClass, "Door@      AsDoor()",                           asFUNCTION(asUtils::DynObjAsDoor),            asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(dynObjectClass, "string GetName()",                              asMETHOD(InstanceDynamicObject,GetName),      asCALL_THISCALL);
  engine->RegisterObjectMethod(dynObjectClass, "void AddTextBox(string, int, int, int, float)", asMETHOD(InstanceDynamicObject,AddTextBox),   asCALL_THISCALL);

  engine->RegisterObjectMethod(dynObjectClass, "Data GetDataStore()",                           asMETHOD(InstanceDynamicObject,GetDataStore), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass,      "Data GetDataStore()",                           asMETHOD(ObjectCharacter,GetDataStore),       asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass,      "Data GetDataStore()",                           asMETHOD(ObjectDoor,GetDataStore),            asCALL_THISCALL);
  engine->RegisterObjectMethod(shelfClass,     "Data GetDataStore()",                           asMETHOD(ObjectShelf,GetDataStore),           asCALL_THISCALL);

  engine->RegisterObjectMethod(dynObjectClass, "void Use(DynamicObject@)",                          asFUNCTION(asUtils::ActionUse), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass,      "void Use(DynamicObject@)",                          asFUNCTION(asUtils::ActionUse), asCALL_CDECL_OBJFIRST);
  //engine->RegisterObjectMethod(dynObjectClass, "void Use(DynamicObject@)",                          asMETHOD(Interactions::Target,ActionUse), asCALL_THISCALL);
  //engine->RegisterObjectMethod(charClass,      "void Use(DynamicObject@)",                          asMETHOD(ObjectCharacter,ActionUse), asCALL_THISCALL);
  engine->RegisterObjectMethod(dynObjectClass, "void TalkTo(Character@)",                       asFUNCTION(asUtils::TalkTo), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass,      "void TalkTo(Character@)",                       asMETHOD(ObjectCharacter,ActionTalkTo), asCALL_THISCALL);

  //engine->RegisterObjectMethod(charClass, "void SetRunning(const bool)",              asMETHOD(ObjectCharacter,SetRunning), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "string GetName()",                         asMETHOD(ObjectCharacter,GetName), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool HasLineOfSight(DynamicObject@)",      asMETHOD(ObjectCharacter,HasLineOfSight), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void GoTo(int, int, int)",                 asFUNCTION(asUtils::GoTo), asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(charClass, "void TeleportTo(int)",                     asFUNCTION(ScriptApi::Character::TeleportTo), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "void GoTo(int)",                           asFUNCTION(ScriptApi::Character::GoTo),       asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "void GoTo(DynamicObject@, int)",           asMETHODPR(ObjectCharacter,GoTo, (InstanceDynamicObject*, int), void), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void GoToRandomDirection()",               asMETHOD(ObjectCharacter,GoToRandomDirection), asCALL_THISCALL);
  // TODO implement C++ for that
  engine->RegisterObjectMethod(charClass, "void MoveTowards(DynamicObject@)",         asFUNCTION(ScriptApi::Character::MoveTowards),  asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "void MoveAwayFrom(DynamicObject@)",        asFUNCTION(ScriptApi::Character::MoveAwayFrom), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "void TruncatePath(int)",                   asMETHOD(ObjectCharacter,TruncatePath), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetPathDistance(DynamicObject@)",     asFUNCTION(asUtils::GetPathDistance), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "float GetDistance(DynamicObject@)",        asMETHOD(ObjectCharacter,GetDistance), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "Inventory@ GetInventory()",                asMETHODPR(ObjectCharacter,GetInventory, (), Inventory&), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "Data GetStatistics()",                     asMETHOD(ObjectCharacter,GetStatistics), asCALL_THISCALL);
  //MSVC2010 strangeness for this function: Base/Derived thingymajig complications
  engine->RegisterObjectMethod(charClass, "int  GetCurrentWaypoint() const",          asFUNCTION(ScriptApi::Character::GetCurrentWaypoint), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "int  GetPathSize() const",                 asFUNCTION(ScriptApi::Character::GetPathSize), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "bool IsInterrupted() const",               asMETHOD(ObjectCharacter,IsInterrupted), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool IsMoving() const",                    asMETHOD(ObjectCharacter,IsMoving), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool IsAlive() const",                     asMETHOD(ObjectCharacter,IsAlive),  asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "DynamicObject@ AsObject()",                asFUNCTION(asUtils::CharacterAsObject), asCALL_CDECL_OBJLAST);
  engine->RegisterObjectMethod(charClass, "int  GetActionPoints() const",             asMETHOD(ObjectCharacter,GetActionPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetMaxActionPoints() const",          asMETHOD(ObjectCharacter,GetMaxActionPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void SetActionPoints(int)",                asMETHOD(ObjectCharacter,SetActionPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "int  GetHitPoints()",                      asMETHOD(ObjectCharacter,GetHitPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void SetHitPoints(int)",                   asMETHOD(ObjectCharacter,SetHitPoints), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void LookAt(Character@)",                  asMETHODPR(ObjectCharacter,LookAt,(InstanceDynamicObject*),void),         asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void LookAt(DynamicObject@)",              asMETHODPR(ObjectCharacter,LookAt,(InstanceDynamicObject*),void),         asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "string GetFactionName()",                  asMETHOD(ObjectCharacter,GetFactionName), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void   SetFaction(string)",                asMETHODPR(ObjectCharacter,SetFaction,(const string&), void),     asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void   SetAsEnemy(Character@, bool)",      asMETHOD(ObjectCharacter,SetAsEnemy),     asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool   IsEnemy(const Character@) const",   asMETHOD(ObjectCharacter,IsEnemy),        asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool   IsAlly(const Character@) const",    asMETHOD(ObjectCharacter,IsAlly),         asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "bool HasEnemiesInSight()",                 asFUNCTION(ScriptApi::Character::HasEnemiesInSight), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(charClass, "Special@ GetStatController()",             asMETHODPR(ObjectCharacter,GetStatController,() const,StatController*), asCALL_THISCALL);
  engine->RegisterObjectMethod(charClass, "void   SetFurtive(bool)",                  asMETHOD(ObjectCharacter,SetFurtive),        asCALL_THISCALL);

  engine->RegisterGlobalFunction("void Use(Character@, DynamicObject@)",                     asFUNCTION(Interactions::Actions::Use::Factory),         asCALL_CDECL);
  engine->RegisterGlobalFunction("void UseObject(Character@, Item@, int)",                   asFUNCTION(Interactions::Actions::UseObject::Factory),   asCALL_CDECL);
  engine->RegisterGlobalFunction("void UseObjectOn(Character@, DynamicObject@, Item@, int)", asFUNCTION(Interactions::Actions::UseObjectOn::Factory), asCALL_CDECL);
  engine->RegisterGlobalFunction("void UseSpellOn(Character@, DynamicObject@, string)",      asFUNCTION(Interactions::Actions::UseSpellOn::Factory),  asCALL_CDECL);
  engine->RegisterGlobalFunction("void UseSkillOn(Character@, DynamicObject@, string)",      asFUNCTION(Interactions::Actions::UseSkillOn::Factory),  asCALL_CDECL);
  engine->RegisterGlobalFunction("void UseWeapon(Character@, Character@, Item@, int)",       asFUNCTION(Interactions::Actions::UseWeaponOn::Factory), asCALL_CDECL);

  engine->RegisterObjectMethod(doorClass, "void   Open()",       asMETHOD(ObjectDoor,Open),         asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "void   Close()",      asMETHOD(ObjectDoor,Close),        asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "void   SetLocked(bool)", asMETHOD(ObjectDoor,SetLocked), asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "void   Unlock()",     asMETHOD(Lockable,Unlock),         asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool   IsLocked()",   asMETHOD(Lockable,IsLocked),       asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "bool   IsOpen()",     asMETHOD(Lockable,IsOpen),         asCALL_THISCALL);
  engine->RegisterObjectMethod(doorClass, "string GetKeyName()", asMETHOD(Lockable,GetKeyName),     asCALL_THISCALL);

  engine->RegisterObjectMethod(shelfClass,  "Inventory@ GetInventory()", asMETHOD(ObjectShelf,GetInventory), asCALL_THISCALL);

  const char* zoneClass = "Zone";
  engine->RegisterObjectType(zoneClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(zoneClass, "string GetName() const",                  asMETHOD(ScriptZone,GetZoneName),      asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneClass, "void SetEnabled(bool)",                   asMETHOD(ScriptZone,SetEnabled),       asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneClass, "bool IsEnabled() const",                  asMETHOD(ScriptZone,IsEnabled),        asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneClass, "bool IsInside(DynamicObject@)",           asMETHOD(ScriptZone,IsInside),         asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneClass, "void Delete()",                           asMETHOD(ScriptZone,Delete),           asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneClass, "void SetEnterCallback(string)",           asMETHOD(ScriptZone,SetEnterCallback), asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneClass, "void SetExitCallback(string)",            asMETHOD(ScriptZone,SetExitCallback),  asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneClass, "void SetMovedWithinZoneCallback(string)", asMETHOD(ScriptZone,SetMovedWithinZoneCallback), asCALL_THISCALL);
  engine->RegisterGlobalFunction("Zone@ Zone_Factory(string)", asFUNCTION(ScriptZone::Factory), asCALL_CDECL);

  const char* worldLight = "Light";
  engine->RegisterObjectType(worldLight, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(worldLight, "void SetEnabled(bool)",                    asMETHOD(WorldLight,SetEnabled),     asCALL_THISCALL);
  engine->RegisterObjectMethod(worldLight, "void SetColor(float,float,float,float)",   asMETHOD(WorldLight,SetColor),       asCALL_THISCALL);
  engine->RegisterObjectMethod(worldLight, "void SetAttenuation(float, float, float)", asMETHOD(WorldLight,SetAttenuation), asCALL_THISCALL);
  
  const char* worldClass = "World";
  engine->RegisterObjectType(worldClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(worldClass, "void   SetWaypointsVisible(bool)",       asMETHOD(World,SetWaypointsVisible),      asCALL_THISCALL);
  engine->RegisterObjectMethod(worldClass, "void   SetMapObjectsVisible(bool)",      asMETHOD(World,SetMapObjectsVisible),     asCALL_THISCALL);
  engine->RegisterObjectMethod(worldClass, "void   SetDynamicObjectsVisible(bool)",  asMETHOD(World,SetDynamicObjectsVisible), asCALL_THISCALL);  
  engine->RegisterObjectMethod(worldClass, "Light@ GetLight(string)",                asMETHOD(World,GetLightByName),           asCALL_THISCALL);
  engine->RegisterObjectMethod(worldClass, "void   WriteToBam(string)",              asMETHOD(World,WriteToBam),               asCALL_THISCALL);

  const char* cameraClass = "Camera";
  engine->RegisterObjectType(cameraClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(cameraClass, "void CenterOn(DynamicObject@)",   asMETHOD(LevelCamera,CenterOnObject), asCALL_THISCALL);
  engine->RegisterObjectMethod(cameraClass, "void Follow(DynamicObject@)",     asMETHOD(LevelCamera,FollowObject),   asCALL_THISCALL);
  engine->RegisterObjectMethod(cameraClass, "void StopFollowing()",            asMETHOD(LevelCamera,StopFollowingNodePath), asCALL_THISCALL);
  engine->RegisterObjectMethod(cameraClass, "void SetEnabledTrackball(bool)",  asMETHOD(SceneCamera,SetEnabledTrackball), asCALL_THISCALL);

  const char* zoneManagerClass = "ZoneManager";
  engine->RegisterObjectType(zoneManagerClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(zoneManagerClass, "bool ZoneExists(string)", asMETHOD(Zones::Manager,ZoneExists), asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneManagerClass, "void InsertPartyInZone(Party@, string)",  asMETHOD(Zones::Manager,InsertPartyInZone),  asCALL_THISCALL);
  engine->RegisterObjectMethod(zoneManagerClass, "void InsertObjectInZone(DynamicObject@, string)", asMETHOD(Zones::Manager,InsertObjectInZone), asCALL_THISCALL);

  const char* levelClass = "Level";
  engine->RegisterObjectType(levelClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(levelClass, "void AppendToConsole(string)", asFUNCTION(asUtils::AppendToConsole), asCALL_CDECL_OBJFIRST);
  OBJ_REF_REGISTER_METHOD(Level, Data,         GetDataEngine,  ());
  OBJ_REF_REGISTER_METHOD(Level, World@,       GetWorld,       ());
  OBJ_REF_REGISTER_METHOD(Level, Camera@,      GetCamera,      ());
  OBJ_REF_REGISTER_METHOD(Level, ZoneManager@, GetZoneManager, ());
  engine->RegisterObjectMethod(levelClass, "ObjectList     GetObjectsInRadius(float,float,float,float)", asFUNCTION(asUtils::GetObjectsInRadius), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(levelClass, "Character@     GetCharacter(string)",                  asMETHODPR(Level,GetCharacter,(const std::string&),ObjectCharacter*), asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "Character@     GetPlayer()",                           asMETHOD(Level,GetPlayer),              asCALL_THISCALL);  
  engine->RegisterObjectMethod(levelClass, "DynamicObject@ GetObject(string)",                     asMETHOD(Level,GetObject),              asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           InsertParty(Party@, string)",           asMETHOD(Level,InsertParty),            asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StripParty(Party@)",                    asMETHOD(Level,RemovePartyFromLevel),   asCALL_THISCALL);
  engine->RegisterObjectMethod(levelClass, "void           StartFight(Character@)",                asFUNCTION(asUtils::Combat::StartFight),asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(levelClass, "void           StopFight()",                           asFUNCTION(asUtils::Combat::StopFight), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(levelClass, "void           NextTurn()",                            asFUNCTION(asUtils::Combat::NextTurn),  asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(levelClass, "Sound@         PlaySound(string)",                     asMETHOD(Level,PlaySound),              asCALL_THISCALL);

  engine->RegisterObjectMethod(levelClass, "void SunlightSetNearFar(float, float)", asFUNCTION(asUtils::LevelUtils::SunlightNearFar), asCALL_CDECL_OBJFIRST);

  const char* worldmapClass = "WorldMap";
  engine->RegisterObjectType(worldmapClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  OBJ_REF_REGISTER_METHOD(WorldMap, void, SetCityVisible,     (string));
  OBJ_REF_REGISTER_METHOD(WorldMap, Data, GetDataEngine,      ());
  OBJ_REF_REGISTER_METHOD(WorldMap, void, AddEntryZoneToCity, (string, string));

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
  OBJ_REF_REGISTER_METHOD(QuestManager, void, AddQuest, (Data));
  engine->RegisterObjectMethod(questmanagerClass, "Quest@ opIndex(const string &in)", asMETHODPR(QuestManager,operator[], (const std::string&), Quest*), asCALL_THISCALL);
  
  const char* gametaskClass = "Game";
  engine->RegisterObjectType(gametaskClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(gametaskClass, "QuestManager@ GetQuestManager()", asMETHOD(GameTask,GetQuestManager), asCALL_THISCALL);
  engine->RegisterObjectMethod(gametaskClass, "Party@ GetPlayerParty()",         asMETHOD(GameTask,GetPlayerParty), asCALL_THISCALL);

  engine->RegisterGlobalProperty("Level@    level",    &(Level::CurrentLevel));
  engine->RegisterGlobalProperty("WorldMap@ worldmap", &(WorldMap::CurrentWorldMap));
  engine->RegisterGlobalProperty("Game@     game",     &(GameTask::CurrentGameTask));
  
  const char* rmlDocumentClass = "RmlElement";
  engine->RegisterObjectType(rmlDocumentClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(rmlDocumentClass, "string      GetId()",                          asFUNCTION(asUtils::RocketGetId),          asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(rmlDocumentClass, "RmlElement@ GetElementById(string)",           asFUNCTION(asUtils::RocketGetElementById), asCALL_CDECL_OBJFIRST);
  engine->RegisterObjectMethod(rmlDocumentClass, "void        SetInnerRML(string)",              asFUNCTION(asUtils::RocketSetInnerRML),    asCALL_CDECL_OBJFIRST);
  
  const char* rocketUiClass = "UserInterface";
  engine->RegisterObjectType(rocketUiClass, 0, asOBJ_REF | asOBJ_NOCOUNT);
  engine->RegisterObjectMethod(rocketUiClass, "void AddEventListener(string, string, string)",  asMETHOD(ScriptApi::RocketUi,AddEventListener), asCALL_THISCALL);
  engine->RegisterObjectMethod(rocketUiClass, "void DelEventListener(string, string, string)",  asMETHOD(ScriptApi::RocketUi,DelEventListener), asCALL_THISCALL);
  engine->RegisterGlobalFunction("UserInterface@ RegisterUserInterface(RmlElement@)",      asFUNCTION(ScriptApi::RocketUi::Factory), asCALL_CDECL);
  engine->RegisterGlobalFunction("void           UnregisterUserInterface(UserInterface@)", asFUNCTION(ScriptApi::RocketUi::Destroy), asCALL_CDECL);
}
