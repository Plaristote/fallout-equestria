#ifndef  MOUSE_HINT_HPP
# define MOUSE_HINT_HPP

# include "globals.hpp"
# include "mouse_interaction_hint.hpp"

class Level;
class LevelUi;
class Mouse;

class MouseHint : public MouseInteractionHint
{
public:
  MouseHint(Level& level, LevelUi& level_ui);

  void     Run(float elapsed_time);
  void     ClearHint(void);

  Level&   level;
  LevelUi& level_ui;
};

#endif
