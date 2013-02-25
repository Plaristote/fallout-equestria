#include "mainmenu.hpp"
#include "musicmanager.hpp"
#include <soundmanager.hpp>

extern PandaFramework framework;

MouseCursor* MouseCursor::_static = 0;

MouseCursor::MouseCursor(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  _root   = context->CreateDocument();
  if (_root)
  {
    _root->SetInnerRML("<img id='mouse-cursor' src='textures/cursor-interaction.png' />");
    _cursor = _root->GetElementById("mouse-cursor");
    if (_cursor)
      _cursor->SetProperty("position", "absolute");
  }
  Show();
  _static = this;
}

void MouseCursor::Update(void)
{
  if (_cursor && IsVisible())
  {
    stringstream strTop, strLeft;
    int          nTop,   nLeft;
    string       top,    left;
    MouseData    pointer = _window->get_graphics_window()->get_pointer(0);

    strLeft << ((int)pointer.get_x() + 1);
    strTop  << ((int)pointer.get_y() + 1);
    _cursor->SetProperty("top",  strTop.str().c_str());
    _cursor->SetProperty("left", strLeft.str().c_str());
    _root->PullToFront();
  }
}

MainMenu::MainMenu(WindowFramework* window) : _window(window), _generalUi(window), _mouseCursor(window, _generalUi.GetRocketRegion()->get_context()), _view(window, _generalUi.GetRocketRegion()->get_context())
{
  _new_game_task = 0;
  _uiLoad        = 0;
  _levelTask     = 0;
  AsyncTaskManager::get_global_ptr()->add(this);

  AlertUi::NewAlert.Connect([this](const string message)
  { _alerts.push_back(new AlertUi(_window, _generalUi.GetRocketRegion()->get_context(), message)); });

  _view.Continue.Connect(*this, &MainMenu::Continue);
  _view.LoadGame.Connect(*this, &MainMenu::OpenUiLoad);
  _view.NewGame.Connect (*this, &MainMenu::NewGame);
  _view.Quit.Connect    (*this, &MainMenu::QuitGame);
  _view.Options.Connect (_generalUi.GetOptions(), &UiBase::FireShow);
  _view.Show();
  slotToLoadPlz         = -1;
  createLevelPlz        = false;
  quitGamePlz           = false;
  _need_garbage_collect = false;

  MusicManager::Initialize();
  MusicManager::Get()->Play("mainmenu");
}

MainMenu::~MainMenu()
{
  if (_new_game_task) { delete _new_game_task; }
  MusicManager::Finalize();
}

void MainMenu::NewGame(Rocket::Core::Event&)
{
  if (_new_game_task)
    delete _new_game_task;
  _new_game_task = new NewGameTask(_window, _generalUi.GetRocketRegion()->get_context());
  _new_game_task->StartGame.Connect(*this, &MainMenu::StartGame);
  _new_game_task->Cancel.Connect(*this, &MainMenu::CancelNewGame);
}

void MainMenu::StartGame(void)
{
  createLevelPlz = true;
}

void MainMenu::CancelNewGame(void)
{
}

void MainMenu::Continue(Rocket::Core::Event&)
{
  createLevelPlz = true;
}

void MainMenu::EndGame(void)
{
  _view.Show();
  delete _levelTask;
  _levelTask            = 0;
  _need_garbage_collect = true;
  MusicManager::Get()->Play("mainmenu");
}

void MainMenu::DisplayAlerts(void)
{
  AlertUi* alert = _alerts.front();
  
  alert->Show();
  if (!(alert->Run()))
  {
    delete alert;
    _alerts.erase(_alerts.begin());
  }  
}

AsyncTask::DoneStatus MainMenu::do_task()
{
  MusicManager* mm = MusicManager::Get();

  if (_alerts.size() > 0)
    DisplayAlerts();
  else
  {
    if (mm) { mm->Run(); }
    if (createLevelPlz) AsyncCreateLevel();
    if (_levelTask)
    {
      DoneStatus done = _levelTask->do_task();

      switch (done)
      {
        case AsyncTask::DoneStatus::DS_exit:
          quitGamePlz = true;
          break ;
        case AsyncTask::DoneStatus::DS_done:
          EndGame();
          break ;
        default:
          break ;
      }
    }
    else if (_need_garbage_collect)
    {
      TexturePool::get_global_ptr()->garbage_collect();
      _need_garbage_collect = false;
    }
  }
  _mouseCursor.Update();
  SoundManager::GarbageCollectAll();
  return (quitGamePlz ? AsyncTask::DoneStatus::DS_exit : AsyncTask::DoneStatus::DS_cont);
}

void MainMenu::AsyncCreateLevel(void)
{
  _levelTask = new GameTask(_window, _generalUi);
  _view.Hide();
  if (slotToLoadPlz >= 0)
    _levelTask->LoadSlot(slotToLoadPlz);
  else
    _levelTask->LoadLastState();
  slotToLoadPlz  = -1;
  createLevelPlz = false;
  if (_new_game_task)
  {
    delete _new_game_task;
    _new_game_task = 0;
  }
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
    std::string                      idz[]                        = { "button-continue", "button-new-game", "button-load-game", "button-options", "button-quit" };
    Observatory::Signal<void (Rocket::Core::Event&)>* signalz[]   = { &Continue, &NewGame, &LoadGame, &Options, &Quit };
    RocketListener*                  listenerz[]                  = { &ContinueClicked, &NewGameClicked, &LoadGameClicked, &OptionsClicked, &QuitClicked };
    Rocket::Core::Element*           buttons[5];

    for (int it = 0 ; it < 5 ; ++it)
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
    Translate();
  }
}
