
string HoovesHit(Item@ item, Character@ user, Character@ target)
{
  Write("Trololo");
  int ap = user.GetActionPoints();

  if (ap >= 3)
  {
    int damage = 5;

    user.SetActionPoints(ap - 3);
    target.SetHitPoints(target.GetHitPoints() - damage);
    return (user.GetName() + " hit " + target.GetName() + " for 5 hit points");
  }
  return ("Not enough Action Points");
}

string BuckHit(Item@ item, Character@ user, Character@ target)
{
  int ap = user.GetActionPoints();

  if (ap >= 3)
  {
    int damage = 10;

    user.SetActionPoints(ap - 3);
    target.SetHitPoints(target.GetHitPoints() - damage);
    return (user.GetName() + " hit " + target.GetName() + " for 10 hit points");
  }
  return ("Not enough Action Points");
}
