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
#include <loading_exception.hpp>
#include "scheduled_task.hpp"
#include "encounter_spawn.hpp"

using namespace std;

GameTask* GameTask::CurrentGameTask = 0;

extern PandaFramework* framework;

GameTask::GameTask(WindowFramework* window, GeneralUi& generalUi) : game_ui(window, generalUi.GetRocketRegion()),
                                                                    pipbuck(window, generalUi.GetRocketRegion()->get_context(), data_engine)
{
  CurrentGameTask  = this;
  _continue        = true;
  this->window     = window;
  item_index       = DataTree::Factory::JSON("data/objects.json");
  level            = 0;
  save_path        = OptionsManager::Get()["savepath"].Value();
  world_map        = 0;
  player_party     = 0;
  player_stats     = 0;
  quest_manager    = 0;
  loading_screen   = 0;
  game_ui.GetMenu().SaveClicked.Connect(*this, &GameTask::SaveClicked);
  game_ui.GetMenu().LoadClicked.Connect(*this, &GameTask::LoadClicked);
  game_ui.GetMenu().ExitClicked.Connect(*this, &GameTask::Exit);
  game_ui.GetMenu().OptionsClicked.Connect(generalUi.GetOptions(), &UiBase::FireShow);
  game_ui.OpenPipbuck.Connect(pipbuck, &UiBase::FireShow);

  SyncLoadLevel.SetDirect(false);
  SyncLoadLevel.Connect(*this, &GameTask::LoadLevel);
  _signals.push_back(&SyncLoadLevel);
}

GameTask::~GameTask()
{
  Cleanup();
  if (item_index) delete item_index;
  CurrentGameTask = 0;
}

void                  GameTask::SaveClicked(Rocket::Core::Event&)
{
  UiSave* ui_save = game_ui.OpenSavingInterface(save_path);

  // SaveToSlot signal is emitted from Rocket's callback. Saving from that thread
  // would cause a deadlock when taking the screenshot. The signal has to be delayed
  // using SetDirect(false) and the Sync::Signals object.
  _signals.push_back(&ui_save->SaveToSlot);
  _signals.push_back(&ui_save->Done);
  ui_save->SaveToSlot.SetDirect(false);
  ui_save->Done.SetDirect(false);
  ui_save->SaveToSlot.Connect(*this, &GameTask::SaveToSlot);
  ui_save->Done.Connect([this, ui_save]()
  {
    Sync::Signals& signals  = _signals;
    UiSave*        ui_save_ = ui_save;

    // This lambda will be called in the signals object. The cleanup must be run later.
    Executor::ExecuteLater([&signals, ui_save_]()
    {
      signals.remove(&ui_save_->SaveToSlot);
      signals.remove(&ui_save_->Done);
      delete ui_save_;
    });
  });
  ui_save->EraseSlot.Connect (*this, &GameTask::EraseSlot);
}

void                  GameTask::LoadClicked(Rocket::Core::Event&)
{
  UiLoad* ui_load = game_ui.OpenLoadingInterface(save_path);

  _signals.push_back(&ui_load->LoadSlot);
  _signals.push_back(&ui_load->Done);
  ui_load->LoadSlot.SetDirect(false);
  ui_load->LoadSlot.Connect (*this, &GameTask::LoadSlot);
  ui_load->EraseSlot.Connect(*this, &GameTask::EraseSlot);
  ui_load->Done.SetDirect(false);
  ui_load->Done.Connect([this, ui_load]()
  {
    Sync::Signals& signals  = _signals;
    UiLoad*        ui_load_ = ui_load;

    // This lambda will be called in the signals object. The cleanup must be run later.
    Executor::ExecuteLater([&signals, ui_load_]()
    {
      signals.remove(&ui_load_->LoadSlot);
      signals.remove(&ui_load_->Done);
      delete ui_load_;
    });
  });
}

void                  GameTask::GameOver(void)
{
  // TODO Implement Game Over Interface
  _continue = false;
}

