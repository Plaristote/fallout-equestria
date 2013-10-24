#include "level/level_zone.hpp"
#include "level/level.hpp"

LevelExitZone::LevelExitZone(Level* level, Zone& zone, std::list<std::string> destinations) : LevelZone(level, zone)
{
  ForEach(destinations, [this](std::string dest)
  {
    _destinations.push_back(dest);
  });
}

void LevelExitZone::GoingThrough(void* character)
{
  if (IsEnabled())
  {
    if (_name.substr(0, 9) == "LocalExit")
      GoingThroughLocal(character);
    else
      GoingThroughExit(character);
  }
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

/*
 * Exit Zone Selection
 */
void Level::CallbackExitZone(void)
{
  if (_currentUis[UiItNextZone])
    _currentUis[UiItNextZone]->Destroy();
  _exitingZone   = true;
}

void Level::CallbackGoToZone(const string& nextZone)
{
  LevelExitZone* zone = reinterpret_cast<LevelExitZone*>(GetPlayer()->GetOccupiedWaypoint()->arcs.front().observer);  
  
  if (_currentUis[UiItNextZone])
    _currentUis[UiItNextZone]->Destroy();
  _exitingZone       = true;
  _exitingZoneTo     = nextZone;
  if (zone)
    _exitingZoneName = zone->GetName();
}

void Level::CallbackSelectNextZone(const vector<string>& nextZoneChoices)
{
  if (!((_currentUis[UiItNextZone] && _currentUis[UiItNextZone]->IsVisible())))
  {
    UiNextZone* uiNextZone = new UiNextZone(_window, _levelUi.GetContext(), nextZoneChoices);

    if (_currentUis[UiItNextZone])
      delete _currentUis[UiItNextZone];
    _camera.SetEnabledScroll(false);
    SetInterrupted(true);
    _currentUis[UiItNextZone] = uiNextZone;
    uiNextZone->Cancel.Connect          (*this, &Level::CallbackCancelSelectZone);
    uiNextZone->NextZoneSelected.Connect(*this, &Level::CallbackGoToZone);
  }
}

void Level::CallbackCancelSelectZone()
{
  CloseRunningUi<UiItNextZone>();
}

const string& Level::GetNextZone(void) const
{
  return (_exitingZoneTo);
}

const string& Level::GetExitZone(void) const
{
  return (_exitingZoneName);
}
