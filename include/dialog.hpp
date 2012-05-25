#ifndef  DIALOG_HPP
# define DIALOG_HPP

# include "rocket_extension.hpp"
# include "scriptengine.hpp"
#include "datatree.hpp"
# include <list>
# include <string>
# include <algorithm>

struct DialogAnswers
{
  typedef std::pair<std::string, std::string> KeyValue;

  std::list<KeyValue> answers;
};

class DialogModel
{
public:
  DialogModel(const std::string& dialogId)
  {
    _tree = DataTree::Factory::JSON("data/dialogs/" + dialogId + ".json");
    if (_tree)
      _data = Data(_tree);
  }

  ~DialogModel()
  {
    if (_tree) delete _tree;
  }

  void               SetCurrentNpcLine(const std::string& id)
  {
    _currentNpcLine = _data[id];
  }
  
  const std::string  GetExecuteMethod(const std::string& answerId)
  {
    return (_currentNpcLine[answerId]["HookExecute"].Value());
  }

  const std::string  GetDefaultNextLine(const std::string& answerId)
  {
    return (_currentNpcLine[answerId]["DefaultAnswer"].Value());
  }

  const std::string GetNpcLine(void)
  {
    return (_currentNpcLine.Key()); // TODO replace this with L18n traduction of the key
  }

  DialogAnswers     GetDialogAnswers(void)
  {
    DialogAnswers   answers;
    Data::iterator  it  = _currentNpcLine.begin();
    Data::iterator  end = _currentNpcLine.end();
    
    for (; it != end ; ++it)
    {
      DialogAnswers::KeyValue pair;

      pair.first  = _currentNpcLine.Key();
      pair.second = _currentNpcLine.Key(); // TODO replace this with L18n traduction of the key
      answers.answers.push_back(pair);
    }
    return (answers);
  }

private:
  DataTree*          _tree;
  Data               _data;
  Data               _currentNpcLine;
};

class DialogView : public UiBase
{
protected:
  DialogView(WindowFramework* window, Rocket::Core::Context* context);
  ~DialogView() { if (_root) delete _root; }

  void UpdateView(const std::string& npcLine, const DialogAnswers& answers);

  RocketListener AnswerSelected;

private:
  Rocket::Core::Element* _containerNpcLine;
  Rocket::Core::Element* _containerAnswers;
};

class DialogController : public DialogView
{
public:
  DialogController(WindowFramework* window, Rocket::Core::Context* context, const std::string& dialogId);
  ~DialogController()
  {
    _context->Release();
  }

private:
  void             ExecuteAnswer(Rocket::Core::Event& event);

  asIScriptContext* _context;
  asIScriptModule*  _module;
  DialogModel       _model;
};


#endif