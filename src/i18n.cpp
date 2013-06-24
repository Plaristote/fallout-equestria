#include <i18n.hpp>
#include <directory.hpp>
#include <gameui.hpp>
#include <algorithm>

using namespace std;

i18n*                     i18n::_self = 0;
Sync::Signal<void> i18n::LanguageChanged;

i18n::i18n(const string& language)
{
  cout << "[i18n] Language set to " << language << endl;
  _dialogs    = DataTree::Factory::JSON("data/i18n/" + language + "/dialogs.json");
  _statistics = DataTree::Factory::JSON("data/i18n/" + language + "/statistics.json");
  if (!_dialogs)
    cout << "[i18n] Dialogs can't be found for the " << language << " language." << endl;
  if (!_statistics)
    cout << "[i18n] Statistics can't be found for the " << language << " language." << endl;
}

i18n::~i18n()
{
  if (_dialogs)    delete _dialogs;
  if (_statistics) delete _statistics;
}

void i18n::Load(const string& language)
{
  Unload();
  _self = new i18n(language);
  LanguageChanged.Emit();
}

void i18n::Unload(void)
{
  if (_self)
  {
    cout << "[i18n] Unloaded current language" << endl;
    delete _self;
  }
}

vector<string> i18n::LanguagesAvailable(void)
{
  vector<string>            list;
  Directory                 dir;

  dir.OpenDir("data/i18n");
  {
    const Directory::Entries& files = dir.GetEntries();
    
    for_each(files.begin(), files.end(), [&list](const Dirent& file)
    {
      if (file.d_type == DT_DIR && file.d_name[0] != '.')
	list.push_back(file.d_name);
    });
  }
  return (list);
}

Data i18n::GetDialogs(void)
{
  if (_self && _self->_dialogs)
    return (Data(_self->_dialogs));
  return (Data());
}

Data i18n::GetStatistics(void)
{
  if (_self && _self->_statistics)
    return (Data(_self->_statistics));
  return (Data());
}

string i18n::T(const std::string& str)
{
  if (_self && _self->_statistics && _self->_dialogs)
  {
    Data statistics(_self->_statistics);
    Data dialogs(_self->_dialogs);
    Data value;

    value   = statistics[str];
    if (value.Nil())
      value = dialogs[str];
    if (!(value.Nil()))
      return (value.Value());
  }
  return (str);
}
