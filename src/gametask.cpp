#include "gametask.hpp"
#include <iostream>

using namespace std;

GameTask::GameTask(WindowFramework* window, GeneralUi& generalUi) : _gameUi(window, generalUi.GetRocketRegion())
{
  _continue        = true;
  _window          = window;
  _level           = 0;
  _savePath        = "saves";
  _worldMap        = 0;
  _charSheet       = 0;
  _playerStats     = 0;
  _playerInventory = 0;
  _uiSaveGame      = 0;
  _uiLoadGame      = 0;
  _gameUi.GetMenu().SaveClicked.Connect(*this, &GameTask::SaveClicked);
  _gameUi.GetMenu().LoadClicked.Connect(*this, &GameTask::LoadClicked);
  _gameUi.GetMenu().ExitClicked.Connect(*this, &GameTask::Exit);
  _gameUi.GetMenu().OptionsClicked.Connect(generalUi.GetOptions(), &UiBase::FireShow);
}

GameTask::~GameTask()
{
  if (_playerStats) { delete _playerStats; }
  if (_charSheet)   { delete _charSheet;   }
  if (_uiSaveGame)  { _uiSaveGame->Destroy(); delete _uiSaveGame; }
  if (_uiLoadGame)  { _uiLoadGame->Destroy(); delete _uiLoadGame; }
  if (_worldMap)    { _worldMap->Destroy();   delete _worldMap;   }
  if (_level)       { delete _level;       }
}

void GameTask::SaveClicked(Rocket::Core::Event&)
{
  if (_uiSaveGame)
    delete _uiSaveGame;
  _uiSaveGame = new UiSave(_window, _gameUi.GetContext(), _savePath);
  _uiSaveGame->SaveToSlot.Connect(*this, &GameTask::SaveToSlot);
  _uiSaveGame->Show();
}

void GameTask::LoadClicked(Rocket::Core::Event&)
{
  if (_uiLoadGame)
    delete _uiLoadGame;
  _uiLoadGame = new UiLoad(_window, _gameUi.GetContext(), _savePath);
  _uiLoadGame->LoadSlot.Connect(*this, &GameTask::LoadSlot);
  _uiLoadGame->Show();
}

void       GameTask::MapOpenLevel(std::string name)
{
  OpenLevel(_savePath, name);
  _loadLevelParams.entry_zone = "worldmap";
}

void       GameTask::SetLevel(Level* level)
{
  _level = level;
}

AsyncTask::DoneStatus GameTask::do_task()
{
  if (!_continue)
    return (AsyncTask::DS_done);
  if (_loadLevelParams.doLoad)
    DoLoadLevel();
  if (_level)
  {
    if (_level->do_task() == AsyncTask::DoneStatus::DS_done)
    {
      const string nextZone  = _level->GetNextZone();
      const string exitPoint = _level->GetExitZone();

      ExitLevel(_savePath);
      if (nextZone != "")
      {
	OpenLevel(_savePath, nextZone);
	_loadLevelParams.entry_zone = exitPoint;
      }
    }
    if (!_level && _worldMap)
      _worldMap->Show();
  }
  else if (_worldMap)
    _worldMap->Run();
  return (AsyncTask::DoneStatus::DS_cont);
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

  return (success);
}

bool GameTask::LoadGame(const std::string& savepath)
{
  cout << "LoadGame 1" << endl;
  Data currentLevel, time;

  if (_worldMap)    delete _worldMap;
  if (_playerStats) delete _playerStats;
  if (_charSheet)   delete _charSheet;
  if (_playerInventory)
  {
    delete _playerInventory;
    _playerInventory = 0;
  }

  cout << "LoadGame 2" << endl;
  _dataEngine.Load(savepath + "/dataengine.json");
  currentLevel     = _dataEngine["system"]["current-level"];
  time             = _dataEngine["time"];
  cout << "LoadGame 3" << endl;
  _timeManager.ClearTasks(0);
  _timeManager.SetTime(time["seconds"], time["minutes"], time["hours"], time["days"], time["month"], time["year"]);
  _charSheet       = DataTree::Factory::JSON(savepath + "/stats-self.json");
  cout << "LoadGame 4" << endl;
  if (!_charSheet)  return (false);
  cout << "LoadGame 5" << endl;
  _playerStats     = new StatController(_charSheet);
  cout << "LoadGame 6" << endl;
  _playerStats->SetView(&(_gameUi.GetPers()));
  cout << "Lol" << endl;
  if (_dataEngine["player"]["inventory"].NotNil())
  {
    _playerInventory = new Inventory;
    cout << "Double lol" << endl;
    _playerInventory->LoadInventory(_dataEngine["player"]["inventory"]);
    cout << "Triple lol" << endl;
    _gameUi.GetInventory().SetInventory(*_playerInventory);
  }

  _worldMap    = new WorldMap(_window, &_gameUi, _dataEngine, _timeManager);
  _worldMap->GoToPlace.Connect(*this, &GameTask::MapOpenLevel);
  
  if (!(currentLevel.Nil()) && currentLevel.Value() != "0")
  {
    _worldMap->Hide();
    LoadLevel(_window, _gameUi, savepath + "/" + currentLevel.Value() + ".blob", currentLevel.Value(), true);
  }
  else
    _worldMap->Show();
  return (true);
}

void GameTask::OpenLevel(const std::string& savepath, const std::string& level)
{
  std::ifstream fileTest;

  _worldMap->Hide();
  fileTest.open((savepath + "/" + level + ".blob").c_str());
  if (fileTest.is_open())
  {
    fileTest.close();
    _level = LoadLevel(_window, _gameUi, savepath + "/" + level + ".blob", level, true);
  }
  else
    _level = LoadLevel(_window, _gameUi, "maps/" + level + ".blob", level, false);
}

