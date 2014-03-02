
string default_weapon_1()
{
  return ("hooves");
}

string default_weapon_2()
{
  return ("buck");
}

StringList GetAvailableApps(Data data)
{
  StringList apps;

  apps.Add("helloworld");
  apps.Add("clock");
  apps.Add("quest manager");
  return (apps);
}

//
// Looting callbacks
//
bool LootingTest(DynamicObject@ self_obj, Character@ user, Item@ item)
{
  Character@ self = self_obj.AsCharacter();

  if (self.GetHitPoints() > 0) // The character user is stealing from self
  {
    Data user_stats      = user.GetStatistics();
    Data self_stats      = self.GetStatistics();
    int  steal_skill     = user_stats["Skills"]["Steal"].AsInt();
    int  self_perception = self_stats["Special"]["PER"].AsInt();
    int  max_throw       = steal_skill - (10 + 3 * self_perception);
    int  throw           = Random() % 100;

    if (max_throw > 95)
      max_throw = 95;
    else if (max_throw < 5)
      max_throw = 5;
    if (throw <= max_throw)
      return (true);
    // If didn't return in the past line, the steal check has failed.
    self.SetAsEnemy(user, true);
    return (false);
  }
  return (true); // Self is dead. There's no need to bother him with statistic checks.
}

bool LootingFailureTest()
{
  return (true);
}

//
// Skill effects on characters
//
bool UseSkill(DynamicObject@ target, Character@ user, string skill)
{
  Cout("Character/UseSkill");
  Character@ self = target.AsCharacter();

  if (skill == "Steal")
  {
    Cout("SCRIPT: Steal skill has been used");
    StartStealing(user, target);
    return (true);
  }
  else if (skill == "Sneak")
  {
    Cout("UseSkill Sneak");
    StartSneaking(user);
    return (true);
  }
  return (false);
}

void StartStealing(Character@ user, DynamicObject@ target)
{
  Use(user, target);
}

void StartSneaking(Character@ user)
{
  user.SetFurtive(true);
}

//
// AI Template
// Follow Character
//
float follow_char_timer1;
float follow_char_timer2;

void ai_template_follow_char(Character@ self, Character@ to_follow, float elapsedTime)
{
  follow_char_timer1 += elapsedTime;
  follow_char_timer2 += elapsedTime;
  if (to_follow.GetDistance(self.AsObject()) > 30)
  {
    self.GoTo(to_follow.AsObject(), 2);
    follow_char_timer1 = 0;
    follow_char_timer2 = 0;
    return ;
  }
  if (!(self.IsMoving()) && (follow_char_timer2 > 3))
  {
    self.GoToRandomDirection();
    follow_char_timer2 = 0;
  }
}
