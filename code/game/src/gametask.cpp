#include "gametask.hpp"
#include "musicmanager.hpp"
#include "quest_manager.hpp"
#include <options.hpp>
#include <dices.hpp>
#include <ui/ui_dialog.hpp>
#include "thread.hpp"
#include "my_zlib.hpp"
#include <iostream>
#include "ui/alert_ui.hpp"

using namespace std;

GameTask* GameTask::CurrentGameTask = 0;

extern PandaFramework* framework;

Buff::~Buff()
{
  if (_task)
    _tm.DelTask(_task);
}

Buff::Buff(const string& name, StatController* stats, Data data, TimeManager& tm) : _tm(tm)
{
  _buff.Duplicate(data);
  InitScripts();
  _context      = 0;
  _target_name  = name;
  _target_stats = stats;
  //_looping    = _buff["loop"].Value() == "1";
  _task       = tm.AddTask(TASK_LVL_WORLDMAP, _buff["loop"].Value() == "1", data["duration"].Nil() ? 1 : (int)data["duration"]);
  _task->Interval.Connect(*this, &Buff::Refresh);
}

Buff::Buff(Utils::Packet& packet, TimeManager& tm, function<StatController* (const string&)> get_controller) : _tm(tm)
{
  string    data_json;
  DataTree* data_tree;

  packet >> _target_name;
  packet >> data_json;
  data_tree = DataTree::Factory::StringJSON(data_json);
  if (data_tree)
  {
    _buff.Duplicate(data_tree);
    delete data_tree;
  }
  _context      = 0;
  _target_stats = get_controller(_target_name);
  _looping      = _buff["loop"].Value() == "1";
  _task         = _tm.AddTask(TASK_LVL_WORLDMAP, _looping, 0);
  _task->next_run.Serialize(packet);
  _task->length.Serialize(packet);
  _task->Interval.Connect(*this, &Buff::Refresh);
}

void Buff::Save(Utils::Packet& packet)
{
  string json;

  DataTree::Writers::StringJSON(_buff, json);
  packet << _target_name << json;
  _task->next_run.Unserialize(packet);
  _task->length.Unserialize(packet);
}

void Buff::InitScripts(void)
{
  Data           data_script  = _buff["script"];
  string         script_src, script_func, script_decl;

  cout << "[Buff] Initializing scripts" << endl;
  if (data_script.Nil())
    return ;
  cout << "[Buff] Script haz data" << endl;
  data_script.Output();
  script_src  = data_script["src"].Value();
  script_func = data_script["hook"].Value();
  script_decl = "bool " + script_func + "(Data, Special@)";
  _context    = Script::Engine::Get()->CreateContext();
  _module     = Script::ModuleManager::Require(script_src, "scripts/buffs/" + script_src);
  if (!_module || !_context)
    return ;
  _refresh    = _module->GetFunctionByDecl(script_decl.c_str());  
  if (_refresh == 0)
    cout << "[Fatal Error] Buff refresh function '" << script_decl << "' doesn't exist" << endl;
}

void Buff::Refresh(void)
{
  bool  keep_going = false;
  bool  looping    = _buff["loop"].Value() == "1";

  if (_target_stats)
  {
    InitScripts();
    if (_context)
    {
      cout << "Executing script" << endl;
      _context->Prepare(_refresh);
      _context->SetArgObject(0, &_buff);
      _context->SetArgObject(1, _target_stats);
      _context->Execute();
      keep_going = _context->GetReturnByte() != 0;
    }
  }
  if (!keep_going || !looping)
  {
    _buff.Output();
    Over.Emit(this);
  }
}

void BuffManager::Cleanup(Buff* to_del)
{
  auto it = find(buffs.begin(), buffs.end(), to_del);
  
  if (it != buffs.end())
    buffs.erase(it);
  garbage.push_back(to_del);
}

void BuffManager::CollectGarbage(void)
{
  for_each(garbage.begin(), garbage.end(), [](Buff* to_del) { delete to_del; } );
  garbage.clear();
}

