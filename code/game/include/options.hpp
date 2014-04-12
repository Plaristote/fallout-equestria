#ifndef  OPTIONS_HPP
# define OPTIONS_HPP

# include "globals.hpp"
# include "datatree.hpp"
# include "observatory.hpp"

class OptionsManager
{
  OptionsManager()  {}
  ~OptionsManager() {}
public:
  static Sync::Signal<void> Updated;

  static void      Initialize(void);
  static void      Finalize(void);
  static Data      Get(void);
  static void      Refresh(void);
private:
  static DataTree* _data;
};

#endif
