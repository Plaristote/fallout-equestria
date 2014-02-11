#include "options.hpp"
#include "i18n.hpp"
#include <panda3d/pandaFramework.h>

DataTree*              OptionsManager::_data = 0;
extern PandaFramework* framework;

void OptionsManager::Initialize(void)
{
  if (!_data)
  {
    _data = DataTree::Factory::JSON("conf.json");
    if (!_data)
      _data = new DataTree;
    Refresh();
  }
}

void OptionsManager::Finalize(void)
{
  if (_data)
  {
    delete _data;
    _data = 0;
  }
}

Data OptionsManager::Get(void)
{
  if (!_data)
    Initialize();
  return (_data);
}

void OptionsManager::Refresh(void)
{
  Data data(_data);
  
  if (_data)
  {
    // Language
    Data language = data["language"];

    if (language.NotNil())
      i18n::Load(language.Value());
    
    // Screen
    Data             screen        = data["screen"];

    if (screen.NotNil())
    {
      bool             hide_cursor   = data["debug"]["cursor"] == 0;
      unsigned int     screen_width  = screen["x"];
      unsigned int     screen_height = screen["y"];
      bool             fullscreen    = screen["fullscreen"] == 1;
      WindowProperties props         = framework->get_window(0)->get_graphics_window()->get_properties();

      props.set_cursor_hidden(hide_cursor);
      props.set_fullscreen(fullscreen);
      props.set_size(screen_width, screen_height);
      props.set_icon_filename("data/icon.ico");
      framework->get_window(0)->get_graphics_window()->request_properties(props);
    }

    // Saving changes
    DataTree::Writers::JSON(_data, "conf.json");
  }
}

