#ifndef  DIALOG_HPP
# define DIALOG_HPP

# include "rocket_extension.hpp"
# include "scriptengine.hpp"
#include "datatree.hpp"
#include <inventory_ui.hpp>
# include <list>
# include <string>
# include <algorithm>

struct DialogAnswers
{
  typedef std::pair<std::string, std::string> KeyValue;
  typedef std::list<KeyValue>                 AnswerList;

  AnswerList answers;
};

class DialogModel
{
public:
  DialogModel(const std::string& dialogId, Data l18n) : _l18n(l18n)
  {
    _tree = DataTree::Factory::JSON("data/dialogs/" + dialogId + ".json");
    if (_tree)
      _data = Data(_tree);
  }

  ~DialogModel()
  {
    if (_tree) delete _tree;
  }

  void               SetCurrentNpcLine(const std::string& id);  
  const std::string  GetHookAvailable(const std::string& answerId)
  {
    return (_data[_currentNpcLine][answerId]["HookAvailable"].Value());
  }

  const std::string  GetExecuteMethod(const std::string& answerId)
  {
    return (_data[_currentNpcLine][answerId]["HookExecute"].Value());
  }

  const std::string  GetDefaultNextLine(const std::string& answerId)
  {
    return (_data[_currentNpcLine][answerId]["DefaultAnswer"].Value());
  }

  const std::string GetNpcLine(void)
  {
    std::string     key  = _data[_currentNpcLine]["npcLine"].Value();
    Data            line = _l18n[key];

    if (line.Nil())
      return (key);
    return (line.Value());
  }

  DialogAnswers     GetDialogAnswers(void);

private:
  DataTree*          _tree;
  Data               _data;
  Data               _l18n;
  std::string        _currentNpcLine;
};

class DialogView : public UiBase
{
public:
  void Destroy(void);
protected:
  DialogView(WindowFramework* window, Rocket::Core::Context* context);
  virtual ~DialogView();

  void UpdateView(const std::string& npcLine, const DialogAnswers& answers);
  void CleanView(const DialogAnswers& answers);

  RocketListener AnswerSelected;
  RocketListener BarterOpened;

private:
  Rocket::Core::Element* _containerNpcLine;
  Rocket::Core::Element* _containerAnswers;
};

class DialogController : public DialogView
{
public:
  DialogController(WindowFramework* window, Rocket::Core::Context* context, ObjectCharacter* character, Data i18n);
  ~DialogController();

  ObjectCharacter* WithCharacter(void) const { return (_character); }

  Sync::Signal<void ()>                 DialogEnded;
  Sync::Signal<void (ObjectCharacter*)> StartBarter;

private:
  void             ExecuteAnswer(Rocket::Core::Event& event);
  void             SetCurrentNode(const std::string& nodeName);
  void             OpenBarter(Rocket::Core::Event& event);

  asIScriptContext* _context;
  asIScriptModule*  _module;
  DialogModel       _model;
  ObjectCharacter*  _character;
};


#endif
