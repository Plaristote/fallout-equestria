#include "newgametask.hpp"
#include <directory.hpp>

using namespace std;
using namespace Rocket;

NewGameTask::NewGameTask(WindowFramework* window, Core::Context* rocket) : _ui_new_game(window, rocket)
{
  _window = window;
  _rocket = rocket;
  _ui_new_game.StartFromScratch.Connect(*this, &NewGameTask::StartFromScratch);
  _ui_new_game.SelectProfile.Connect   (*this, &NewGameTask::SelectProfile);
  _ui_new_game.Show();
}

NewGameTask::~NewGameTask()
{
  // TODO Fix StatController deletion issues
  //if (_stat_controller) { delete _stat_controller; }
  if (_stat_view)       { delete _stat_view;       }
  if (_stat_sheet)      { delete _stat_sheet;      }
}

void NewGameTask::SelectProfile(const std::string& profile)
{
  _stat_sheet = DataTree::Factory::JSON("data/charsheets/" + profile + ".json");
  if (_stat_sheet)
  {
    _stat_controller = new StatController(_stat_sheet);
    _stat_view       = new StatViewRocket(_window, _rocket);
    _stat_controller->SetView(_stat_view);
    _stat_controller->ChangesAccepted.Connect(*this,     &NewGameTask::Done);
    _stat_controller->ChangesAccepted.Connect(StartGame, &Observatory::Signal<void>::Emit);
    _stat_view->Show();
    _ui_new_game.Hide();
  }
}

void NewGameTask::StartFromScratch(void)
{
  SelectProfile("null");
}

void NewGameTask::Done(void)
{
  string savepath  = OptionsManager::Get()["savepath"];
  string to_copy[] = { "dataengine.json", "map.json", "cities.json", "player-party.blob" };

  _stat_sheet->Save(savepath + "/stats-self.json");
  for (unsigned short i = 0 ; i < 3 ; ++i)
    Filesystem::FileCopy("data/newgame/" + to_copy[i], savepath + "/" + to_copy[i]);
}

/*
 * UiNewGame
 */
UiNewGame::UiNewGame(WindowFramework* window, Core::Context* context) : UiBase(window, context)
{
  _root = context->LoadDocument("data/new_game.rml");
  if (_root)
  {
    cout << "Loading UiNewGame" << endl;
    ToggleEventListener(true, "button-start-from-scratch", "click", EventStartFromScratch);
    ToggleEventListener(true, "button-select-profile",     "click", EventSelectProfile);
    ToggleEventListener(true, "button-previous-profile",   "click", PreviousProfile);
    ToggleEventListener(true, "button-next-profile",       "click", NextProfile);
    EventSelectProfile.EventReceived.Connect   (*this, &UiNewGame::SelectedProfile);
    PreviousProfile.EventReceived.Connect      (*this, &UiNewGame::GoToPreviousProfile);
    NextProfile.EventReceived.Connect          (*this, &UiNewGame::GoToNextProfile);
    EventStartFromScratch.EventReceived.Connect(*this, &UiNewGame::ClickedStartFromScratch);
  }
  else
    cout << "Missing file data/new_game.rml" << endl;
  
  _data_profiles = DataTree::Factory::JSON("data/newgame/profiles.json");
  if (_data_profiles)
  {
    Data profiles(_data_profiles);
    
    for_each(profiles.begin(), profiles.end(), [this](Data profile)
    { _profiles.push_back(profile.Key()); });
    GoToProfile((*profiles.begin()).Key());
  }
  _current_profile = _profiles.begin();
}

void UiNewGame::GoToPreviousProfile(Rocket::Core::Event&)
{
  _current_profile++;
  if (_current_profile == _profiles.end())
    _current_profile = _profiles.begin();
  GoToProfile(*_current_profile);
}

void UiNewGame::GoToNextProfile(Rocket::Core::Event&)
{
  if (_current_profile == _profiles.begin())
    _current_profile = _profiles.end();
  _current_profile--;
  GoToProfile(*_current_profile);
}

void UiNewGame::GoToProfile(const string& profile)
{
  // TODO
  cout << "Load Profile " << profile << endl;
  Data profiles(_data_profiles);
  
  Core::Element* name        = _root->GetElementById("character-name");
  Core::Element* description = _root->GetElementById("character-description");
  Core::Element* pros        = _root->GetElementById("character-pros");
  Core::Element* cons        = _root->GetElementById("character-cons");
  
  name->SetInnerRML       (profiles[profile]["Name"].Value().c_str());
  description->SetInnerRML(profiles[profile]["Description"].Value().c_str());
  pros->SetInnerRML       (profiles[profile]["Pros"].Value().c_str());
  cons->SetInnerRML       (profiles[profile]["Cons"].Value().c_str());
  
  string rml = "<img src='newgame/" + profiles[profile]["Image"].Value() + "' style='width: 100%; height: 100%;' />";
  Core::Element* image = _root->GetElementById("background-image");
  image->SetInnerRML(rml.c_str());
}

void UiNewGame::SelectedProfile(Rocket::Core::Event&)
{
  SelectProfile.Emit(*_current_profile);
}

void UiNewGame::ClickedStartFromScratch(Rocket::Core::Event&)
{
  StartFromScratch.Emit();
}

UiNewGame::~UiNewGame()
{
  ToggleEventListener(false, "button-start-from-scratch", "click", EventStartFromScratch);
  ToggleEventListener(false, "button-select-profile",     "click", EventSelectProfile);
  ToggleEventListener(false, "button-previous-profile",   "click", PreviousProfile);
  ToggleEventListener(false, "button-next-profile",       "click", NextProfile);  
}