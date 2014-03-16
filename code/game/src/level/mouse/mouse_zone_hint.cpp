#include "level/mouse/mouse_zone_hint.hpp"
#include "level/level.hpp"
#include "mousecursor.hpp"

MouseZoneHint::MouseZoneHint(Level& level) : MouseWaypointHint(level, level.GetWindow()), zone_manager(level.GetZoneManager())
{
}

void MouseZoneHint::SetHoveredWaypoint(Waypoint* waypoint)
{
  MouseCursor* cursor = MouseCursor::Get();

  if (waypoint)
    ExitZoneHint(waypoint);
  else
    cursor->SetHint("nowhere");
  MouseWaypointHint::SetHoveredWaypoint(waypoint);
}

void MouseZoneHint::ExitZoneHint(Waypoint* waypoint)
{
  MouseCursor* cursor = MouseCursor::Get();

  if (zone_manager.IsInsideExitZone(waypoint))
    cursor->SetHint("exit");
  else
    cursor->SetHint("");
}
