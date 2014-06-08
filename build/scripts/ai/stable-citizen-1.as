#include "general_pony.as"

int waypoint_id_start = -1;
int waypoint_id_end   = -1;
int trajectory_step   = -2;

void main(Character@ self, float elapsedTime)
{
  if (!(self.IsMoving()))
  {
    if (waypoint_id_start == -1 && trajectory_step > -1)
      waypoint_id_start = self.GetCurrentWaypoint();
    else if (waypoint_id_end == -1 && trajectory_step > -2)
      waypoint_id_end   = self.GetCurrentWaypoint();
    if (trajectory_step == -2)
    {
      self.GoTo(19, 14, -4);
      if (self.IsMoving())
        trajectory_step = -1;
    }
    else if (trajectory_step == -1)
    {
      self.GoTo(10, 14, -4);
      if (self.IsMoving())
        trajectory_step = 0;
    }
    else if (trajectory_step == 0)
    {
      self.GoTo(waypoint_id_start);
      if (self.IsMoving())
        trajectory_step = 1;
    }
    else if (trajectory_step == 1)
    {
      self.GoTo(waypoint_id_end);
      if (self.IsMoving())
        trajectory_step = 0;
    }
  }
}
void combat(Character@ self)
{
  Cout("Stable citizen in Combat mode");
}