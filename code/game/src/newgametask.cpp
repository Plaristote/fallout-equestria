#include "newgametask.hpp"
#include "ui/alert_ui.hpp"
#include "playerparty.hpp"

using namespace std;
using namespace Rocket;

NewGameTask::NewGameTask(WindowFramework* window, Core::Context* rocket) : _ui_new_game(window, rocket)
{
  _stat_controller = 0;
  _stat_view       = 0;
  _stat_sheet      = 0;
  _window          = window;
  _rocket          = rocket;
  _ui_new_game.StartFromScratch.Connect(*this,        &NewGameTask::StartFromScratch);
  _ui_new_game.SelectProfile.Connect   (*this,        &NewGameTask::SelectProfile);
  _ui_new_game.Cancel.Connect          (_ui_new_game, &UiBase::Hide);
  _ui_new_game.Cancel.Connect          (Cancel,       &Sync::Signal<void>::Emit);
  _ui_new_game.Show();
  StartGame.Connect(*this, &NewGameTask::Done);
}

NewGameTask::~NewGameTask()
{
  if (_stat_controller) { delete _stat_controller; }
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
    _stat_view->SetEditMode(StatView::Create);
    _stat_controller->SetView(_stat_view);
    _stat_controller->ChangesCanceled.Connect(Cancel,    &Sync::Signal<void>::Emit);
    //_stat_controller->ChangesAccepted.Connect(*this,     &NewGameTask::Done);
    _stat_controller->ChangesAccepted.Connect(StartGame, &Sync::Signal<void>::Emit);
    _stat_view->Show();
    _ui_new_game.Hide();
  }
}

void NewGameTask::StartFromScratch(void)
{
  SelectProfile("null");
}

void NewGameTask::ClearSaveDirectory(Directory& savedir)
{
  const string& savepath = savedir.Path();

  for_each(savedir.GetEntries().begin(), savedir.GetEntries().end(), [savepath](const Dirent entry)
  {
    if (entry.d_type == DT_REG)
    {
      cout << "[Clearing old game save] Removing file " << entry.d_name << endl;
      remove(string(savepath + "/" + entry.d_name).c_str());
    }
  });
}

void NewGameTask::GenerateFirstSaveFromTemplate(const std::string& savepath)
{
  string    to_copy[] = { "dataengine.json", "map.json", "cities.json" };

  for (unsigned short i = 0 ; i < 3 ; ++i)
  {
    bool ret = Filesystem::FileCopy("data/newgame/" + to_copy[i], savepath + "/" + to_copy[i]);

    cout << "[NewGameTask] Copying '" << "data/newgame/" + to_copy[i] << "' to '" << savepath + "/" + to_copy[i] << "'" << endl;

    if (ret == false)
      cerr << "[NewGameTask] Failed to copy file " << to_copy[i] << endl;
  }
}

void NewGameTask::GeneratePlayerParty(void)
{
  string       savepath = OptionsManager::Get()["savepath"].Value();
  PlayerParty  party;
  DataTree     datatree;

  {
    Data       player_data(&datatree);

    player_data["name"]         = "self";
    player_data["interactions"] = (int)(Interactions::UseObject | Interactions::UseSkill | Interactions::UseSpell | Interactions::LookAt);
    player_data["model"]        = "lpip.egg";
    player_data["texture"]      = "characters/lpip.png";
    party.Join(player_data);
    GeneratePlayerInventory(party.GetMember("self")->GetInventory());
  }
  party.SetName("player");
  party.Save(savepath);
}

void NewGameTask::GeneratePlayerInventory(Inventory* inventory)
{
  DataTree* inventory_data = DataTree::Factory::JSON("data/newgame/inventory.json");

  if (inventory_data)
    inventory->LoadInventory(inventory_data);
  else
    AlertUi::NewAlert.Emit("Your character's inventory failed to initialize.");
}

void NewGameTask::Done(void)
{
  Directory savedir;
  string    savepath  = OptionsManager::Get()["savepath"].Value();

  cout << "[NewGameTask] Done. Creating initial save." << endl;
  if (savedir.OpenDir(savepath))
  {
    ClearSaveDirectory(savedir);
    GenerateFirstSaveFromTemplate(savepath);
    DataTree::Writers::JSON(_stat_sheet, "data/charsheets/self.json");
    GeneratePlayerParty();
  }
  else
    AlertUi::NewAlert.Emit("Could not open save directory '" + savepath + "'");
}

/*
 * UiNewGame
 */
UiNewGame::UiNewGame(WindowFramework* window, Core::Context* context) : UiBase(window, context)
{
  _root = context->LoadDocument("data/new_game.rml");
  if (_root)
  {
    ToggleEventListener(true, "button-start-from-scratch", "click", EventStartFromScratch);
    ToggleEventListener(true, "button-select-profile",     "click", EventSelectProfile);
    ToggleEventListener(true, "button-previous-profile",   "click", PreviousProfile);
    ToggleEventListener(true, "button-next-profile",       "click", NextProfile);
    ToggleEventListener(true, "quit-button",               "click", EventCancel);
    EventSelectProfile.EventReceived.Connect   (*this, &UiNewGame::SelectedProfile);
    PreviousProfile.EventReceived.Connect      (*this, &UiNewGame::GoToPreviousProfile);
    NextProfile.EventReceived.Connect          (*this, &UiNewGame::GoToNextProfile);
    EventStartFromScratch.EventReceived.Connect(*this, &UiNewGame::ClickedStartFromScratch);
    EventCancel.EventReceived.Connect          (*this, &UiNewGame::GoCancel);
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
  ToggleEventListener(false, "quit-button",               "click", EventCancel);  
}
