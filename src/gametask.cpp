#include "gametask.hpp"
#include <iostream>

using namespace std;

LevelTask::LevelTask(WindowFramework* window, PT(RocketRegion) rocket) : _gameUi(window, rocket)
{
  _window   = window;
  _level    = 0;
  _savePath = "saves";
  _dataEngine.Load(_savePath + "/dataengine.json");
  _worldMap = new WorldMap(window, &_gameUi, _dataEngine);
  _worldMap->GoToPlace.Connect(*this, &LevelTask::MapOpenLevel);
  _worldMap->Show();
}

LevelTask::~LevelTask()
{
  if (_level)    delete _level;
  if (_worldMap) delete _worldMap;
}

void       LevelTask::MapOpenLevel(std::string name)
{
  OpenLevel(_savePath, name);
}

void       LevelTask::SetLevel(Level* level)
{
  _level = level;
}

AsyncTask::DoneStatus LevelTask::do_task()
{
  if (_loadLevelParams.doLoad)
    DoLoadLevel();
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
      _worldMap->Show();
  }
  else
    _worldMap->Run();
  return (AsyncTask::DoneStatus::DS_cont);
}

bool LevelTask::SaveGame(const std::string& savepath)
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

bool LevelTask::LoadGame(const std::string& savepath)
{
  Data currentLevel;

  _dataEngine.Load(savepath + "/dataengine.json");
  currentLevel = _dataEngine["system"]["current-level"];
  if (!(currentLevel.Nil()) && currentLevel.Value() != "0")
  {
    LoadLevel(_window, _gameUi, savepath + "/" + currentLevel.Value() + ".blob", true);
  }
}

bool LevelTask::OpenLevel(const std::string& savepath, const std::string& level)
{
  std::ifstream fileTest;

  _worldMap->Hide();
  fileTest.open((savepath + "/" + level + ".blob").c_str());
  if (fileTest.is_open())
  {
    fileTest.close();
    _level = LoadLevel(_window, _gameUi, savepath + "/" + level + ".blob", true);
  }
  else
    _level = LoadLevel(_window, _gameUi, "maps/" + level + ".blob", false);
  return (_level != 0);
}

void LevelTask::ExitLevel(const std::string& savepath)
{
  /*if (!(SaveGame(savepath)))
  {
    cerr << "¡¡ Couldn't save level state on ExitLevel !!" << endl;
  }*/
  // TODO Find why level destruction makes panda3d crash
  // TODO TODO Find out if level destruction still makes panda3d crash
  delete _level;
  _level = 0;
  cout << "Exited Level" << endl;
}

bool LevelTask::CopySave(const std::string& savepath, const std::string& slotPath)
{
  // Copy the savepath directory to the slotpath directory
}

// LEVEL EVENTS
void LevelTask::LevelExitZone(const std::string& toLevel)
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

void LevelTask::UiSaveGame(const std::string& slotPath)
{
  SaveGame(_savePath);
  CopySave(_savePath, slotPath);
}

void LevelTask::UiLoadGame(const std::string& slotPath)
{
  CopySave(_savePath, slotPath);
  LoadGame(_savePath);
}

bool LevelTask::SaveLevel(Level* level, const std::string& name)
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

Level* LevelTask::DoLoadLevel(void)
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
      level = new Level(_window, _gameUi, packet);
      if (_loadLevelParams.isSaveFile)
	level->Load(packet);
      SetLevel(level);
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
  if (_level)
  {
    // WARNING This is only temporary
    _dataEngine.Load(_savePath + "/dataengine.json");

    _levelName = _loadLevelParams.name;
    _level->SetDataEngine(&_dataEngine);
    SetLevel(_level);
  }
  else
  {
    cerr << "¡¡ Can't open level !!" << endl;
    _worldMap->Show();
  }
  _loadLevelParams.doLoad = false;
  return (level);  
}

Level* LevelTask::LoadLevel(WindowFramework* window, GameUi& gameUi, const std::string& name, bool isSaveFile)
{
  _loadLevelParams.path       = name;
  _loadLevelParams.isSaveFile = isSaveFile;
  _loadLevelParams.doLoad     = true;
  return (0);
}