void                  GameTask::Exit(Rocket::Core::Event&)
{
  UiDialog* dialog      = new UiDialog(window, game_ui.GetContext());

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

void GameTask::RunLevel(void)
{
  if (level->do_task() == AsyncTask::DS_done)
  {
    SetupLoadingScreen();
    Level::Exit  exit      = level->GetExit();

    ExitLevel();
    if (!(exit.ToWorldmap()))
      OpenLevel(exit.level, exit.zone);
    else
      RemoveLoadingScreen();
    SaveGame();
  }
  if (!level && world_map)
    world_map->Show();
}

AsyncTask::DoneStatus GameTask::do_task()
{
  if (!_continue)
    return (AsyncTask::DS_done);
  _signals.ExecuteRecordedCalls();

  time_manager.ExecuteTasks();
  if (player_stats && (int)(player_stats->GetData()["Variables"]["Hit Points"]) <= 0)
    GameOver();
  if (level)
    RunLevel();
  else if (world_map)
    world_map->Run();
  pipbuck.Run();
  return (AsyncTask::DS_cont);
}

bool GameTask::SaveGame()
{
  bool     success      = true;
  DateTime current_time = time_manager.GetDateTime();

  world_map->Save(save_path);
  if (level)
  {
    data_engine["system"]["current-level"] = level->GetName();
    success = success && SaveLevel(level, save_path + "/" + level->GetName() + ".blob");
  }
  else
    data_engine["system"]["current-level"] = 0;
  player_party->Save(save_path);
  data_engine["time"]["seconds"] = current_time.GetSecond();
  data_engine["time"]["minutes"] = current_time.GetMinute();
  data_engine["time"]["hours"]   = current_time.GetHour();
  data_engine["time"]["days"]    = current_time.GetDay();
  data_engine["time"]["month"]   = current_time.GetMonth();
  data_engine["time"]["year"]    = current_time.GetYear();
  data_engine.Save(save_path + "/dataengine.json");

  if (level != 0)
  {
    UiBase::ToggleUserInterface.Emit(false);
    window->get_render().set_transparency(TransparencyAttrib::M_alpha, 1);
    framework->get_graphics_engine()->render_frame();
    window->get_graphics_window()->get_screenshot()->write(save_path + "/preview.png");
    UiBase::ToggleUserInterface.Emit(true);
  }
  return (success);
}

void GameTask::Cleanup(void)
{
  if (quest_manager) delete quest_manager;
  if (level)
  {
      cout << "Dynamic object count: " << level->GetWorld()->dynamicObjects.size() << endl;
    if (player_party)
      level->RemovePartyFromLevel(*player_party);
    delete level;
  }
  if (world_map)     delete world_map;
  if (player_party)  delete player_party;
  quest_manager = 0;
  level         = 0;
  world_map     = 0;
  player_party  = 0;
  time_manager.ClearTasks(0);
}

void GameTask::LoadDataEngine(void)
{
  Data time;

  data_engine.Load(save_path + "/dataengine.json");
  time             = data_engine["time"];
  time_manager.SetTime(time["seconds"], time["minutes"], time["hours"], time["days"], time["month"], time["year"]);
  pipbuck.Restart();
  LoadingScreen::AppendText("The time is " + time_manager.GetDateTime().ToString());
}

void GameTask::LoadPlayerData(void)
{
  LoadingScreen::AppendText("Loading party");
  player_party     = new PlayerParty(save_path);
  player_stats     = player_party->GetPlayerController();
  quest_manager    = new QuestManager(data_engine, player_stats);
  player_stats->SetView(&(game_ui.GetPers()));
  game_ui.GetInventory().SetInventory(*(player_party->GetPlayerInventory()));
}

void GameTask::LoadWorldMap(void)
{
  world_map    = new WorldMap(window, &game_ui, data_engine, time_manager);
  world_map->GoToPlace.Connect   ([this](const std::string& level)                          { OpenLevel(level, "worldmap"); });
  world_map->GoToCityZone.Connect([this](const std::string& level, const std::string& zone) { OpenLevel(level, zone);       });
  world_map->RequestRandomEncounter.Connect(*this, &GameTask::MakeEncounter);
}

bool GameTask::LoadGame()
{
  SetupLoadingScreen();
  Cleanup();
  try
  {
    Data current_level;
    Data loading_level;

    LoadDataEngine();
    LoadingScreen::SetBackground(data_engine["system"]["current-level"].Value());
    LoadPlayerData();
    LoadWorldMap();
    loading_level = data_engine["system"]["loading-level"];
    current_level = loading_level.Nil() ? data_engine["system"]["current-level"] : loading_level["level-name"];
    if (!(current_level.Nil()) && current_level.Value() != "0")
    {
      LoadLevelParams params;
      
      params.name       = current_level.Value();
      params.path       = save_path + '/' + params.name + ".blob";
      params.entry_zone = loading_level.Nil() ? "" : loading_level["entry-zone"].Value();
      params.isSaveFile = loading_level.Nil() || Filesystem::FileExists(params.path);
      if (params.isSaveFile == false)
        params.path     = "maps/" + params.name + ".blob";
      SyncLoadLevel.Emit(params);
      world_map->Hide();
    }
    else
    {
      world_map->Show();
      RemoveLoadingScreen();
    }
    loading_level.Remove();
    time_manager.AddRepetitiveTask(TASK_LVL_WORLDMAP, DateTime::Days(1))->Interval.Connect(*player_stats, &StatController::RunMetabolism);
  }
  catch (LoadingException exception)
  {
    RemoveLoadingScreen();
    exception.Display();
    return (false);
  }
  return (true);
}

void GameTask::OpenLevel(const std::string& level_name, const std::string& entry_zone)
{
  std::ifstream   fileTest;
  std::string     filename = level_name + ".blob";
  LoadLevelParams params;

  data_engine["system"]["loading-level"]["level-name"] = level_name;
  data_engine["system"]["loading-level"]["entry-zone"] = entry_zone;
  params.name       = level_name;
  params.entry_zone = entry_zone;
  params.isSaveFile = Filesystem::FileExists(save_path + '/' + filename);
  if (params.isSaveFile)
    params.path     = save_path + '/' + filename;
  else
    params.path     = "maps/" + filename;
  SetupLoadingScreen();
  SyncLoadLevel.Emit(params);
}

void GameTask::ExitLevel()
{
  level->RemovePartyFromLevel(*player_party);
  if (level->IsPersistent())
    SaveLevel(level, save_path + "/" + level->GetName() + ".blob");
  quest_manager->Finalize();
  delete level;
  level = 0;
  world_map->SetInterrupted(false);
}

void GameTask::EraseSlot(unsigned char slot)
{
  stringstream stream;
  string       dirname;
  Directory    dir;
  
  stream << save_path << "/slots/slot-" << (int)slot;
  remove(stream.str().c_str());
  remove((stream.str() + ".png").c_str());
  remove((stream.str() + ".json").c_str());
}

void GameTask::SaveToSlot(unsigned char slot)
{
  if (SaveGame())
  {
    if (!Directory::Exists(save_path + "/slots") && !Directory::MakeDir(save_path + "/slots"))
      AlertUi::NewAlert.Emit("Failed to save: cannot access directory " + save_path + "/slots");
    else
    {
      std::stringstream stream;

      stream << save_path << "/slots/slot-" << (int)slot;
      EraseSlot(slot);
      CompressSave(stream.str());
    }
  }
}

bool GameTask::CompressSave(const std::string& slotPath)
{
  DataTree metadata;
  Data     data(&metadata);

  try
  {
    data["time"].Duplicate(data_engine["time"]);
    data["system"].Duplicate(data_engine["system"]);
    Utils::DirectoryCompressor::Compress(slotPath, save_path, [](const string& i) { return (i != "preview.png"); });
    Filesystem::FileCopy(save_path + "preview.png", slotPath + ".png");
    DataTree::Writers::JSON(data, slotPath + ".json");
  }
  catch (const std::exception& exception)
  {
    AlertUi::NewAlert.Emit("Failed to save: " + std::string(exception.what()));
    return (false);
  }
  return (true);
}

void GameTask::LoadSlot(unsigned char slot)
{
  std::stringstream slot_path;

  slot_path << save_path << "/slots/slot-" << (unsigned int)slot;
  RemoveCurrentProgression();
  try
  {
    Utils::DirectoryCompressor::Uncompress(slot_path.str(), save_path);
    LoadGame();
  }
  catch (const std::exception& exception)
  {
    AlertUi::NewAlert.Emit("Failed to load: " + std::string(exception.what()));
    if (world_map == 0 && level == 0) // If nothin's running, go back to main menu
      _continue = 0;
  }
}

void GameTask::RemoveCurrentProgression(void)
{
  Directory         dir;

  dir.OpenDir(save_path);

  std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [](const Dirent& entry)
  {
    if (entry.d_type == DT_REG)
    {
      std::string dname = entry.d_name;
      remove(dname.c_str());
    }
  });
}

