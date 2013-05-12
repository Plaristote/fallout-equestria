#include "level/level.hpp"

LevelExitZone::LevelExitZone(Level* level, std::list<std::string> destinations) : _level(level)
{
  ForEach(destinations, [this](std::string dest)
  {
    _destinations.push_back(dest);
  });
}

void LevelExitZone::GoingThrough(void* character)
{
  if (_name.substr(0, 9) == "LocalExit")
    GoingThroughLocal(character);
  else
    GoingThroughExit(character);
}

void LevelExitZone::GoingThroughLocal(void* ptr)
{
  if (_destinations.size() > 0)
  {
    ObjectCharacter* character = reinterpret_cast<ObjectCharacter*>(ptr);
    
    _level->SetEntryZone(character, _destinations.front());
  }
}

void LevelExitZone::GoingThroughExit(void* character)
{
  if (character == _level->GetPlayer())
  {
    if (_destinations.size() == 0)
      ExitZone.Emit();
    else if (_destinations.size() == 1)
      GoToNextZone.Emit(_destinations.front());
    else
      SelectNextZone.Emit(_destinations);
  }
}
