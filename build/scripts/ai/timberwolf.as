#include "general_pony.as"

Timer myTimer;
int   movCount     = 0;
int   initWaypoint = 0;

bool shieldCasted = false;

void main(Character@ self, float elaspedTime)
{
  if (!(self.IsMoving()) && myTimer.GetElapsedTime() > 2.5)
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
  }
}

Character@ currentTarget = null;

Character@ SelectTarget(Character@ self)
{
  CharacterList         enemies  = self.GetNearbyEnemies();
  int                   nEnemies = enemies.Size();
  int                   it       = 0;
  Character@            bestMatch;

  while (it < nEnemies)
  {
    Character@ current = enemies[it];

    if (@bestMatch == null || current.GetHitPoints() < bestMatch.GetHitPoints())
      @bestMatch = @current;
 
    it++;
  }
  return (bestMatch);
}

void MyOutput(string str)
{
  Cout(str);
  Write(str);
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

      Item@  bestEquipedItem;
      int    bestAction = -1;

      int    nActions = data1["actions"].Count();
      int    cAction  = 0;
      Data   cData    = data1;
      Item@  cItem    = @equiped1;

      while (cAction < nActions)
      {
        Data action   = cData["actions"][cAction];

        // If it's a combative action
        if (action["combat"].AsInt() == 1)
        {
          // If we're in range
          if (action["range"].AsFloat() > distance)
          {
            bool setAsBest = true;

            // If there's an action to compare this one to
            if (bestAction >= 0)
            {
              Data dataBestAction   = bestEquipedItem.AsData()["actions"][bestAction];
              int  bestActionNShots = dataBestAction["ap-cost"].AsInt() / actionPoints;
              int  curActionNShots  = action["ap-cost"].AsInt()         / actionPoints;

              if (bestActionNShots * dataBestAction["damage"].AsInt() >= curActionNShots * action["damage"].AsInt())
                setAsBest = false;
            }
            if (setAsBest)
            {
              @bestEquipedItem = @cItem;
              bestAction       = cAction;
            }
          }
        }
        cAction++;
      }

      if (@bestEquipedItem != null)
      {
        actionPointsCost = bestEquipedItem.AsData()["actions"][bestAction]["ap-cost"].AsInt();
        if (actionPoints >= actionPointsCost)
          level.ActionUseWeaponOn(self, currentTarget, bestEquipedItem, bestAction);
        else
          level.NextTurn();
      }
      else
      {
        if (currentTarget.GetPathDistance(self.AsObject()) <= 1)
          level.NextTurn();
        else
        {
          self.GoTo(currentTarget.AsObject(), 1);
          self.TruncatePath(1);
          /*if (self.GetPathSize() != 1)
            level.NextTurn();*/
        }
      }
    }
    else
    {
      //Cout("Timberwolf GoTo Player");
      self.GoTo(currentTarget.AsObject(), 1);
      self.TruncatePath(1);
      if (self.GetPathSize() != 1)
      {
        //Cout("Timberwolf didn't find any path");
        level.NextTurn();
      }
      else
        Cout("Timberwolf found a path");
    }
  }
}