void BuffManager::Load(Utils::Packet& packet, function<StatController* (const string&)> get_controller)
{
  unsigned short n_buffs;

  packet >> n_buffs;
  for (unsigned short i = 0 ; i < n_buffs ; ++i)
  {
    Buff* buff = new Buff(packet, tm, get_controller);

    buff->Over.Connect(*this, &BuffManager::Cleanup);
    buffs.push_back(buff);
  }
}

void BuffManager::Save(Utils::Packet& packet, function<bool (const string&)> callback)
{
  unsigned short n_buff = 0;
  auto           it     = buffs.begin();
  auto           end    = buffs.end();

  // How much buffs need to be saved ?
  while (it != end)
  {
    Buff* buff = *it;

    if (callback(buff->GetTargetName()))
      n_buff++;
    ++it;
  }
  packet << n_buff;

  // Now save them
  it  = buffs.begin();
  end = buffs.end();
  while (it != end)
  {
    Buff* buff = *it;

    if (callback(buff->GetTargetName()))
    {
      buff->Save(packet);
      it = buffs.erase(it);
    }
    else
      ++it;
  }
}

void GameTask::LoadLevelBuffs(Utils::Packet& packet)
{
  _buff_manager.Load(packet, [this](const string& name) -> StatController*
  {
    ObjectCharacter* character = _level->GetCharacter(name);

    return (character ? character->GetStatController() : 0);
  });
}

void GameTask::SaveLevelBuffs(Utils::Packet& packet)
{
  _buff_manager.Save(packet, _is_level_buff);
}

void GameTask::SavePartyBuffs(Utils::Packet&)
{
  auto it  = _buff_manager.buffs.begin();
  auto end = _buff_manager.buffs.end();

  while (it != end)
  {
    Buff* buff = *it;

    if (_is_level_buff(buff->GetTargetName()))
    {
      it = _buff_manager.buffs.erase(it);
      delete buff;
    }
    else
      ++it;
  }
}

void GameTask::PushBuff(ObjectCharacter* character, Data data)
{
  StatController* controller = character->GetStatController();
  Buff*           buff       = new Buff(character->GetName(), controller, data, _timeManager);

  _buff_manager.buffs.push_back(buff);
}

void GameTask::PushBuff(const std::string& name, Data data)
{
  // If Level is open, do it with level
  if (_level)
  {
    ObjectCharacter* character = _level->GetCharacter(name);

    if (character)
    {
      PushBuff(character, data);
      return ;
    }
  }

  // If target hasn't been found, or if level isn't open, search for target in PlayerParty
  auto it  = _playerParty->GetObjects().begin();
  auto end = _playerParty->GetObjects().end();
 
  for (; it != end ; ++it)
  {
    DynamicObject* object = *it;
    
    if (object->nodePath.get_name() == name)
    {
      StatController* controller = 0;
      Buff*           buff       = new Buff(name, controller, data, _timeManager);

      _buff_manager.buffs.push_back(buff);
      break ;
    }
  }
}

GameTask::GameTask(WindowFramework* window, GeneralUi& generalUi) : _gameUi(window, generalUi.GetRocketRegion()),
                                                                    _buff_manager(_timeManager),
                                                                    _pipbuck(window, generalUi.GetRocketRegion()->get_context(), _dataEngine)
{
  CurrentGameTask  = this;
  _continue        = true;
  _window          = window;
  _level           = 0;
  _savePath        = OptionsManager::Get()["savepath"].Value();
  _worldMap        = 0;
  _charSheet       = 0;
  _playerParty     = 0;
  _playerStats     = 0;
  _playerInventory = 0;
  _quest_manager   = 0;
  _uiSaveGame      = 0;
  _uiLoadGame      = 0;
  _gameUi.GetMenu().SaveClicked.Connect(*this, &GameTask::SaveClicked);
  _gameUi.GetMenu().LoadClicked.Connect(*this, &GameTask::LoadClicked);
  _gameUi.GetMenu().ExitClicked.Connect(*this, &GameTask::Exit);
  _gameUi.GetMenu().OptionsClicked.Connect(generalUi.GetOptions(), &UiBase::FireShow);
  _gameUi.OpenPipbuck.Connect(_pipbuck, &UiBase::FireShow);
  
  _is_level_buff   = [this](const string& name) -> bool
  {
    ObjectCharacter* character   = _level->GetCharacter(name);
    auto             party_chars = _playerParty->ConstGetObjects();
    auto             it = party_chars.begin(), end = party_chars.end();
    
    for (; it != end ; ++it)
    {
      DynamicObject* party_character = *it;
      
      if (character->GetDynamicObject() == party_character)
	return (false);
    }
    return (character != 0);
  };
  
  SyncLoadLevel.SetDirect(false);
  SyncLoadLevel.Connect(*this, &GameTask::DoLoadLevel);
  _signals.push_back(&SyncLoadLevel);
}

