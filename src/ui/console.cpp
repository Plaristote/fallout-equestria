#include "ui/game_console.hpp"
#include "scriptengine.hpp"
#include <scripthelper/scripthelper.h>
#include <sstream>
#include <Rocket/Controls.h>

using namespace std;
using namespace Rocket;

template<typename T>
void ArrayToJson(std::vector<T> array, std::string output)
{
  DataTree tmp;

  {
    Data     root(&tmp);
    Data     data = root["array"];

    for_each(array.begin(), array.end(), [data](const T& value)
    {
      Data node = data["new_value"];

      node = value;
      node.SetKey("");
    });
    DataTree::Writers::JSON(root, output);
  }
}

GameConsole* GameConsole::GConsole= 0;

GameConsole::GameConsole(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  cout << "Loading GameConsole" << endl;
  //
  // Load Script context
  //
  GConsole= this;

  if (Script::Engine::Get())
    _script_context = Script::Engine::Get()->CreateContext();
  else
  {
    _script_context = 0;
    cerr << "/!\\ [GameConsole] No Script::Engine" << endl;
  }

  _observerError = Script::Engine::ScriptError.Connect(*this, &GameConsole::Output);

  //
  // Load History
  //
  {
    DataTree* history_file = DataTree::Factory::JSON("data/history_console.json");
    if (history_file)
    {
      {
        Data   data_history(history_file);

        ForEach(data_history["array"], [this](Data entry)
        {
          _history.push_back(entry.Value());
        });
      }
      delete history_file;
    }
    _history.push_back("LF();");
    _histIter = _history.end();
    _histIter--;
  }

  //
  // Load Interface
  //
  _root  = context->LoadDocument("data/console.rml");
  if (_root)
  {
    cout << "[UI] Console is ready" << endl;
    ToggleEventListener(true, "console_input", "keyup", ConsoleKeyUp);
    ConsoleKeyUp.EventReceived.Connect(*this, &GameConsole::KeyUp);
    Translate();
  }
}

GameConsole::~GameConsole()
{
  if (_script_context)
    _script_context->Release();
  Script::Engine::ScriptError.Disconnect(_observerError);
  ToggleEventListener(false, "console_input", "keyup", ConsoleKeyUp);
}

void GameConsole::Execute(Rocket::Core::Event&)
{
  Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_input);
  Rocket::Core::String string = control->GetValue();

  if (string != "")
  {
    _history.push_back(string.CString());
    ArrayToJson(_history, "data/history_console.json");
    _histIter= _history.end();

    if (_script_context)
    {
      stringstream stream;

      _currentLine = string.CString();
      int ret = ExecuteString(Script::Engine::Get(), _currentLine.c_str(), 0, _script_context);
      if (ret == asEXECUTION_ABORTED || ret == asEXECUTION_EXCEPTION)
      {
	stream << "<span class='console-as-error'>[AngelScript]</span> ";
        if (ret == asEXECUTION_ABORTED)
	  stream << "Execution Aborted";
	else if (ret == asEXECUTION_EXCEPTION)
	  stream << "NullPointer error";
        Output(stream.str());
      }
    }
    _currentLine = "";
    control->SetValue("");
  }
}

void GameConsole::WriteOn(const std::string& str)
{
  GConsole->Output("<span class='console-write'>[Write]</span><span class='console-write-output'> " + str + "</span>");
}

void GameConsole::Output(const std::string str)
{
  Rocket::Core::Element* element = _root->GetElementById("console_output");
  Rocket::Core::String   string;

  element->GetInnerRML(string);
  string  = (str + "<br />" + string.CString()).c_str();
  element->SetInnerRML(string);
}

void GameConsole::KeyUp(Rocket::Core::Event& event)
{
  cout << "Some event" << endl;
  if (event == "keyup") {
          Rocket::Controls::ElementFormControl* control = reinterpret_cast<Rocket::Controls::ElementFormControl*>(_input);
          int keyId= event.GetParameter<int>("key_identifier", 0);

          if (keyId == Core::Input::KI_RETURN) {
                  Execute(event);
          };

          if (!_history.empty()) {
                  if (keyId == Core::Input::KI_DOWN) {
                          if (_histIter!=_history.begin()) {
                                  _histIter--;
                                  control->SetValue( _histIter->c_str() );
                          };
                  }; 
                  if (keyId == Core::Input::KI_UP) {
                          if (_histIter!=_history.end()) {
                                  control->SetValue( _histIter->c_str() );
                                  _histIter++;
                          } else {
                                  control->SetValue("");
                          };
                  };
          };
  }
}

//List all the functions registered inside AngelScript
void GameConsole::ListFunctions() {
	asIScriptEngine* engine = Script::Engine::Get();

	for (unsigned int i= 0; i<engine->GetGlobalFunctionCount(); i++)
		cout << "GLOBAL: " << engine->GetGlobalFunctionByIndex(i)->GetName() << endl;
	for (unsigned int i= 0; i<engine->GetFuncdefCount(); i++)
		cout << "FUNCDEF: " << engine->GetFuncdefByIndex(i)->GetName() << endl;
	for (unsigned int i= 0; i<engine->GetObjectTypeCount(); i++) {
		asIObjectType* obj = engine->GetObjectTypeByIndex(i);
		cout << "OBJ: " << obj->GetName() << endl;
		for (unsigned int j= 0; j<obj->GetMethodCount(); j++) 
			cout << "---->" << obj->GetMethodByIndex(j)->GetName() << endl;
	}
}

//Print out the entire scene graph into the console
void GameConsole::PrintScenegraph() {
	cout << "PRINTING SCENE GRAPH:" << endl;
	//render.ls();
	PrintChildren(GameConsole::Get()._window->get_render(),0);
	cout << endl;
}

void GameConsole::PrintChildren(const NodePath& n, int lvl) {
	for (int i= 0; i<lvl; i++)
		cout << "---";
	if (n.get_name()!="")
		cout << n.get_name() << endl;
	else
		cout << n << endl;

	if (n.get_num_children()!=0) {
		if (n.get_num_children()<100)
			for (int i= 0; i<n.get_num_children(); i++)
				PrintChildren( n.get_child(i), lvl+1 );
		else {
			for (int i= 0; i<lvl; i++)
				cout << "   ";
			cout << "(skipping past " << n.get_num_children() << " instances)\n";
		}
	}
	else {
		//cout << "{";
		//n.ls();
		//cout << "}";
	}
}
GameConsole& GameConsole::Get() {
	return *GConsole;
}

