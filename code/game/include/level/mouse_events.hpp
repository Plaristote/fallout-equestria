#ifndef  MOUSE_EVENTS_HPP
# define MOUSE_EVENTS_HPP

# include "level/mouse.hpp"

class Level;
class InstanceDynamicObject;

class MouseEvents : public Mouse
{
public:
  Sync::Signal<void (Waypoint*)>              WaypointPicked;
  Sync::Signal<void (InstanceDynamicObject*)> TargetPicked;

  enum State
  {
    MouseAction,
    MouseInteraction,
    MouseTarget,
    MouseWaypointPicker
  };
  
  MouseEvents(Level& level, WindowFramework* window);
  
  void               SetState(State);
  State              GetState(void) const { return (mouse_state); }
  void               SetCursorFromState(void);
  bool               IsHoveringUi(void) const;
  void               MouseLeftClicked(void);
  void               MouseRightClicked(void);
  void               MouseWheelUp(void);
  void               MouseWheelDown(void);

private:
  Level&             level;
  State              mouse_state;
};

#endif