void use(DynamicObject@ self, Character@ user)
{
  Cout("Using Terminal " + self.GetName());
  self.TalkTo(user);
}

bool use_skill(DynamicObject@ self, Character@ user, string skill)
{
  if (skill == "Science")
    use_science(self, user);
  else if (skill == "Lockpick")
    return (use_lockpick(self, user));
  else
    return (false);
  return (true);
}

void use_science(DynamicObject@ self, Character@ user)
{
  Data datastore = self.GetDataStore();

  Cout("Used science !");
  datastore["unlocked"] = 1;
}

bool use_lockpick(DynamicObject@ self, Character@ user)
{
  Data datastore    = self.GetDataStore();
  bool keyhole_seen = datastore["keyhole-seen"].AsInt() == 1;
  bool is_unlocked  = datastore["unlocked"].AsInt() == 1;

  if (keyhole_seen && !is_unlocked)
  {
    level.AppendToConsole("You successfully unlocked the terminal.");
    datastore["unlocked"] = 1;
    return (true);
  }
  else
    level.AppendToConsole("That does nothing");
  return (false);
} 

bool talk_to(DynamicObject@ self, Character@ user)
{
  return (self.GetDataStore()["unlocked"].AsInt() == 1);
}

bool look_at(DynamicObject@ self, Character@ user)
{
  Data datastore       = self.GetDataStore();
  Data already_seen    = datastore["already_seen"];
  Data secret_revealed = datastore["keyhole-seen"];
  int  perception      = user.GetStatistics()["Special"]["PER"].AsInt();
  int  chance          = user.GetStatistics()["Special"]["LUC"].AsInt();

  secret_revealed = secret_revealed.AsInt() == 1 || (((Random() % 10) - chance / 2) <= perception) ? 1 : 0;
  if (user.GetDistance(self) < 8 && secret_revealed.AsInt() == 1)
  {
    if (already_seen.AsInt() == 1)
      level.AppendToConsole("Looking at it from upclose, there seems to be keyhole: it might enable the terminal.");
    else
      level.AppendToConsole("It's a terminal. It seems there's a keyhole that might enable it.");
  }
  else if (already_seen.AsInt() != 1)
    level.AppendToConsole("It's a terminal. It seems to be controlling the force-field right next to it.");
  else
    level.AppendToConsole("It's a terminal.");
  already_seen = 1;
  return (true);
}