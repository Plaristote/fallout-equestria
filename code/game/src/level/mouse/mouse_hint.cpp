#include "level/mouse/mouse_hint.hpp"
#include "level/level.hpp"
#include "mousecursor.hpp"

using namespace std;

MouseHint::MouseHint(Level& level, LevelUi& level_ui) : MouseInteractionHint(level), level(level), level_ui(level_ui)
{
}

void MouseHint::Run(float elapsed_time)
{
  if (!(IsHoveringUi()))
  {
    switch (GetState())
    {
      case MouseEvents::MouseAction:
        ClosestWaypoint(level.GetWorld(), level.GetCurrentFloor());
        MouseZoneHint::SetHoveredWaypoint(Hovering().waypoint_ptr);
        break ;
      case MouseEvents::MouseTarget:
        if (!Hovering().dynObject.is_empty())
          MouseTargetHint::SetHoveredObject(Hovering().dynObject);
        break ;
      case MouseEvents::MouseInteraction:
        if (!Hovering().dynObject.is_empty())
          MouseInteractionHint::SetHoveredObject(Hovering().dynObject);
        break ;
      default:
        ClearHint();
    }
  }
  else
    ClearHint();
  MouseInteractionHint::Run(elapsed_time);
}

void MouseHint::ClearHint()
{
  MouseCursor::Get()->SetHint("");
}
