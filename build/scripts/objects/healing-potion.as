#include "objectsItems.as"

bool DrinkPotion(Item@ item, Character@ user)
{
  return (GivePotion(item, user, user));
}

bool GivePotion(Item@ item, Character@ user, Character@ target)
{
  Data   item_data        = item.AsData();
  Data   action           = item_data["actions"]["Use on"];
  int    ap_cost          = action["ap-cost"].AsInt();

  if (user.GetActionPoints() >= ap_cost)
  {
    Data   statistics     = user.GetStatistics();
    string skill          = action["Skill"].AsString();
    int    max_hit_points = statistics["Statistics"]["Hit Points"].AsInt();
    int    skill_points   = statistics["Skills"][skill].AsInt();
    int    luck_factor    = (Random() % 50) + 50;             // Between 50 and 100.
    int    heal_percents  = (luck_factor + skill_points) / 2; // How much of the original HP heal value will be healed
    int    previous_hp    = target.GetHitPoints();
    int    hit_points     = target.GetHitPoints() + ((30 * heal_percents) / 100);

    if (hit_points >= max_hit_points)
      hit_points = max_hit_points;
    target.SetHitPoints(hit_points);
    level.PlaySound("use/healing-potion");
    level.ConsoleWrite(target.GetName() + " was healed for " + (-(previous_hp - hit_points)) + " hit points.");
    user.SetActionPoints(user.GetActionPoints() - ap_cost);
    user.GetInventory().DelObject(item);
    return (true);
  }
  else if (@user == @level.GetPlayer())
    level.ConsoleWrite("You don't have enough action points.");
  return (false);
}
