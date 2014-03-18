#include <i18n.hpp>
#include <directory.hpp>
#include <ui/gameui.hpp>
#include <algorithm>

using namespace std;

i18n*                     i18n::_self = 0;
Sync::Signal<void> i18n::LanguageChanged;

i18n::i18n(const string& language)
{
  cout << "[i18n] Language set to " << language << endl;
  current_language = DataTree::Factory::JSON("i18n/" + language + ".json");
}

i18n::~i18n()
{
  if (current_language) delete current_language;
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

// TODO update that to find the language files
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

string i18n::T(const std::string& key)
{
  if (_self && _self->current_language)
  {
    Data translations(_self->current_language);
    Data value = translations[key];

    if (value.NotNil())
      return (value.Value());
  }
  return (key);
}
