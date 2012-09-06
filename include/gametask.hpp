#ifndef  GAMETASK_HPP
# define GAMETASK_HPP

# include "level/level.hpp"
# include "worldmap/worldmap.hpp"

class LevelTask
{
public:
  LevelTask(WindowFramework* window, PT(RocketRegion) rocket);  
  ~LevelTask();
  
  void                  MapOpenLevel(std::string name);
  void                  SetLevel(Level* level);
  AsyncTask::DoneStatus do_task();
  bool                  SaveGame(const std::string& savepath);
  bool                  LoadGame(const std::string& savepath);  
  bool                  OpenLevel(const std::string& savepath, const std::string& level);
  void                  ExitLevel(const std::string& savepath);
  bool                  CopySave(const std::string& savepath, const std::string& slotPath);
  
  // LEVEL EVENTS
  void                  LevelExitZone(const std::string& toLevel);
  void                  UiSaveGame(const std::string& slotPath);
  void                  UiLoadGame(const std::string& slotPath);

private:
  static bool           SaveLevel(Level* level, const std::string& name);
  Level*                LoadLevel(WindowFramework* window, GameUi& gameUi, const std::string& name, bool isSaveFile = false);  
  Level*                DoLoadLevel(void);

  WindowFramework*      _window;
  GameUi                _gameUi;
  DataEngine            _dataEngine;
  
  WorldMap*             _worldMap;
  
  std::string           _levelName;
  Level*                _level;

  std::string           _savePath;
  
  struct LoadLevelParams
  { LoadLevelParams() : doLoad(false) {} bool doLoad; std::string name; std::string path; bool isSaveFile; };

  LoadLevelParams       _loadLevelParams;
};

#endif