bool GameTask::SaveLevel(Level* level, const std::string& name)
{
  Utils::Packet packet;
  std::ofstream file;

  level->Serialize(packet);
  file.open(name.c_str(), std::ios::binary);
  if (file.is_open())
  {
    file.write(packet.raw(), packet.size());
    file.close();
  }
  else
  {
    AlertUi::NewAlert.Emit("Failed to save level: couldn't open file '" + name + "'.");
    return (false);
  }
  return (true);
}

extern bool world_is_game_save;

void GameTask::LoadLevelFromPacket(LoadLevelParams params, Utils::Packet& packet)
{
  LoadingScreen::AppendText("Entering " + params.name);
  world_is_game_save = params.isSaveFile;
  level = new Level(params.name, window, game_ui, packet, time_manager);
  if (params.isSaveFile)
    level->Unserialize(packet);
  level->SetDataEngine(&data_engine);
  if (params.entry_zone != "")
    level->InsertParty(*player_party, params.entry_zone);
  else
    level->MatchPartyToExistingCharacters(*player_party);
  level->obs.Connect(pipbuck.VisibilityToggled, level->GetLevelUi().InterfaceOpened, &Sync::Signal<void (bool)>::Emit);
  quest_manager->Initialize(level);
  world_map->Hide();
  data_engine["system"]["loading-level"].Remove();
}

