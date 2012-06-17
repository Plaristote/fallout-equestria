#include "general_pony.as"

Timer myTimer;
int   movCount     = 0;
int   initWaypoint = 0;

void main(Character@ self, float elaspedTime)
{
  /*if (!(self.IsMoving()) && myTimer.GetElapsedTime() > 2.5)
  {
    if (initWaypoint == 0)
      initWaypoint = self.GetCurrentWaypoint();

    if (movCount == 5)
    {
      self.GoTo(initWaypoint);
      movCount = 0;
    }
    else
    {
      self.GoToRandomWaypoint();
      movCount++;
    }

    myTimer.Restart();
  }*/
  if (!(self.IsMoving()))
  {
    float distance = self.GetDistance(level.GetPlayer().AsObject());

    if (distance < 30)
    {
      int waypoint = self.GetFarthestWaypoint(level.GetPlayer().AsObject());

      if (waypoint != self.GetCurrentWaypoint())
        self.GoTo(waypoint);
    }
  }
}

void combat(Character@ self)
{
  if (self.GetPathDistance(level.GetPlayer().AsObject()) <= 1)
  {
    if (!(self.IsMoving()))
    {
      Character@ target = level.GetPlayer();

      if (self.GetActionPoints() >= 3)
        level.ActionUseWeaponOn(self, target, self.GetEquipedItem(0));
      else
        level.NextTurn();
    }
    return ;
  }
  self.GoTo(level.GetPlayer().AsObject(), 1);
}