void GameTask::ExitLevel(const std::string& savepath)
{
  if (!(SaveGame(savepath)))
  {
    cerr << "!! Couldn't save level state on ExitLevel !!" << endl;
  }
  delete _level;
  _level = 0;
  cout << "Exited Level" << endl;
}

bool GameTask::CopySave(const std::string& savepath, const std::string& slotPath)
{
  // Copy the savepath directory to the slotpath directory
  Directory                          dir;
  Directory::Entries::const_iterator it, end;

  Directory::MakeDir(slotPath);
  dir.OpenDir(savepath);
  it  = dir.GetEntries().begin();
  end = dir.GetEntries().end();
  
  for (; it != end ; ++it)
  {
    const Dirent& entry = *it;

    if (entry.d_type == DT_REG)
    {
      // Copy file to the slot
      std::string   ifilePath, ofilePath;
      std::ifstream ifile;
      std::ofstream ofile;

      ifilePath = savepath + "/" + entry.d_name;
      ofilePath = slotPath + "/" + entry.d_name;
      ifile.open(ifilePath.c_str());
      ofile.open(ofilePath.c_str());
      if (ifile.is_open() && ofile.is_open())
      {
	long  begin, end;
	long  size;
	char* raw;

	begin     = ifile.tellg();
	ifile.seekg (0, std::ios::end);
	end       = ifile.tellg();
	ifile.seekg(0, std::ios::beg);
	size      = end - begin;
	raw       = new char[size + 1];
	raw[size] = 0;
	ifile.read(raw, size);
	ifile.close();

	ofile.write(raw, size);
	ofile.close();

	delete[] raw;
      }
      else
      {
	// Can't copy file...
	return (false);
      }
    }
  }
  return (true);
}

void GameTask::EraseSlot(unsigned char slot)
{
  std::stringstream stream;
  Directory         dir;
  
  stream << _savePath << "/slot-" << (int)slot;
  dir.OpenDir(stream.str());
  
  std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [](const Dirent& entry)
  {
	std::string to_remove = entry.d_name;
    remove(to_remove.c_str());
  });
}

void GameTask::SaveToSlot(unsigned char slot)
{
  if (SaveGame(_savePath))
  {
    std::stringstream stream;

    stream << _savePath << "/slot-" << (int)slot;
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

    stream << _savePath << "/slot-" << (unsigned int)slot;
    CopySave(stream.str(), _savePath);
  }
  
  if (_uiLoadGame) _uiLoadGame->Hide();
  FinishLoad();
}

void GameTask::FinishLoad(void)
{
  if (!(LoadGame(_savePath)))
  {
    // TODO Handle error while loading the game
    cout << "[NOT IMPLEMENTED] GameTask::FinishLoad -> LoadGame Failure" << endl;
  }
}

// LEVEL EVENTS
void GameTask::UiSaveGame(const std::string& slotPath)
{
  SaveGame(_savePath);
  CopySave(_savePath, slotPath);
}

void GameTask::UiLoadGame(const std::string& slotPath)
{
  CopySave(_savePath, slotPath);
  LoadGame(_savePath);
}

bool GameTask::SaveLevel(Level* level, const std::string& name)
{
  Utils::Packet packet;
  std::ofstream file;

  level->SaveUpdateWorld();
  level->UnprocessAllCollisions();
  level->GetWorld()->Serialize(packet);
  level->Save(packet);
  level->ProcessAllCollisions();
  file.open(name.c_str(), std::ios::binary);
  if (file.is_open())
    file.write(packet.raw(), packet.size());
  else
  {
    std::cerr << "　 Failed to open file '" << name << "', save failed !!" << std::endl;
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
    save.Output();
    _playerInventory = new Inventory;
    _playerInventory->LoadInventory(save);
    _gameUi.GetInventory().SetInventory(*_playerInventory);
  }
  _level->SetPlayerInventory(_playerInventory);
}

Level* GameTask::DoLoadLevel(void)
{
  std::cout << "DoLoadLevel" << std::endl;
  Level*        level = 0;
  std::ifstream file;
  std::string   name  = _loadLevelParams.path;
  
  file.open(name.c_str(), std::ios::binary);
  if (file.is_open())
  {
    Utils::Packet packet(file);

    try
    {
      level = new Level(_window, _gameUi, packet, _timeManager);
      if (_loadLevelParams.isSaveFile)
	level->Load(packet);
      SetLevel(level);
      file.close();
    }
    catch (const char* error)
    {
      std::cerr << "　 Failed to load file !! (" << error << ")" << std::endl;
      level = 0;
    }
  }
  else
    std::cerr << "　 File not found !!" << std::endl;
  if (_level)
  {
    _levelName = _loadLevelParams.name;
    _level->SetDataEngine(&_dataEngine);
    _level->GetPlayer()->SetStatistics(_charSheet, _playerStats);
    SetPlayerInventory();
    _level->SetEntryZone(_loadLevelParams.entry_zone);
    SetLevel(_level);
  }
  else
  {
    cerr << "　 Can't open level !!" << endl;
    _worldMap->Show();
  }
  _loadLevelParams.entry_zone = "";
  _loadLevelParams.doLoad     = false;
  return (level);  
}

Level* GameTask::LoadLevel(WindowFramework* window, GameUi& gameUi, const std::string& path, const std::string& name, bool isSaveFile)
{
  _loadLevelParams.name       = name;
  _loadLevelParams.path       = path;
  _loadLevelParams.isSaveFile = isSaveFile;
  _loadLevelParams.doLoad     = true;
  return (0);
}