GameTask::~GameTask()
{
  if (_playerParty)   { delete _playerParty;   }
  if (_playerStats)   { delete _playerStats;   }
  if (_charSheet)     { delete _charSheet;     }
  if (_uiSaveGame)    { _uiSaveGame->Destroy(); delete _uiSaveGame; }
  if (_uiLoadGame)    { _uiLoadGame->Destroy(); delete _uiLoadGame; }
  if (_worldMap)      { _worldMap->Destroy();   delete _worldMap;   }
  if (_quest_manager) { delete _quest_manager; }
  if (_level)         { delete _level;         }
  CurrentGameTask = 0;
}

void                  GameTask::SaveClicked(Rocket::Core::Event&)
{
  if (_uiSaveGame)
  {
    _signals.remove(&_uiSaveGame->SaveToSlot);
    delete _uiSaveGame;
  }
  _uiSaveGame = new UiSave(_window, _gameUi.GetContext(), _savePath);
  _uiSaveGame->SaveToSlot.SetDirect(false);
  _uiSaveGame->SaveToSlot.Connect(*this, &GameTask::SaveToSlot);
  _uiSaveGame->EraseSlot.Connect (*this, &GameTask::EraseSlot);
  _uiSaveGame->Show();
  _signals.push_back(&_uiSaveGame->SaveToSlot);
}

void                  GameTask::LoadClicked(Rocket::Core::Event&)
{
  if (_uiLoadGame)
    delete _uiLoadGame;
  _uiLoadGame = new UiLoad(_window, _gameUi.GetContext(), _savePath);
  _uiLoadGame->LoadSlot.Connect(*this, &GameTask::LoadSlot);
  _uiLoadGame->EraseSlot.Connect (*this, &GameTask::EraseSlot);
  _uiLoadGame->Show();
}

void                  GameTask::MapOpenLevel(std::string name)
{
  OpenLevel(_savePath, name, "worldmap");
}

void                  GameTask::SetLevel(Level* level)
{
  _level = level;
}

void                  GameTask::GameOver(void)
{
  // TODO Implement Game Over Interface
  _continue = false;
}

void                  GameTask::Exit(Rocket::Core::Event&)
{
  UiDialog* dialog      = new UiDialog(_window, _gameUi.GetContext());

  dialog->SetMessage(i18n::T("If you quit without saving, you'll lose your recent progresses.") + "<br/>" + i18n::T("Are you sure ?"));
  dialog->AddChoice(i18n::T("No"),  [this, dialog](Rocket::Core::Event&) { dialog->SetModal(false); dialog->Hide(); });
  dialog->AddChoice(i18n::T("Yes"), [this, dialog](Rocket::Core::Event&)
  {
    _continue = false;
    dialog->SetModal(false);
    dialog->Hide();
  });
  dialog->Show();
  dialog->SetModal(true);
}

bool is_loading = false;

