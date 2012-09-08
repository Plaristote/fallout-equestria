#include "mainmenu.hpp"

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
    MouseData    pointer = _window->get_graphics_window()->get_pointer(0);

    strTop  << (pointer.get_y() + 1);
    strLeft << (pointer.get_x() + 1);
    _cursor->SetProperty("top",  strTop.str().c_str());
    _cursor->SetProperty("left", strLeft.str().c_str());
    _root->PullToFront();
  }
}

MainMenu::MainMenu(WindowFramework* window) : _window(window), _generalUi(window), _mouseCursor(window, _generalUi.GetRocketRegion()->get_context()), _view(window, _generalUi.GetRocketRegion()->get_context())
{
  _uiLoad    = 0;
  _levelTask = 0;
  AsyncTaskManager::get_global_ptr()->add(this);
  
  _view.Continue.Connect(*this, &MainMenu::Continue);
  _view.LoadGame.Connect(*this, &MainMenu::OpenUiLoad);
  _view.Quit.Connect(*this, &MainMenu::QuitGame);
  _view.Show();
  slotToLoadPlz  = -1;
  createLevelPlz = false;
  quitGamePlz    = false;
}

void MainMenu::Continue(Rocket::Core::Event&)
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
  _mouseCursor.Update();
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
    std::string                      idz[]       = { "button-continue", "button-new-game", "button-load-game", "button-options", "button-quit" };
    Observatory::Signal<void (Rocket::Core::Event&)>* signalz[]   = { &Continue, &NewGame, &LoadGame, &Options, &Quit };
    RocketListener*                  listenerz[] = { &ContinueClicked, &NewGameClicked, &LoadGameClicked, &OptionsClicked, &QuitClicked };
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
  }
}