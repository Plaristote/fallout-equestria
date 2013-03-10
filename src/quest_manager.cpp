#include "quest_manager.hpp"
#include "level/level.hpp"
#include "statsheet.hpp"
#include <gametask.hpp>
#include <algorithm>

using namespace std;

/*
 * QuestManager
 */
QuestManager::QuestManager(DataEngine& de, StatController* player_controller) : _data_engine(de), _stats_controller(player_controller)
{
  Data quests = de["Quests"];
  
  _level = 0;
  for_each(quests.begin(), quests.end(), [this](Data data_quest)
  {
    Quest* quest = new Quest;
    
    quest->data = data_quest;
    quest->QuestCompleted.Connect(*this, &QuestManager::QuestCompleted);
    _quests.push_back(quest);
  });
}

void QuestManager::AddQuest(Data data)
{
  Quest* quest = new Quest;
  
  _data_engine["Quests"][data.Key()].Duplicate(data);
  quest->data = _data_engine["Quests"][data.Key()];
  quest->QuestCompleted.Connect(*this, &QuestManager::QuestCompleted);
  _quests.push_back(quest);
  GameTask::CurrentGameTask->PlaySound("pipbuck/newquest");
  if (_level)
    quest->Initialize(_level);
  QuestsUpdated.Emit();
}

void QuestManager::QuestCompleted(Quest* quest)
{
  Data data = quest->data;

  if (data["complete"].Value() != "1")
  {
    data["complete"] = 1;
    _stats_controller->AddExperience(data["reward"]);
  }
}

/*
 * Quests
 */
void Quest::Initialize(Level* level)
{
  Data objectives = data["objectives"];
  
  for_each(objectives.begin(), objectives.end(), [this, level](Data objective)
  {
    Data   conditions = objective["conditions"];
    Quest* _this      = this;
    Level* _level     = level;

    for_each(conditions.begin(), conditions.end(), [_this, _level](Data condition)
    {
      _this->InitializeCondition(condition, _level);
    });
  });
}

void Quest::InitializeCondition(Data condition, Level* level)
{
  std::string        type           = condition.Key();
  WatcherInitializer initializers[] = {
    { "InventoryContains", &Quest::WatcherCharacterInventory },
    { "CharacterKilled",   &Quest::WatcherCharacterKill      },
    { "DateFilter",        &Quest::WatcherTime               },
    { "", 0 }
  };

  for (short i = 0 ; initializers[i].method != 0 ; ++i)
  {
    if (initializers[i].key == condition.Key())
    {
      (this->*initializers[i].method)(condition, level);
      break ;
    }
  }
}

/*
 * Watcher Initialization
 */
void Quest::WatcherCharacterInventory(Data condition, Level* level)
{
  std::string      character_name = condition["target"];
  ObjectCharacter* character      = level->GetCharacter(character_name);

  cout << "WatchedCharacterInventory" << endl;
  condition.Output();
  if (character)
  {
    cout << "Setting up observer on character " << character->GetName() << endl;
    _observers.Connect(character->GetInventory().ContentChanged, [this, condition, character](void)
    {
      std::string    object_type = condition["object_type"].Value();
      unsigned short quantity    = condition["object_quantity"];

      cout << "Checking if quest objective has been completed" << endl;
      if (character->GetInventory().ContainsHowMany(object_type) >= quantity)
      {
        (Data)(condition["completed"]) = 1;
        CheckIfCompleted();
      }
      else
        (Data)(condition["completed"]) = 0;
    });
  }
}

void Quest::WatcherCharacterKill(Data condition, Level* level)
{
  std::string      character_name = condition["target"];
  ObjectCharacter* character      = level->GetCharacter(character_name);
  
  if (character)
  {
    std::function<void (void)> eval([this, condition](void)
    {
      (Data)(condition["completed"]) = 1;
      CheckIfCompleted();
    });
    
    if (character->GetHitPoints() <= 0)
      eval();
    else
      _observers.Connect(character->CharacterDied, eval);
  }
}

void Quest::WatcherTime(Data condition, Level* level)
{
  
}

bool Quest::CheckIfCompleted(void)
{
  bool success    = true;
  Data objectives = data["objectives"];

  for_each(objectives.begin(), objectives.end(), [this, &success](Data objective)
  {
    Data   conditions        = objective["conditions"];
    bool   objective_success = true;

    for_each(conditions.begin(), conditions.end(), [&objective_success](Data condition)
    {
      objective_success = objective_success && condition["completed"] == 1;
    });
    success = success && objective_success;
    if (objective_success)
      ObjectiveCompleted.Emit(objective);
  });
  if (success)
    QuestCompleted.Emit(this);
  return (success);
}