AsyncTask::DoneStatus GameTask::do_task()
{
  if (!_continue)
    return (AsyncTask::DS_done);
  else if (is_loading == true)
    return (AsyncTask::DS_cont);
  _signals.ExecuteRecordedCalls();

  {
    Data charsheet(_charSheet);
    if ((int)charsheet["Variables"]["Hit Points"] <= 0)
      GameOver();
  }
  if (_level)
  {
    if (_level->do_task() == AsyncTask::DS_done)
    {
      const string nextZone  = _level->GetNextZone();
      const string exitPoint = _level->GetExitZone();

      ExitLevel(_savePath);
      if (nextZone != "")
	OpenLevel(_savePath, nextZone, exitPoint);
      SaveGame(_savePath); // Auto-save for the Continue feature
    }
    if (!_level && _worldMap)
      _worldMap->Show();
  }
  else if (_worldMap)
    _worldMap->Run();
  _pipbuck.Run();
  _timeManager.ExecuteTasks();
  _buff_manager.CollectGarbage();
  return (AsyncTask::DS_cont);
}

bool GameTask::SaveGame(const std::string& savepath)
{
  bool success = true;

  _worldMap->Save(savepath);
  if (_level)
  {
    _dataEngine["system"]["current-level"] = _levelName;
    success = success && SaveLevel(_level, savepath + "/" + _levelName + ".blob");
  }
  else
    _dataEngine["system"]["current-level"] = 0;
  _playerParty->Save(savepath);

  {
    Data player_inventory = _dataEngine["player"]["inventory"];
    
    if (player_inventory.Count() != 0)
    {
      player_inventory.CutBranch();
      player_inventory = _dataEngine["player"]["inventory"];
      player_inventory.Output();
    }
    _playerInventory->SaveInventory(player_inventory);
  }
  _dataEngine["time"]["seconds"] = _timeManager.GetSecond();
  _dataEngine["time"]["minutes"] = _timeManager.GetMinute();
  _dataEngine["time"]["hours"]   = _timeManager.GetHour();
  _dataEngine["time"]["days"]    = _timeManager.GetDay();
  _dataEngine["time"]["month"]   = _timeManager.GetMonth();
  _dataEngine["time"]["year"]    = _timeManager.GetYear();
  _dataEngine.Save(savepath + "/dataengine.json");

  DataTree::Writers::JSON(_charSheet, savepath + "/stats-self.json");

  if (_level != 0)
  {
    _window->get_render().set_transparency(TransparencyAttrib::M_alpha, 1);
    framework->get_graphics_engine()->render_frame();
    _window->get_graphics_window()->get_screenshot()->write(savepath + "/preview.png");
  }

  return (success);
}

