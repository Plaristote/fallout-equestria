#include "gametask.hpp"
#include <iostream>

using namespace std;

LevelTask::LevelTask(WindowFramework* window, PT(RocketRegion) rocket) : _gameUi(window, rocket)
{
  _continue   = true;
  _window     = window;
  _level      = 0;
  _savePath   = "saves";
  _dataEngine.Load(_savePath + "/dataengine.json");
  _worldMap   = new WorldMap(window, &_gameUi, _dataEngine);
  _worldMap->GoToPlace.Connect(*this, &LevelTask::MapOpenLevel);
  _worldMap->Show();

  _uiSaveGame = 0;
  _uiLoadGame = 0;
  _gameUi.GetMenu().SaveClicked.Connect(*this, &LevelTask::SaveClicked);
  _gameUi.GetMenu().LoadClicked.Connect(*this, &LevelTask::LoadClicked);
  _gameUi.GetMenu().ExitClicked.Connect(*this, &LevelTask::Exit);
}

void LevelTask::SaveClicked(Rocket::Core::Event&)
{
  if (_uiSaveGame)
    delete _uiSaveGame;
  _uiSaveGame = new UiSave(_window, _gameUi.GetContext(), _savePath);
  _uiSaveGame->SaveToSlot.Connect(*this, &LevelTask::SaveToSlot);
  _uiSaveGame->Show();
}

void LevelTask::LoadClicked(Rocket::Core::Event&)
{
  if (_uiLoadGame)
    delete _uiLoadGame;
  _uiLoadGame = new UiLoad(_window, _gameUi.GetContext(), _savePath);
  _uiLoadGame->LoadSlot.Connect(*this, &LevelTask::LoadSlot);
  _uiLoadGame->Show();
}

LevelTask::~LevelTask()
{
  if (_uiSaveGame) delete _uiSaveGame;
  if (_uiLoadGame) delete _uiLoadGame;
  if (_level)      delete _level;
  if (_worldMap)   delete _worldMap;
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
      /*if (nextZone != "")
      {
	OpenLevel(_savePath, nextZone);
	if (_level)
	  _level->SetEntryZone(exitPoint);
      }*/
    }
    if (!_level && _worldMap)
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

  _worldMap    = new WorldMap(_window, &_gameUi, _dataEngine);
  _worldMap->GoToPlace.Connect(*this, &LevelTask::MapOpenLevel);
  
  if (!(currentLevel.Nil()) && currentLevel.Value() != "0")
  {
    _worldMap->Hide();
    LoadLevel(_window, _gameUi, savepath + "/" + currentLevel.Value() + ".blob", true);
  }
  else
    _worldMap->Show();
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
  Directory                          dir;
  Directory::Entries::const_iterator it, end;

  Directory::MakeDir(slotPath);
  dir.OpenDir(savepath);
  it  = dir.GetEntries().begin();
  end = dir.GetEntries().end();
  
  for (; it != end ; ++it)
  {
    const struct dirent& entry = *it;

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

void LevelTask::EraseSlot(unsigned char slot)
{
  std::stringstream stream;
  Directory         dir;
  
  stream << _savePath << "/slot-" << (int)slot;
  dir.OpenDir(stream.str());
  
  std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [](const struct dirent& entry)
  {
    remove(entry.d_name);
  });
}

void LevelTask::SaveToSlot(unsigned char slot)
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

void LevelTask::LoadSlot(unsigned char slot)
{
  if (_level)    delete _level;
  if (_worldMap) { _worldMap->Hide(); delete _worldMap; }
  
  // Clear original directory
  {
    Directory         dir;
    
    dir.OpenDir(_savePath);

    std::for_each(dir.GetEntries().begin(), dir.GetEntries().end(), [](const struct dirent& entry)
    {
      if (entry.d_type == DT_REG)
        remove(entry.d_name);
    });
  }
  
  // Copy the saved files in the original directory
  {
    std::stringstream stream;

    stream << _savePath << "/slot-" << (unsigned int)slot;
    CopySave(stream.str(), _savePath);
  }
  
  if (_uiLoadGame) _uiLoadGame->Hide();
  if (!(LoadGame(_savePath)))
  {
    // TODO Handle error while loading the game
  }
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
    _worldMap->Show();
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