#ifndef  QUEST_MANAGER_HPP
# define QUEST_MANAGER_HPP

# include "datatree.hpp"
# include "observatory.hpp"
# include "dataengine.hpp"

class Level;
class StatController;

class Quest
{
public:
  Data                             data;
  Sync::Signal<void (Data)>        ObjectiveCompleted;
  Sync::Signal<void (Quest*)>      QuestCompleted;

  void Initialize(Level* level);

  void Finalize(void)
  {
    _observers.DisconnectAll();
  }

private:
  typedef void (Quest::*WatcherInitializerMethod)(Data, Level*);

  struct WatcherInitializer
  {
    std::string              key;
    WatcherInitializerMethod method;
  };

  bool CheckIfCompleted(void);
  void InitializeCondition(Data condition, Level* level);
  // Watcher Initialization
  void WatcherCharacterInventory(Data condition, Level* level);
  void WatcherCharacterKill(Data condition, Level* level);
  void WatcherTime(Data condition, Level* level);

  Sync::ObserverHandler _observers;
};

class QuestManager
{
  typedef std::list<Quest*> Quests;
public:
  QuestManager(DataEngine& de, StatController* player_controller);

  ~QuestManager()
  {
    for_each(_quests.begin(), _quests.end(), [](Quest* quest) { delete quest; });
  }

  void   AddQuest(Data);

  void   Initialize(Level* level)
  {
    _level = level;
    for_each(_quests.begin(), _quests.end(), [level](Quest* quest) { quest->Initialize(level); });
  }
  
  void   Finalize(void)
  {
    _level = 0;
    for_each(_quests.begin(), _quests.end(), [](Quest* quest)      { quest->Finalize(); });
  }
  
  Sync::Signal<void>          QuestsUpdated;
  Sync::Signal<void (Quest*)> QuestUpdated;

private:
  void   QuestCompleted(Quest*);

  DataEngine&     _data_engine;
  Quests          _quests;
  StatController* _stats_controller;
  Level*          _level;
};

#endif