bool GameTask::LoadGame(const std::string& savepath)
{
  Data currentLevel, time;

  if (_worldMap)      delete _worldMap;
  if (_playerParty)   delete _playerParty;
  if (_playerStats)   delete _playerStats;
  if (_charSheet)     delete _charSheet;
  if (_quest_manager) delete _quest_manager;
  if (_playerInventory)
  {
    delete _playerInventory;
    _playerInventory = 0;
  }

  try { _dataEngine.Load(savepath + "/dataengine.json"); } catch (int&) { return (false); }
  currentLevel     = _dataEngine["system"]["current-level"];
  time             = _dataEngine["time"];
  _pipbuck.Restart();
  _timeManager.ClearTasks(0);
  _timeManager.SetTime(time["seconds"], time["minutes"], time["hours"], time["days"], time["month"], time["year"]);
  _charSheet       = DataTree::Factory::JSON(savepath + "/stats-self.json");
  if (!_charSheet)  return (false);
  _playerParty     = new PlayerParty(savepath);
  _playerStats     = new StatController(_charSheet);
  _quest_manager   = new QuestManager(_dataEngine, _playerStats);
  _playerStats->SetView(&(_gameUi.GetPers()));
  
  _gameUi.GetPers().SwapToPartyMember.DisconnectAll();
  _gameUi.GetPers().SwapToPartyMember.Connect([this, savepath](const std::string& name)
  {
    StatController* controller = 0;

    if (name == "self")
      controller = _playerStats;
    else
    {
      if (_level)
      {
        DataTree* charsheet = DataTree::Factory::JSON(savepath + "/stats-" + name + ".json");

        if (charsheet)
        {
          Data             char_name = Data(charsheet)["Name"];
          ObjectCharacter* character = _level->GetCharacter(char_name.Value());

          if (character)
            controller = character->GetStatController();
          else
            cout << "Character " << name << " isn't instancied in the level" << endl;
          delete charsheet;
        }
      }
      if (!(controller))
      {
        DataTree* charsheet = DataTree::Factory::JSON(savepath + "/stats-" + name + ".json");

        if (charsheet == 0)
          charsheet = DataTree::Factory::JSON("data/charsheets/" + name + ".json");
        if (charsheet != 0)
          controller = new StatController(charsheet);
      }
    }
    if (controller != 0)
      controller->SetView(&(_gameUi.GetPers()));
  });
  
  std::function<void (void)> set_party_members = [this]()
  {
    Party::Statsheets        statsheets = _playerParty->GetStatsheets();
    std::vector<std::string> members;
	GameUi&                  game_ui    = _gameUi;

    for_each(statsheets.begin(), statsheets.end(), [&game_ui, &members](std::pair<std::string, std::string> statsheet)
    {
      if (statsheet.second != "")
        members.push_back(statsheet.second);
      else
        members.push_back("self");
    });
    game_ui.GetPers().SetPartyMembers(members);
  };
  
  set_party_members();
  _playerParty->Updated.Connect(set_party_members);
  
  if (_dataEngine["player"]["inventory"].NotNil())
  {
    _playerInventory = new Inventory;
    _playerInventory->LoadInventory(_dataEngine["player"]["inventory"]);
    _gameUi.GetInventory().SetInventory(*_playerInventory);
  }

  _worldMap    = new WorldMap(_window, &_gameUi, _dataEngine, _timeManager);
  _worldMap->GoToPlace.Connect(*this, &GameTask::MapOpenLevel);
  _worldMap->GoToCityZone.Connect([this](string level, string zone)
  { OpenLevel(_savePath, level, zone); });
  _worldMap->RequestRandomEncounter.Connect(*this, &GameTask::MakeEncounter);

  if (!(currentLevel.Nil()) && currentLevel.Value() != "0")
  {
    _worldMap->Hide();
    LoadLevel(_window, _gameUi, savepath + "/" + currentLevel.Value() + ".blob", currentLevel.Value(), "", true);
  }
  else
    _worldMap->Show();
  _timeManager.AddTask(TASK_LVL_WORLDMAP, true, 0, 0, 0, 1)->Interval.Connect(*_playerStats, &StatController::RunMetabolism);
  return (true);
}

void GameTask::OpenLevel(const std::string& savepath, const std::string& level, const std::string& entry_zone)
{
  std::ifstream fileTest;

  _worldMap->Hide();
  fileTest.open((savepath + "/" + level + ".blob").c_str());
  if (fileTest.is_open())
  {
    fileTest.close();
    LoadLevel(_window, _gameUi, savepath + "/" + level + ".blob", level, entry_zone, true);
  }
  else
    LoadLevel(_window, _gameUi, "maps/" + level + ".blob",        level, entry_zone, false);
}

void GameTask::ExitLevel(const std::string& savepath)
{
  _level->StripParty(*_playerParty);
  if (_level->IsPersistent())
  {
    cout << "Level is persistent" << endl;
    if (!(SaveGame(savepath)))
      AlertUi::NewAlert.Emit(i18n::T("Fatal Error") + ": " + i18n::T("Cannot save level"));
  }
  _quest_manager->Finalize();
  delete _level;
  _level = 0;
  _worldMap->SetInterrupted(false);
}

bool GameTask::CopySave(const std::string& savepath, const std::string& slotPath)
{
  DataTree metadata;
  Data     data(&metadata);

  data["time"].Duplicate(_dataEngine["time"]);
  data["system"].Duplicate(_dataEngine["system"]);
  Utils::DirectoryCompressor::Compress(slotPath, savepath, [](const string& i) { return (i != "preview.png"); });
  Filesystem::FileCopy(savepath + "preview.png", slotPath + ".png");
  DataTree::Writers::JSON(data, slotPath + ".json");
  return (true);
}

void GameTask::EraseSlot(unsigned char slot)
{
  stringstream stream;
  string       dirname;
  Directory    dir;
  
  stream << _savePath << "/slots/slot-" << (int)slot;
  remove(stream.str().c_str());
  remove((stream.str() + ".png").c_str());
  remove((stream.str() + ".json").c_str());
  cout << "Clearing Slot: " << stream.str() << endl;
}

