
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
// Skill effects on characters
//
bool UseSkill(DynamicObject@ target, Character@ user, string skill)
{
  Cout("Character/UseSkill");
  Character@ self = target.AsCharacter();

  if (skill == "Steal")
  {
    Cout("SCRIPT: Steal skill has been used");
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
  if (!(self.IsMoving()) && follow_char_timer1 > 10)
  {
    self.GoTo(to_follow.AsObject(), 2);
    follow_char_timer1 = 0;
    follow_char_timer2 = 0;
    return ;
  }
  if (!(self.IsMoving()) && (follow_char_timer2 > 3))
  {
    self.GoToRandomWaypoint();
    follow_char_timer2 = 0;
  }
}
