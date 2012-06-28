#include "dialog.hpp"

using namespace Rocket;
using namespace std;

// VIEW
DialogView::DialogView(WindowFramework* window, Rocket::Core::Context* context) : UiBase(window)
{
  _root = context->LoadDocument("data/dialog.rml");
  if (_root)
  {
    _containerNpcLine = _root->GetElementById("npcLine");
    _containerAnswers = _root->GetElementById("answers");
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
DialogController::DialogController(WindowFramework* window, Rocket::Core::Context* context, const string& dialogId, Data l18n) : DialogView(window, context), _model(dialogId, l18n)
{
  _context = Script::Engine::Get()->CreateContext();
  _module   = Script::Engine::LoadModule("Dialog-" + dialogId, "scripts/dialogs/" + dialogId + ".as");
  AnswerSelected.EventReceived.Connect(*this, &DialogController::ExecuteAnswer);

  if (_module)
  {
    // Init Dialogue
    asIScriptFunction* hook = _module->GetFunctionByDecl("string HookInit()");

    if (hook)
    {
      _context->Prepare(hook);
      if ((_context->Execute()) == asEXECUTION_FINISHED)
      {
        string npcLine = *(reinterpret_cast<string*>(_context->GetReturnObject()));

        SetCurrentNode(npcLine);
      }
    }
  }
  else
    DialogEnded.Emit();
}

void DialogController::SetCurrentNode(const string& node)
{
  DialogAnswers answers;
  DialogAnswers::AnswerList::iterator it;
  DialogAnswers::AnswerList::iterator end;

  CleanView(_model.GetDialogAnswers());
  _model.SetCurrentNpcLine(node);
  std::cout << "currentNpcLine is  " << _model.GetNpcLine() << std::endl;
  answers = _model.GetDialogAnswers();
  for (it = answers.answers.begin(), end = answers.answers.end() ; it != end ;)
  {
    const string         availableHook = _model.GetHookAvailable((*it).first);
    bool                 available     = true;

    if (availableHook != "")
    {
      string             sign          = "bool " + availableHook + "()";
      asIScriptFunction* hook          = _module->GetFunctionByDecl(sign.c_str());

      _context->Prepare(hook);
      if ((_context->Execute()) == asEXECUTION_FINISHED)
      {
        char test = _context->GetReturnByte();

        available = test != 0;
      }
    }
    if (!available)
      it = answers.answers.erase(it);
    else
      ++it;
  }
  UpdateView(_model.GetNpcLine(), answers);
}

void DialogController::ExecuteAnswer(Rocket::Core::Event& event)
{
  const string idAnswer = event.GetCurrentElement()->GetId().CString();
  std::cout << "EXECUTE ANSWER" << idAnswer << std::endl;
  const string exMethod = _model.GetExecuteMethod(idAnswer);
  string       nextNpcLine;

  if (exMethod != "")
  {
    string             sign = "string " + exMethod + "()";
    asIScriptFunction* hook = _module->GetFunctionByDecl(sign.c_str());

    if (hook)
    {
      _context->Prepare(hook);
      if ((_context->Execute()) == asEXECUTION_FINISHED)
        nextNpcLine = *(reinterpret_cast<string*>(_context->GetReturnObject()));
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
