#ifndef  GAME_CONSOLE_HPP
# define GAME_CONSOLE_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include "ui/rocket_extension.hpp"
# include "observatory.hpp"

class asIScriptContext;

class GameConsole : public UiBase
{
  friend class GameUi;
public:
  GameConsole(WindowFramework* window, Rocket::Core::Context* context);
  ~GameConsole(void);

  static GameConsole* GConsole;

  static GameConsole& Get();

  static void WriteOn(const std::string& string);
  static void ListFunctions();
  static void PrintScenegraph();
  static void PrintChildren(const NodePath& n, int lvl);

  static void Toggle(const Event*, void* data)
  {
    GameConsole* console = reinterpret_cast<GameConsole*>(data);
    console->IsVisible() ? console->Hide() : console->Show();
  }

  RocketListener         ConsoleKeyUp, ExecuteButton;

  Sync::Signal<void (std::string)> CommandToExecute;

private:
  void KeyUp(Rocket::Core::Event&);
  void Execute(Rocket::Core::Event&);
  void ExecuteString(std::string);
  void Output(const std::string str);

  std::vector<std::string>       _history;
  vector<std::string>::iterator  _histIter;

  WindowFramework*               _window;
  Rocket::Core::Element*         _input;
  std::string                    _currentLine;
  asIScriptContext*              _script_context;
  Sync::ObserverId               _observerError;
};

#endif
