#ifndef  I18N_HPP
# define I18N_HPP

# include "globals.hpp"
# include "datatree.hpp"
# include "observatory.hpp"
# include <vector>

class i18n
{
  static i18n* _self;
  
  i18n(const std::string& language);
  ~i18n();
public:
  static void                      Load(const std::string& language);
  static void                      Unload(void);
  static std::vector<std::string>  LanguagesAvailable(void);

  static std::string               T(const std::string& key);
  
  static Data                      GetDialogs(void);
  static Data                      GetStatistics(void);
  
  static Sync::Signal<void> LanguageChanged;

private:
  DataTree*                        _dialogs;
  DataTree*                        _statistics;
};

#endif
