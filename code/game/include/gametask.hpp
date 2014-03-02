#ifndef  GAMETASK_HPP
# define GAMETASK_HPP

# include "globals.hpp"
# include "level/level.hpp"
# include "worldmap/worldmap.hpp"
# include "playerparty.hpp"
# include "ui/general_ui.hpp"
# include "ui/saveuis.hpp"
# include "cmap/statcontroller.hpp"
# include "pipbuck/pipbuck.hpp"
# include "encounter.hpp"

class QuestManager;

class GameTask
{
  struct LoadLevelParams
  { LoadLevelParams() {} std::string name; std::string path; bool isSaveFile; std::string entry_zone; };
public:
  static GameTask* CurrentGameTask;
  GameTask(WindowFramework* window, GeneralUi&);
  ~GameTask();
  
  AsyncTask::DoneStatus do_task(void);
  bool                  LoadGame(void);  
  bool                  SaveGame(void);
  void                  OpenLevel(const std::string& level, const std::string& entry_zone = "");
  void                  ExitLevel(void);

  void                  SaveToSlot(unsigned char slot);
  void                  LoadSlot(unsigned char slot);
  void                  Exit(Rocket::Core::Event&);

  void                  SetLevelEncounter(const Encounter&);

  // Getters
  GameUi&               GetGameUi(void)         { return (game_ui);                                }
  DataEngine&           GetDataEngine(void)     { return (data_engine);                            }
  Data                  GetItemIndex(void)      { return (item_index ? Data(item_index) : Data()); }
  TimeManager&          GetTimeManager(void)    { return (time_manager);                           }
  WorldDiplomacy&       GetDiplomacy(void)      { return (data_engine.GetDiplomacy());             }
  QuestManager&         GetQuestManager(void)   { return (*quest_manager);                         }
  Party*                GetPlayerParty(void)    { return (player_party);                           }
  const std::string&    GetSavePath(void) const { return (save_path);                              }

  ISampleInstance*      PlaySound(const std::string&);

private:
  void                  RemoveCurrentProgression(void);
  void                  LoadClicked(Rocket::Core::Event&);
  void                  SaveClicked(Rocket::Core::Event&);
  static bool           SaveLevel(Level* level, const std::string& name);
  void                  LoadLevel(WindowFramework* window, GameUi& gameUi, const std::string& path, const std::string& name, const std::string& entry_zone = "", bool isSaveFile = false);
  void                  LoadLevelFromPacket(LoadLevelParams, Utils::Packet&);
  void                  DoLoadLevel(LoadLevelParams);
  void                  GameOver(void);
  void                  RunLevel(void);
  
  void                  MakeEncounter(int x, int y, bool is_event = true);

  void                  Cleanup(void);
  void                  LoadDataEngine(void);
  void                  LoadPlayerData(void);
  void                  LoadWorldMap(void);
  void                  SetPlayerInventory(void);

  void                  EraseSlot(unsigned char slot);
  bool                  CompressSave(const std::string& output_path);

  Sync::Signals         _signals;
  bool                  _continue;
  WindowFramework*      window;
  std::string           save_path;
  SoundManager          sound_manager;
  GameUi                game_ui;
  DataEngine            data_engine;
  DataTree*             item_index;
  TimeManager           time_manager;
  Pipbuck               pipbuck;
  PlayerParty*          player_party;
  StatController*       player_stats;
  QuestManager*         quest_manager;

  WorldMap*             world_map;
  Level*                level;

  Sync::Signal<void (LoadLevelParams)> SyncLoadLevel;
};

#endif

