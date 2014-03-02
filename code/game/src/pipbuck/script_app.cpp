#include "pipbuck/scripted_app.hpp"

using namespace std;

PipbuckAppScript::PipbuckAppScript(Data script) : _data(script)
{
  _object = new AngelScript::Object((const std::string&)("scripts/pipbuck/" + _data["src"].Value()));
}

void PipbuckAppScript::Exited(DataEngine& de)
{
  const string             function_name = _data["hooks"]["exit"].Value();
  AngelScript::Type<Data*> param(&de);

  if (!(_object->IsDefined(function_name)))
  {
    const string     function_decl = "void " + _data["hooks"]["exit"].Value() + "(Data)";

    _object->asDefineMethod(function_name, function_decl);
    if (!(_object->IsDefined(function_name)))
    {
      cout << "App missing exit hook (prototype is '" << function_name << "')" << endl;
      return ;
    }
  }
  _object->Call(function_name, 1, &param);
}

void PipbuckAppScript::Focused(Rocket::Core::Element* root, DataEngine& de)
{
  const string                              function_name = _data["hooks"]["focused"].Value();
  AngelScript::Type<Rocket::Core::Element*> param1(root);
  AngelScript::Type<Data*>                  param2(&de);

  if (!(_object->IsDefined(function_name)))
  {
    const string function_decl = "void " + function_name + "(RmlElement@, Data)";

    _object->asDefineMethod(function_name, function_decl);
    if (!(_object->IsDefined(function_name)))
    {
      cout << "App missing focused hook (prototype is '" << function_name << "')" << endl;
      return ;
    }
  }
  _object->Call(function_name, 2, &param1, &param2);
}

void PipbuckAppScript::Unfocused(DataEngine& de)
{
  const string             function_name = _data["hooks"]["unfocused"].Value();
  AngelScript::Type<Data*> param(&de);

  if (!(_object->IsDefined(function_name)))
  {
    const string     function_decl = "void " + function_name + "(Data)";

    _object->asDefineMethod(function_name, function_decl);
    if (!(_object->IsDefined(function_name)))
    {
      cout << "App missing exit unfocused (prototype is '" << function_name << "')" << endl;
      return ;
    }
  }
  _object->Call(function_name, 1, &param);
}

void PipbuckAppScript::RunAsBackgroundTask(DataEngine& de)
{
  const string             function_name = _data["hooks"]["run_background"].Value();
  AngelScript::Type<Data*> param(&de);
  int                      ret_value;

  if (!(_object->IsDefined(function_name)))
  {
    const string     function_decl = "int " + function_name + "(Data)";

    _object->asDefineMethod(function_name, function_decl);
    if (!(_object->IsDefined(function_name)))
      return ;
  }
  ret_value = _object->Call(function_name, 1, &param);
  switch (ret_value)
  {
    case 0:
      break ;
    default:
    case 1:
      Exit.Emit(this);
      break ;
    case 2:
      AskFocus.Emit(this);
      break ;
  }
}

void PipbuckAppScript::RunAsMainTask(Rocket::Core::Element* root, DataEngine& de)
{
  const string                              function_name = _data["hooks"]["run_focused"].Value();
  AngelScript::Type<Rocket::Core::Element*> param1(root);
  AngelScript::Type<Data*>                  param2(&de);
  int                                       ret_value;

  if (!(_object->IsDefined(function_name)))
  {
    const string     function_decl = "int " + function_name + "(RmlElement@, Data)";

    _object->asDefineMethod(function_name, function_decl);
    if (!(_object->IsDefined(function_name)))
      return ;
  }
  ret_value = _object->Call(function_name, 2, &param1, &param2);
  switch (ret_value)
  {
    case 0:
      break ;
    default:
    case 1:
      Exit.Emit(this);
      break ;
    case 2:
      Unfocus.Emit();
      break ;
  }
}

bool PipbuckAppScript::Started(DataEngine& de)
{
  const string             function_name = _data["hooks"]["start"].Value();
  AngelScript::Type<Data*> param(&de);

  if (!(_object->IsDefined(function_name)))
  {
    const string           function_decl = "bool " + function_name + "(Data)";

    _object->asDefineMethod(function_name, function_decl);
    if (!(_object->IsDefined(function_name)))
    {
      cout << "App missing start hook (prototype is configured to be: '" << function_name << "'" << endl;
      return (false);
    }
  }
  return (_object->Call(function_name, 1, &param));
}
