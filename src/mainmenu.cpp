#include "mainmenu.hpp"

extern PandaFramework framework;

MainMenu::MainMenu(WindowFramework* window) : _window(window), _generalUi(window), _view(window, _generalUi.GetRocketRegion()->get_context())
{
  _uiLoad    = 0;
  _levelTask = 0;
  AsyncTaskManager::get_global_ptr()->add(this);
  
  _view.NewGame.Connect(*this, &MainMenu::NewGame);
  _view.LoadGame.Connect(*this, &MainMenu::OpenUiLoad);
  _view.Quit.Connect(*this, &MainMenu::QuitGame);
  _view.Show();
  slotToLoadPlz  = -1;
  createLevelPlz = false;
  quitGamePlz    = false;
}

void MainMenu::NewGame(Rocket::Core::Event&)
{
  createLevelPlz = true;
}

void MainMenu::EndGame(void)
{
  _view.Show();
  delete _levelTask;
  _levelTask = 0;
}

AsyncTask::DoneStatus MainMenu::do_task()
{
  if (createLevelPlz) AsyncCreateLevel();
  if (_levelTask)
  {
    DoneStatus done = _levelTask->do_task();
    
    if (done == AsyncTask::DoneStatus::DS_exit)
      EndGame();
  }
  return (quitGamePlz ? AsyncTask::DoneStatus::DS_exit : AsyncTask::DoneStatus::DS_cont);
}

void MainMenu::AsyncCreateLevel(void)
{
  _levelTask = new LevelTask(_window, _generalUi.GetRocketRegion());
  _view.Hide();
  if (slotToLoadPlz >= 0)
    _levelTask->LoadSlot(slotToLoadPlz);
  slotToLoadPlz  = -1;
  createLevelPlz = false;
}

void MainMenu::OpenUiLoad(Rocket::Core::Event&)
{
  if (_uiLoad) delete _uiLoad;
  _uiLoad = new UiLoad(_window, _generalUi.GetRocketRegion()->get_context(), "./saves");
  _uiLoad->LoadSlot.Connect(*this, &MainMenu::LoadSlot);
  _uiLoad->Show();
}

void MainMenu::LoadSlot(unsigned char slot)
{
  createLevelPlz = true;
  slotToLoadPlz  = slot;
  _uiLoad->Hide();
}

void MainMenu::QuitGame(Rocket::Core::Event&)
{
  quitGamePlz = true;
}

/*
 * MainMenu::View
 */
MainMenu::View::View(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  _root = context->LoadDocument("data/startmenu.rml");

  if (_root)
  {
    std::string                      idz[]       = { "button-new-game", "button-load-game", "button-options", "button-quit" };
    Observatory::Signal<void (Rocket::Core::Event&)>* signalz[]   = { &NewGame, &LoadGame, &Options, &Quit };
    RocketListener*                  listenerz[] = { &NewGameClicked, &LoadGameClicked, &OptionsClicked, &QuitClicked };
    Rocket::Core::Element*           buttons[4];

    for (int it = 0 ; it < 4 ; ++it)
    {
      Rocket::Core::Element* element = _root->GetElementById(idz[it].c_str());

      if (element)
      {
	element->AddEventListener("click", listenerz[it]);
	listenerz[it]->EventReceived.Connect(*signalz[it], &Observatory::Signal<void (Rocket::Core::Event&)>::Emit);
      }
      else
	cerr << "Missing button " << idz[it] << endl;
    }
  }
}