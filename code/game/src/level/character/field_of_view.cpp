#include "level/characters/field_of_view.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"
#include "dices.hpp"
#include <algorithm>

using namespace std;

FieldOfView::FieldOfView(Level& level, ObjectCharacter& character) : level(level), character(character)
{
  needs_update = true;
  SetAsRepetitiveTask(true);
}

FieldOfView::~FieldOfView(void)
{
}

void FieldOfView::SetIntervalDurationFromStatistics(void)
{
  unsigned short        duration   = 3;
  const StatController* statistics = character.GetStatController();

  if (statistics)
  {
    unsigned short perception = statistics->Model().GetSpecial("PER");

    duration = (10 - perception) / 2;
  }
  SetIntervalDurationInSeconds(duration != 0 ? duration : 1);
}

void FieldOfView::RunCheck()
{
  if (needs_update && character.IsAlive())
  {
    SetIntervalDurationFromStatistics();
    LoseTrackOfCharacters(detected_enemies);
    LoseTrackOfCharacters(detected_characters);
    DetectCharacters();
    needs_update = false;
  }
}

void FieldOfView::Run()
{
  RunCheck();
  ScheduledTask::Run();
}

Level::CharacterList FieldOfView::GetDetectedCharacters(void) const
{
  CharacterList list;

  AppendEntriesToCharacterList(detected_enemies,    list);
  AppendEntriesToCharacterList(detected_characters, list);
  return (list);
}

Level::CharacterList FieldOfView::GetDetectedEnemies(void) const
{
  CharacterList list;

  AppendEntriesToCharacterList(detected_enemies, list);
  return (list);
}

Level::CharacterList FieldOfView::GetDetectedCharactersMatching(std::function<bool (ObjectCharacter*)> functor) const
{
  CharacterList list = GetDetectedCharacters();
  
  for (auto it = list.begin() ; it != list.end() ;)
  {
    if (functor(*it))
      ++it;
    else
      it = list.erase(it);
  }
  return (list);
}

Level::CharacterList FieldOfView::GetDetectedNonHostile(void) const
{
  return (GetDetectedCharactersMatching([this](ObjectCharacter* detected_character) -> bool
  {
    return (!(character.IsAlly(detected_character)) && !(character.IsEnemy(detected_character)));
  }));
}

Level::CharacterList FieldOfView::GetDetectedAllies(void) const
{
  return (GetDetectedCharactersMatching([this](ObjectCharacter* detected_character) -> bool
  {
    return (character.IsAlly(detected_character));
  }));
}

void FieldOfView::AppendEntriesToCharacterList(const std::list<Entry>& entries, CharacterList& list) const
{
  for (auto it = entries.begin() ; it != entries.end() ; ++it)
  {
    if (it->character)
      list.push_back(it->character);
  }
}

bool FieldOfView::HasLivingEnemiesInSight(void) const
{
  for (auto iterator = detected_enemies.begin() ; iterator != detected_enemies.end() ; ++iterator)
  {
    if ((**iterator).IsAlive())
      return (true);
  }
  return (false);
}

bool FieldOfView::IsDetected(const ObjectCharacter* character_to_check) const
{
  if (character.IsEnemy(character_to_check))
    return (IsCharacterInList(character_to_check, detected_enemies));
  return (IsCharacterInList(character_to_check, detected_characters));
}

bool FieldOfView::IsCharacterInList(const ObjectCharacter* character_to_check, const std::list<Entry>& list) const
{
  return (find(list.begin(), list.end(), character_to_check) != list.end());
}

void FieldOfView::DetectCharacters()
{
  Level::CharacterList characters_in_range = GetCharactersInRange();
  auto                 iterator            = characters_in_range.begin();

  for (; iterator < characters_in_range.end() ; ++iterator)
  {
    ObjectCharacter* checking_character = *iterator;

    if (checking_character->IsAlive() && checking_character != &character)
    {
      if (character.IsAlly(checking_character))
        SetCharacterDetected(*checking_character);
      else if (character.HasLineOfSight(checking_character))
      {
        if (character.IsEnemy(checking_character) && CheckIfEnemyIsDetected(*checking_character))
          SetEnemyDetected(*checking_character);
        else
          SetCharacterDetected(*checking_character);
      }
    }
  }
}

bool FieldOfView::CheckIfEnemyIsDetected(const ObjectCharacter& enemy) const
{
  return (!(enemy.HasFlag(FLAG_CHARACTER_SNEAK)) ||
          CheckIfSneakingEnemyIsDetected(enemy));
}

bool FieldOfView::CheckIfSneakingEnemyIsDetected(const ObjectCharacter &enemy) const
{
  const StatController* enemy_stats = enemy.GetStatController();
  const StatController* self_stats  = character.GetStatController();

  if (enemy_stats && self_stats)
  {
    short perception         = self_stats->Model().GetSpecial("PER");
    short sneak_skill        = enemy_stats->Model().GetSkill("Sneak");
    short sneak_success_rate = sneak_skill - (perception * (3 + (sneak_skill / 100)));

    if (sneak_success_rate > 95)
      sneak_success_rate     = 95;
    if (Dices::Throw(100) < sneak_success_rate)
      return (false);
  }
  return (true);
}

void FieldOfView::SetEnemyDetected(ObjectCharacter& enemy)
{
  InsertOrUpdateCharacterInList(enemy, detected_enemies);
}

void FieldOfView::SetCharacterDetected(ObjectCharacter& character)
{
  InsertOrUpdateCharacterInList(character, detected_characters);
}

void FieldOfView::InsertOrUpdateCharacterInList(ObjectCharacter& character, std::list<Entry>& list)
{
  auto iterator = find(list.begin(), list.end(), &character);
  
  if (iterator != list.end())
    iterator->time_to_live = FOV_TTL;
  else
    list.push_back(&character);
}

float FieldOfView::GetRadius() const
{
  const StatController* stat_controller = character.GetStatController();
  short                 perception      = 5;

  if (stat_controller)
    perception = stat_controller->Model().GetSpecial("PER");
  return (20 + (perception * 5));
}

Level::CharacterList FieldOfView::GetCharactersInRange() const
{
  float field_of_view_radius = GetRadius();

  return (level.FindCharacters([this, field_of_view_radius](ObjectCharacter* character) -> bool
  {
      return (character != &this->character &&
              character->GetDistance(&this->character) < field_of_view_radius);
  }));
}

void FieldOfView::LoseTrackOfCharacters(std::list<Entry>& entries)
{
  auto character = entries.begin();

  while (character != entries.end())
  {
    character->time_to_live--;
    if (character->time_to_live < 0)
      character = entries.erase(character);
    else
      character++;
  }
}
 
