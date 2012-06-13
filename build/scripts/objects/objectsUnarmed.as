
string HoovesHit(Item@ item, Character@ user, Character@ target)
{
  int ap = user.GetActionPoints();

  if (ap > 3)
  {
    user.SetActionPoints(ap - 3);
    return (user.GetName() + " hit " + target.GetName() + " for 0 hit points");
  }
  return ("Not enough Action Points");
}

string BuckHit(Item@ item, Character@ user, Character@ target)
{
  int ap = user.GetActionPoints();

  if (ap > 3)
  {
    user.SetActionPoints(ap - 3);
    return (user.GetName() + " hit " + target.GetName() + " for 0 hit points");
  }
  return ("Not enough Action Points");
}