void GameTask::SaveToSlot(unsigned char slot)
{
  cout << "SaveToSlot Called" << endl;
  if (SaveGame(_savePath))
  {
    std::stringstream stream;

    stream << _savePath << "/slots/slot-" << (int)slot;
    EraseSlot(slot);
    CopySave(_savePath, stream.str());
  }
  if (_uiSaveGame) _uiSaveGame->Hide();
}

void GameTask::LoadLastState(void)
{
  if (_level)    delete _level;
  if (_worldMap) { _worldMap->Hide(); delete _worldMap; }
  FinishLoad();
}

void GameTask::LoadSlot(unsigned char slot)
{
  if (_level)    { delete _level; _level = 0; }
  if (_worldMap) { _worldMap->Hide(); delete _worldMap; _worldMap = 0; }
  
  // Clear original directory
  {
    Directory         dir;

    dir.OpenDir(_savePath);

    std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [](const Dirent& entry)
    {
      if (entry.d_type == DT_REG)
      {
        std::string dname = entry.d_name;
        remove(dname.c_str());
      }
    });
  }
  
  // Copy the saved files in the original directory
  {
    std::stringstream stream;

    stream << _savePath << "/slots/slot-" << (unsigned int)slot;
    Utils::DirectoryCompressor::Uncompress(stream.str(), _savePath);
  }
  
  if (_uiLoadGame) _uiLoadGame->Hide();
  FinishLoad();
}

void GameTask::FinishLoad(void)
{
  if (!(LoadGame(_savePath)))
  {
    AlertUi::NewAlert.Emit(i18n::T("There was nothing to load"));
    _continue = false;
  }
}

bool GameTask::SaveLevel(Level* level, const std::string& name)
{
  cout << "Saving level" << endl;
  Utils::Packet packet;
  std::ofstream file;

  level->SaveUpdateWorld();
  level->UnprocessAllCollisions();
  level->GetWorld()->Serialize(packet);
  level->Save(packet);
  level->ProcessAllCollisions();
  file.open(name.c_str(), std::ios::binary);
  if (file.is_open())
  {
    file.write(packet.raw(), packet.size());
    file.close();
  }
  else
  {
    std::cerr << "?? Failed to open file '" << name << "', save failed !!" << std::endl;
    return (false);
  }
  return (true);
}

void   GameTask::SetPlayerInventory(void)
{
  if (!_playerInventory)
  {
    Inventory& inventory = _level->GetPlayer()->GetInventory();
    Data       save      = _dataEngine["player"]["inventory"];

    inventory.SaveInventory(save);
    //save.Output();
    _playerInventory = new Inventory;
    _playerInventory->LoadInventory(save);
    _gameUi.GetInventory().SetInventory(*_playerInventory);
  }
  if (_level)
    _level->SetPlayerInventory(_playerInventory);
}

class LoadingTask : public Sync::MyThread
{
public:
  Sync::Signal<void> StartLoading;
  
private:
  void Run(void)
  {
    StartLoading.Emit();
    delete this;
  }
};

