#ifndef  MOUSE_HINT_HPP
# define MOUSE_HINT_HPP

# include "globals.hpp"

class Level;
class LevelUi;
class Mouse;

class MouseHint
{
public:
  MouseHint(Level& level, LevelUi& level_ui, Mouse& mouse) : level(level), level_ui(level_ui), mouse(mouse) {}

  void     Run(void);
  void     ClearHint(void);

private:
  void     ActionHint(void);
  void     TargetHint(void);

  void     NoPathHint(void);
  void     ExitZoneHint(void);
  void     SuccessRateHint(void);

  Level&   level;
  LevelUi& level_ui;
  Mouse&   mouse;
};

#endif