void GameTask::LoadLevel(LoadLevelParams params)
{
  ifstream file;
  bool     success = false;

  file.open(params.path.c_str(), std::ios::binary);
  if (file.is_open())
  {
    Utils::Packet packet(file);

    file.close();
    try
    {
      LoadLevelFromPacket(params, packet);
      success = true;
    }
    catch (LoadingException exception)
    {
      exception.SetName(params.name);
      exception.Display();
    }
    catch (const std::exception& exception)
    {
      AlertUi::NewAlert.Emit("Failed to load level (" + std::string(exception.what()) + ")");
    }
    catch (const char* error)
    {
      AlertUi::NewAlert.Emit("Failed to load level (" + std::string(error) + ")");
    }
  }
  else
    AlertUi::NewAlert.Emit("Failed to open map file '" + params.path + '\'');
  RemoveLoadingScreen();
  if (success == false)
  {
    if (level) { delete level; level = 0; }
    world_map->Show();
  }
}

void GameTask::SetLevelEncounter(const Encounter& encounter)
{
  OpenLevel(encounter.GetMapName(), "worldmap");
  SyncLoadLevel.Connect([this, encounter](LoadLevelParams)
  {
    if (level)
    {
      world_map->Hide();
      level->SetPersistent(encounter.IsSpecial());

      // Make a city out of the special encounter
      if (encounter.IsSpecial())
      {
        float x, y;

        world_map->GetCurrentPosition(x, y);
        world_map->AddCity(encounter.GetMapName(), x, y, 10);
        world_map->SetCityVisible(encounter.GetMapName());
      }
      // Spawn encounter if it contains people/critters
      else if (encounter.IsEvent())
      {
        EncounterSpawn* spawn = new EncounterSpawn(encounter.GetEncounterName());
        stringstream    entry_zone;

        entry_zone << "spawn_" << 1;
        spawn->LoadFromDataEngine(data_engine);
        level->InsertParty(*spawn, entry_zone.str());
      }
    }

    auto& _SyncLoadLevel = SyncLoadLevel;
    auto* self           = this;
    Executor::ExecuteLater([&_SyncLoadLevel, self]()
    {
      _SyncLoadLevel.DisconnectAll();
      _SyncLoadLevel.Connect(*self, &GameTask::LoadLevel);
    });
  });
}

void GameTask::MakeEncounter(int x, int y, bool is_event)
{
  Encounter encounter(player_stats);

  try
  {
    world_map->SetInterrupted(true);
    encounter.SetAsEvent(is_event);
    encounter.Initialize(data_engine, world_map);
    if (is_event && encounter.IsDetectedByParty())
    {
      UiDialog* dialog = new UiDialog(window, game_ui.GetContext());

      if (encounter.IsSpecial())
        dialog->SetMessage(i18n::T("You discovered ") + i18n::T(encounter.GetMapName()) + i18n::T(". Do you want to go in ?"));
      else
        dialog->SetMessage(i18n::T("You encountered ") + i18n::T(encounter.GetEncounterName()) + i18n::T(". Do you want to go in ?"));
      dialog->AddChoice(i18n::T("Yes"), [this, encounter](Rocket::Core::Event&) { SetLevelEncounter(encounter);     });
      dialog->AddChoice(i18n::T("No"),  [this]           (Rocket::Core::Event&) { world_map->SetInterrupted(false); });
      dialog->Show();
      dialog->SetModal(true);
      world_map->SetInterrupted(true);
      player_stats->AddExperience(50);
    }
    else
      SetLevelEncounter(encounter);
  }
  catch (const Encounter::NoAvailableEncounters)
  {
    world_map->SetInterrupted(false);
    cout << "[GameTask][DoCheckRandomEncounter] No available encounters at " << x << ',' << y << endl;
  }
}

ISampleInstance* GameTask::PlaySound(const std::string& name)
{
  if (sound_manager.Require(name))
  {
    ISampleInstance* instance = sound_manager.CreateInstance(name);

    instance->Start();
    return (instance);
  }
  return (0);
}

void GameTask::SetupLoadingScreen(void)
{
  if (!loading_screen)
  {
    loading_screen = new LoadingScreen(window, game_ui.GetContext());
  }
}

void GameTask::RemoveLoadingScreen(void)
{
  if (loading_screen)
  {
    delete loading_screen;
    loading_screen = 0;
  }
}