void GameTask::DoLoadLevel(LoadLevelParams params)
{
  ifstream file;
  string   name  = params.path;

  file.open(name.c_str(), std::ios::binary);
  if (file.is_open())
  {
    Utils::Packet packet(file);

    try
    {
      Level*   level = 0;

      level = new Level(params.name, _window, _gameUi, packet, _timeManager);
	  cout << "Level Loaded" << endl;
      SetLevel(level);
      if (params.isSaveFile)
        level->Load(packet);
      file.close();
    }
    catch (const char* error)
    {
      std::cerr << "?? Failed to load file !! (" << error << ")" << std::endl;
    }
  }
  else
    std::cerr << "?? File not found !!" << std::endl;
  if (_level)
  {
    _worldMap->Hide();
    cout << "Level loaded." << endl;
    MusicManager::Get()->Play(params.name);
    _levelName = params.name;
    _level->SetDataEngine(&_dataEngine);
    if (params.entry_zone != "")
      _level->InsertParty(*_playerParty);
    else
      cout << "No entry zone" << endl;
    if (_level->GetPlayer() != 0)
    {
      SetPlayerInventory();
      _level->InitPlayer();
      _level->GetPlayer()->SetStatistics(_charSheet, _playerStats);
      if (params.entry_zone == "")
        _level->FetchParty(*_playerParty);
      else
        _level->SetEntryZone(*_playerParty, params.entry_zone);
      SetLevel(_level);

      _level->obs.Connect(_pipbuck.VisibilityToggled, [this](bool visible)
      {
        _level->GetCamera().SetEnabledScroll(!visible);
      });
      _quest_manager->Initialize(_level);
    }
    else
    {
      delete _level;
      _level = 0;
      AlertUi::NewAlert.Emit("The characters couldn't be loaded properly.");
      _worldMap->Show();
    }
  }
  else
  {
    AlertUi::NewAlert.Emit("Cannot load level's map file.");
    cerr << "?? Can't open level !!" << endl;
    _worldMap->Show();
  }
}

void GameTask::LoadLevel(WindowFramework* window, GameUi& gameUi, const std::string& path, const std::string& name, const std::string& entry_zone, bool isSaveFile)
{
  LoadLevelParams params;
  
  params.name       = name;
  params.path       = path;
  params.isSaveFile = isSaveFile;
  params.entry_zone = entry_zone;
  SyncLoadLevel.Emit(params);
}

void GameTask::SetLevelEncounter(const Encounter& encounter)
{
  MapOpenLevel(encounter.GetMapName());
  if (_level)
  {
    _worldMap->Hide();
    _level->SetPersistent(encounter.IsSpecial());

    // Make a city out of the special encounter
    if (encounter.IsSpecial())
    {
      float x, y;

      _worldMap->GetCurrentPosition(x, y);
      _worldMap->AddCity(encounter.GetMapName(), x, y, 10);
      _worldMap->SetCityVisible(encounter.GetMapName());
    }
    // Spawn encounter if it contains people/critters
    else if (encounter.IsEvent())
    {
      _level->SpawnEnemies(encounter.GetEncounterName(), 1);
    }
  }
  SyncLoadLevel.Disconnect(obs_level_unpersistent);
}

void GameTask::MakeEncounter(int x, int y, bool is_event)
{
  Encounter encounter(_playerStats);

  try
  {
    _worldMap->SetInterrupted(true);
    encounter.SetAsEvent(is_event);
    encounter.Initialize(_dataEngine, _worldMap);
    if (is_event && encounter.IsDetectedByParty())
    {
      UiDialog* dialog = new UiDialog(_window, _gameUi.GetContext());

      if (encounter.IsSpecial())
        dialog->SetMessage(i18n::T("You discovered ") + i18n::T(encounter.GetMapName()) + i18n::T(". Do you want to go in ?"));
      else
        dialog->SetMessage(i18n::T("You encountered ") + i18n::T(encounter.GetEncounterName()) + i18n::T(". Do you want to go in ?"));
      dialog->AddChoice(i18n::T("Yes"), [this, encounter](Rocket::Core::Event&) { SetLevelEncounter(encounter);     });
      dialog->AddChoice(i18n::T("No"),  [this]           (Rocket::Core::Event&) { _worldMap->SetInterrupted(false); });
      dialog->Show();
      dialog->SetModal(true);
      _worldMap->SetInterrupted(true);
      _playerStats->AddExperience(50);
    }
    else
      SetLevelEncounter(encounter);
  }
  catch (const Encounter::NoAvailableEncounters)
  {
    _worldMap->SetInterrupted(false);
    cout << "[GameTask][DoCheckRandomEncounter] No available encounters at " << x << ',' << y << endl;
  }
}

ISampleInstance* GameTask::PlaySound(const std::string& name)
{
  if (_sound_manager.Require(name))
  {
    ISampleInstance* instance = _sound_manager.CreateInstance(name);

    instance->Start();
    return (instance);
  }
  return (0);
}
