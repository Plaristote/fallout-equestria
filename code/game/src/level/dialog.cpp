#include "level/dialog.hpp"
#include "level/objects/character.hpp"
#include "ui/alert_ui.hpp"
#include "ui/gameui.hpp"

using namespace Rocket;
using namespace std;

// VIEW
DialogView::DialogView(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window, context)
{
  _root = context->LoadDocument("data/dialog.rml");
  if (_root)
  {
    _containerNpcLine = _root->GetElementById("npcLine");
    _containerAnswers = _root->GetElementById("answers");
    ToggleEventListener(true, "button-open-barter", "click", BarterOpened);
  }
}

DialogView::~DialogView()
{
  if (_root)
  {
    if (_containerAnswers)
    {
      Core::Element* element = 0;
      
      for (int i = 0 ; (element = _containerAnswers->GetChild(i)) != 0 ; ++i)
      {
        element->RemoveEventListener("click", &AnswerSelected);
      }
    }
    ToggleEventListener(false, "button-open-barter", "click", BarterOpened);
    _root->Close();
    _root->RemoveReference();
  }
}

void DialogView::Destroy()
{
  if (_root)
  {
    if (_containerAnswers)
    {
      Core::Element* element = 0;

      for (int i = 0 ; (element = _containerAnswers->GetChild(i)) != 0 ; ++i)
      {
        element->RemoveEventListener("click", &AnswerSelected);
      }
    }
    _root->Close();
    _root->RemoveReference();
    _root = 0;
  }
}

void DialogView::SetCurrentNpcText(const string& message)
{
  _containerNpcLine->SetInnerRML(message.c_str());
}

void DialogView::UpdateView(const std::string& npcLine, const DialogAnswers& answers)
{
  if (_root && _containerAnswers && _containerNpcLine)
  {
    std::string answersRml;

    std::for_each(answers.answers.begin(), answers.answers.end(), [&answersRml](DialogAnswers::KeyValue data)
    {
      answersRml += "<button class='dialog-answer' id='" + data.first + "'>";
      answersRml += data.second;
      answersRml += "</button><br />";
    });
    std::cout << "RML -> " << npcLine << std::endl;
    std::cout << "RML -> " << answersRml << std::endl;
    _containerNpcLine->SetInnerRML(npcLine.c_str());
    _containerAnswers->SetInnerRML(answersRml.c_str());
    std::for_each(answers.answers.begin(), answers.answers.end(), [this](DialogAnswers::KeyValue data)
    {
      Core::Element* element;

      element = _containerAnswers->GetElementById(data.first.c_str());
      element->AddEventListener("click", &AnswerSelected);
    });
    _root->Show();
  }
}

void DialogView::CleanView(const DialogAnswers& answers)
{
  std::for_each(answers.answers.begin(), answers.answers.end(), [this](DialogAnswers::KeyValue data)
  {
    Core::Element* element;

    element = _containerAnswers->GetElementById(data.first.c_str());
    if (element)
    {
      element->RemoveEventListener("click", &AnswerSelected);
      _containerAnswers->RemoveChild(element);
    }
  });
}

// CONTROLLER
DialogController::DialogController(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* character, Data l18n) : DialogView(window, context), _script("scripts/dialogs/" + character->GetDialog() + ".as"), _model(character->GetDialog(), l18n)
{
  _character = character;
  AnswerSelected.EventReceived.Connect(*this, &DialogController::ExecuteAnswer);
  _script.asDefineMethod("HookInit", "string HookInit(Character@)");
  try
  {
    string                              npc_line;
    AngelScript::Type<ObjectCharacter*> self(character);

    npc_line = *(string*)(_script.Call("HookInit", 1, &self));

    SetCurrentNode(npc_line);
    BarterOpened.EventReceived.Connect(*this, &DialogController::OpenBarter);
  }
  catch (const AngelScript::Exception& exception)
  {
    const std::string message = "Dialog script crashed: ";

    AlertUi::NewAlert.Emit(message + exception.what());
    DialogEnded.Emit();
  }
}

DialogController::~DialogController()
{
  cout << "Destroyed Dialog Controller" << endl;
}

void DialogController::OpenBarter(Core::Event& event)
{
  cout << "OpenBarter executed" << endl;
  StartBarter.Emit(_character);
}

