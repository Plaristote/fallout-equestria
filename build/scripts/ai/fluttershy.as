#include "general_pony.as"

Timer myTimer;
int   movCount     = 0;
int   initWaypoint = 0;

bool shieldCasted = false;

void CastShield(Character@ self)
{
  if (shieldCasted == false)
  {
    Data dataEngine = level.GetDataEngine();
    Data buffData   = dataEngine["shielding spell"];

    buffData["graphics"]["model"]    = "sphere.obj";
    buffData["graphics"]["scale"]    = 3;
    buffData["duration"] = 30;

    buffData["script"]["source"]    = "spell.as";
    buffData["script"]["hookBegin"] = "shieldBegin";
    buffData["script"]["hookEnd"]   = "shieldEnd";

    self.PushBuff(buffData, self);
    shieldCasted = true;
  }
}

void main(Character@ self, float elaspedTime)
{
  if (myTimer.GetElapsedTime() > 2.5)
    CastShield(self);
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

Character@ currentTarget = null;

Character@ SelectTarget(Character@ self)
{
  CharacterList         enemies  = self.GetNearbyEnemies();
  CharacterListIterator it       = enemies.Begin();
  int                   nEnemies = enemies.Size();
  Character@            bestMatch;

  while (nEnemies > 0)
  {
    Character@ current = it.Value();

    if (@bestMatch == null || current.GetHitPoints() < bestMatch.GetHitPoints())
      @bestMatch = @(it.Value());

    nEnemies--;
    it++;
  }
  Write("Selected enemy: " + bestMatch.GetName());
  return (bestMatch);
}

void combat(Character@ self)
{
  if (self.IsMoving())
    return ;

  if (@currentTarget == null || !(currentTarget.IsAlive()))
    @currentTarget = @SelectTarget(self);
  if (@currentTarget == null)
    level.NextTurn();
  else
  {
    if (self.HasLineOfSight(currentTarget.AsObject()))
    {
      int   actionPoints = self.GetActionPoints();
      int   actionPointsCost;
      Item@ equiped1 = self.GetEquipedItem(0);
      Item@ equiped2 = self.GetEquipedItem(1);
      Data  data1    = equiped1.AsData();
      Data  data2    = equiped2.AsData();
      bool  suitable1,  suitable2;
      int   distance = currentTarget.GetDistance(self.AsObject());

      suitable1 = data1["range"].AsFloat() > distance;
      suitable2 = data2["range"].AsFloat() > distance;
      if (suitable1 && suitable2)
      {
        int firstNShots = data1["ap-cost"].AsInt() / actionPoints;
        int secndNShots = data2["ap-cost"].AsInt() / actionPoints;

        if (firstNShots * data1["damage"].AsInt() > secndNShots * data2["damage"].AsInt())
          suitable2 = false;
        else
          suitable1 = false;
      }
      if (suitable1)
        actionPointsCost = data1["ap-cost"].AsInt();
      else if (suitable2)
        actionPointsCost = data2["ap-cost"].AsInt();
      if (suitable1 || suitable2)
      {
        Write("IA can hit if haz action points");
        Write("Action point cost: " + actionPointsCost);
        if (actionPoints >= actionPointsCost)
          level.ActionUseWeaponOn(self, currentTarget, (suitable1 ? equiped1 : equiped2));
        else
          level.NextTurn();
      }
      else
      {
        self.GoTo(level.GetPlayer().AsObject(), 1);
        self.TruncatePath(1);
      }
    }
    else
      self.GoTo(level.GetPlayer().AsObject(), 1); 
  }
}

/*void combat(Character@ self)
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
}*/
