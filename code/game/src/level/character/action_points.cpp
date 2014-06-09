#include "level/characters/action_points.hpp"
#include "level/level.hpp"

using namespace std;

void CharacterActionPoints::SetActionPoints(unsigned short value)
{
  controller->SetActionPoints(value);
}

void CharacterActionPoints::RefreshActionPoints(void)
{
  if (_level->GetState() == Level::Fight)
  {
    Data variables     = controller->GetData()["Variables"];
    Data left_to_spend = variables["Action Points Left To Spend"];
    
    if (left_to_spend.Or(0) > GetMaxActionPoints())
    {
      left_to_spend              = left_to_spend.Or(0) - GetMaxActionPoints();
      SetActionPoints(0);
    }
    else
    {
      SetActionPoints(GetMaxActionPoints() - left_to_spend.Or(0));
      left_to_spend = 0;
    }
  }
  else
    SetActionPoints(GetMaxActionPoints());
  RemoveArmorClassBonusFromActionPoints();
}

bool CharacterActionPoints::UseActionPoints(unsigned short amount)
{
  Data variables     = controller->GetData()["Variables"];
  Data action_points = variables["Action Points"];

  if (amount > action_points.Or(GetMaxActionPoints()))
  {
    if (GetActionPoints() == GetMaxActionPoints())
    {
      SetActionPoints(0);
      variables["Action Points Left To Spend"] = amount - GetMaxActionPoints();
    }
    else if (this == _level->GetPlayer())
    {
      _level->GetLevelUi().GetMainBar().AppendToConsole(i18n::T("You do not have enough action points."));
      return (false);
    }
  }
  else
    SetActionPoints(action_points.Or(GetMaxActionPoints()) - amount);
  return (true);
}

unsigned short CharacterActionPoints::GetMaxActionPoints(void) const
{
  return (controller->GetData()["Statistics"]["Action Points"].Or(0));
}

unsigned short CharacterActionPoints::GetActionPoints(void) const
{
  if (!(_level->GetState() == Level::Fight))
    return (GetMaxActionPoints());
  return (controller->GetData()["Variables"]["Action Points"].Or(0));
}

void CharacterActionPoints::ConvertRemainingActionPointsToArmorClass(void)
{
  int action_points = GetActionPoints();
  
  if (_level->GetState() == Level::Fight && action_points > 0)
  {
    Data variables = controller->GetData()["Variables"];
    Data bonus     = variables["Armor Class Bonus From Action Points"];

    bonus                    = action_points;
    controller->SetArmorClass(controller->Model().GetArmorClass() + (int)bonus);
  }
}

void CharacterActionPoints::RemoveArmorClassBonusFromActionPoints(void)
{
  Data variables = controller->GetData()["Variables"];
  Data bonus     = variables["Armor Class Bonus From Action Points"];
  
  if (bonus.NotNil())
  {
    controller->SetArmorClass(controller->Model().GetArmorClass() - (int)bonus);
    bonus.Remove();
  }
}
