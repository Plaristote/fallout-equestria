#include "dialog.hpp"

using namespace Rocket::Core;
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

void DialogView::UpdateView(const std::string& npcLine, const DialogAnswers& answers)
{
  if (_root && _containerAnswers && _containerNpcLine)
  {
    std::string answersRml;

    std::for_each(answers.answers.begin(), answers.answers.end(), [&answersRml](DialogAnswers::KeyValue data)
    {
      answersRml += "<button class='dialog-answer' id='" + data.first + ">";
      answersRml += data.second;
      answersRml += "</button><br />";
    });
    _containerNpcLine->SetInnerRML(npcLine.c_str());
    _containerAnswers->SetInnerRML(answersRml.c_str());
    std::for_each(answers.answers.begin(), answers.answers.end(), [this](DialogAnswers::KeyValue data)
    {
      Rocket::Core::Element* element;

      element = _containerAnswers->GetElementById(data.first.c_str());
      element->AddEventListener("click", &AnswerSelected);
    });
  }
}

// CONTROLLER
DialogController::DialogController(WindowFramework* window, Rocket::Core::Context* context, const string& dialogId) : DialogView(window, context), _model(dialogId)
{
  _context = Script::Engine::Get()->CreateContext();
  _module   = Script::Engine::LoadModule("Dialog-" + dialogId, "scripts/dialogs/" + dialogId + ".as");
  AnswerSelected.EventReceived.Connect(*this, &DialogController::ExecuteAnswer);

  // Init Dialogue
  asIScriptFunction* hook = _module->GetFunctionByDecl("string HookInit()");

  if (hook)
  {
    _context->Prepare(hook);
    if ((_context->Execute()) == asEXECUTION_FINISHED)
    {
      string npcLine = *(reinterpret_cast<string*>(_context->GetReturnObject()));

      _model.SetCurrentNpcLine(npcLine);
       UpdateView(_model.GetNpcLine(), _model.GetDialogAnswers());
    }
  }
}

void DialogController::ExecuteAnswer(Rocket::Core::Event& event)
{
  const string idAnswer = event.GetCurrentElement()->GetId().CString();
  const string exMethod = _model.GetExecuteMethod(idAnswer);
  string       nextNpcLine;

  if (exMethod != "")
  {
    string             sign = "string " + exMethod + " (Character@, float)";
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

  if (nextNpcLine != "")
  {
    _model.SetCurrentNpcLine(nextNpcLine);
    UpdateView(_model.GetNpcLine(), _model.GetDialogAnswers());
  }
  else
  {
    // END DIALOG
  }
}