void DialogController::SetCurrentNode(const string& node)
{
  std::string                         npc_line;
  DialogAnswers                       answers;
  DialogAnswers::AnswerList::iterator it;
  DialogAnswers::AnswerList::iterator end;

  CleanView(_model.GetDialogAnswers());
  _model.SetCurrentNpcLine(node);
  std::cout << "currentNpcLine is  " << _model.GetNpcLine() << std::endl;
  npc_line = SolveStringVariables(_model.GetNpcLine());
  answers  = _model.GetDialogAnswers();
  for (it = answers.answers.begin(), end = answers.answers.end() ; it != end ;)
  {
    const string         availableHook = _model.GetHookAvailable((*it).first);
    bool                 available     = true;

    if (availableHook != "")
    {
      string             sign          = "bool " + availableHook + "()";

      try
      {
        _script.asDefineMethod(availableHook, "bool " + availableHook + "()");
        available = _script.Call(availableHook);
      }
      catch (const AngelScript::Exception& exception)
      {
        const string message = "Dialog crashed: ";

        AlertUi::NewAlert.Emit(message + exception.what());
      }
    }
    if (!available)
      it = answers.answers.erase(it);
    else
      ++it;
  }
  UpdateView(npc_line, answers);
}

void DialogController::ExecuteAnswer(Rocket::Core::Event& event)
{
  const string idAnswer = event.GetCurrentElement()->GetId().CString();
  std::cout << "EXECUTE ANSWER" << idAnswer << std::endl;
  const string exMethod = _model.GetExecuteMethod(idAnswer);
  string       nextNpcLine;

  if (exMethod != "")
  {
    try
    {
      _script.asDefineMethod(exMethod, "string " + exMethod + "()");
      nextNpcLine = *(string*)(_script.Call(exMethod));
    } catch (const AngelScript::Exception& exception)
    {
      const string message = "Dialog script crashed: ";

      AlertUi::NewAlert.Emit(message + exception.what());
    }
  }
  if (exMethod == "")
    nextNpcLine = _model.GetDefaultNextLine(idAnswer);

  std::cout << "NEXT NPC LINE IS " << nextNpcLine << std::endl;

  if (nextNpcLine != "")
    SetCurrentNode(nextNpcLine);
  else
    DialogEnded.Emit();
}

#include <sstream>
std::string DialogController::SolveStringVariables(const std::string& text)
{
  std::stringstream stream;
  bool              fetching_func_name = false;
  unsigned int      func_name_begin;
  unsigned int      text_begin         = 0;

  for (unsigned int i = 0 ; i < text.length() ; ++i)
  {
    if (fetching_func_name)
    {
      if (text[i] == '}')
      {
        std::string func_name = text.substr(func_name_begin, i - func_name_begin);

        // GETTING FUNC RESULT
        try
        {
          if (!(_script.IsDefined(func_name)))
          {
            std::string signature = "string " + func_name + "()";

            _script.asDefineMethod(func_name, signature);
          }
          stream << *(std::string*)(_script.Call(func_name));
        }
        catch (const AngelScript::Exception& exception)
        {
          stream << "#{Pipbuck Failure: " << exception.what() << "}";
        }
        text_begin         = i + 1;
        fetching_func_name = false;
      }
    }
    else
    {
      if ((text.substr(i, 2) == "#{") &&
          (i == 0 || text[i - 1] != '\\'))
      {
        stream << text.substr(text_begin, i - text_begin);
        func_name_begin    = i + 2;
        fetching_func_name = true;
        ++i;
      }
    }
  }
  stream << text.substr(text_begin);
  return (stream.str());
}

// MODEL
DialogAnswers     DialogModel::GetDialogAnswers(void)
{
  DialogAnswers   answers;

  if (_currentNpcLine != "")
  {
    Data::iterator  it  = _data[_currentNpcLine].begin();
    Data::iterator  end = _data[_currentNpcLine].end();

    _data[_currentNpcLine].Output();
    for (; it != end ; ++it)
    {
      if ((*it).Key() == "npcLine")
        continue ;
      DialogAnswers::KeyValue pair;
      Data                    line = _l18n[(*it).Key()];

      pair.first  = (*it).Key();
      pair.second = (line.Nil() ? (*it).Key() : line.Value());
      answers.answers.push_back(pair);
    }
  }
  return (answers);
}

void              DialogModel::SetCurrentNpcLine(const std::string& id)
{
  _currentNpcLine = id;
}
