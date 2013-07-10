#include "mainmenu.hpp"
#include "musicmanager.hpp"
#include "soundmanager.hpp"
#include "executor.hpp"
#include <ui_dialog.hpp>

extern PandaFramework framework;

static AsyncTask::DoneStatus main_menu_task(GenericAsyncTask* task, void* main_menu)
{
  return (reinterpret_cast<MainMenu*>(main_menu)->do_task());
}

MainMenu::MainMenu(WindowFramework* window) : _window(window), _generalUi(window), _mouseCursor(window, _generalUi.GetRocketRegion()->get_context()), _view(window, _generalUi.GetRocketRegion()->get_context())
{
  _new_game_task = 0;
  _uiLoad        = 0;
  _levelTask     = 0;

  AsyncTaskManager::get_global_ptr()->add(new GenericAsyncTask("Everything", &main_menu_task, this));

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

  _window->set_background_type(WindowFramework::BT_black);

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
  static unsigned int fps = 0;
  static Timer        timer;
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
        case AsyncTask::DS_exit:
          quitGamePlz = true;
          break ;
        case AsyncTask::DS_done:
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
  if (!quitGamePlz)
    _mouseCursor.Update();
  else
    framework.close_framework();
  SoundManager::GarbageCollectAll();
  Executor::Run(); // Executor does not have any specific application. It just executes lambdas collected here and there.
  fps++;
  if (timer.GetElapsedTime() > 1.f)
  {
    std::cout << ":::: Frame per seconds: " << fps << std::endl;
    fps = 0;
    timer.Restart();
  }
  return (quitGamePlz ? AsyncTask::DS_exit : AsyncTask::DS_cont);
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
  if (_new_game_task)
  {
    delete _new_game_task;
    _new_game_task = 0;
  }
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
    std::string                      idz[]                 = { "button-continue", "button-new-game", "button-load-game", "button-options", "button-quit" };
    Sync::Signal<void (Rocket::Core::Event&)>* signalz[]   = { &Continue, &NewGame, &LoadGame, &Options, &Quit };
    RocketListener*                  listenerz[]           = { &ContinueClicked, &NewGameClicked, &LoadGameClicked, &OptionsClicked, &QuitClicked };

    for (int it = 0 ; it < 5 ; ++it)
    {
      Rocket::Core::Element* element = _root->GetElementById(idz[it].c_str());

      if (element)
      {
	element->AddEventListener("click", listenerz[it]);
	listenerz[it]->EventReceived.Connect(*signalz[it], &Sync::Signal<void (Rocket::Core::Event&)>::Emit);
      }
      else
	cerr << "Missing button " << idz[it] << endl;
    }
    Translate();
  }
}
