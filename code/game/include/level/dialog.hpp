#ifndef  DIALOG_HPP
# define DIALOG_HPP

# include "ui/rocket_extension.hpp"
# include "scriptengine.hpp"
# include "as_object.hpp"
# include "datatree.hpp"
# include <ui/inventory_ui.hpp>
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
  DialogModel(const std::string& dialogId);
  ~DialogModel();

  void               SetCurrentNpcLine(const std::string& id);  
  const std::string  GetHookAvailable(const std::string& answerId);
  const std::string  GetExecuteMethod(const std::string& answerId);
  const std::string  GetDefaultNextLine(const std::string& answerId);
  const std::string GetNpcLine(void);

  DialogAnswers     GetDialogAnswers(void);

private:
  DataTree*          _tree;
  Data               _data;
  std::string        _currentNpcLine;
};

class DialogView : public UiBase
{
public:
  void Destroy(void);
  void SetCurrentNpcText(const std::string&);
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

  std::string      SolveStringVariables(const std::string&);

  AngelScript::Object _script;
  DialogModel         _model;
  ObjectCharacter*    _character;
};


#endif
