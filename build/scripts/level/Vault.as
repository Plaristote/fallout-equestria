ScheduledTask intro_dialog_task;

void execute_intro_dialog()
{
  Data       data_engine          = level.GetDataEngine();
  Data       first_cinematic_done = data_engine["first-cinematic-done"];
  int        cinematic_step       = first_cinematic_done.AsInt();
  int        wait_time            = 0;
  int        mood                 = 2; // 0 to 4, from bad to cheerfull with 2 being neutral
  string     answer               = "";
  Character@ speaker;
  bool       finished             = false;

  // Overmare
  if (cinematic_step < 4)
  {
    @speaker   = level.GetCharacter("Overmare");
    if (cinematic_step == 1)
      answer  = "The stable isn't doing well. The water ship won't hold much longer. We lack spare parts.";
    else if (cinematic_step == 2)
      answer  = "This is a time of dire need and we need the full support of our fellow citizen.";
    else if (cinematic_step == 3)
      answer  = "We do not have time to discuss such matters. We need to stand strong.";
  }
  // Player Character
  else if (cinematic_step > 3 && cinematic_step < 7)
  {
    @speaker  = level.GetPlayer();
    // If the player is not intelligent enough, drop the regular line and skip this part
    if (cinematic_step == 4)
    {
      if ((speaker.GetStatistics()["Special"]["INT"].AsInt()) < 7)
      {
        answer = "But why won't we open the door then ?";
        cinematic_step += 2;
      }
      else
        answer = "But why are we not opening the door ? I mean...";
    }
    else if (cinematic_step == 5)
      answer = "... the stable was built near an industrial zone.";
    else if (cinematic_step == 6)
      answer = "I'm sure we would find all the spare parts we need in no time.";
  }
  // Overmare
  else if (cinematic_step == 7)
  {
    @speaker = level.GetCharacter("Overmare");
    if ((speaker.GetStatistics()["Special"]["INT"].AsInt()) < 7)
      answer = "Why such a risk ? We don't even know if we can find the parts we need.";
    else
      answer = "I can't just open that door. Goddess knows what horrors awaits us out there.";
  }
  // Security Chief
  else if (cinematic_step > 8 && cinematic_step < 12)
  {
    @speaker = level.GetCharacter("SecurityChief");
    mood    = 3;
    if (cinematic_step == 8)
      answer  = "Either way " + level.GetPlayer().GetName() + ", we need to know what they're saying.";
    else if (cinematic_step == 9)
      answer  = "I can't send any of my ponies, everypony knows us.";
    else if (cinematic_step == 10)
      answer  = "But you'll blend in just fine.";
    else if (cinematic_step == 11)
      answer  = "Just go find those troublemakers and find out what they're planning.";
  }
  // Overmare
  else if (cinematic_step > 11 && cinematic_step < 14)
  {
    @speaker = level.GetCharacter("Overmare");
    mood    = 3;
    if (cinematic_step == 12)
      answer = "I assure you we would be very grateful if you were to comply.";
    if (cinematic_step == 13)
      answer = "Perhaps we could even solve your issues with ++insert plot device++.";
  }
  // Player
  else if (cinematic_step == 14)
  {
    @speaker = level.GetPlayer();
    mood    = 1;
    answer  = "I see.";
  }
  // Dialog finished
  else
    finished = true;

  if (@speaker != null)
  {
    int   red, green, blue;
    float answer_time = 2 + (answer.length() / 24.0);

    if      (mood == 0) { red = 255 ; green = 0   ; blue = 0   ; }
    else if (mood == 1) { red = 255 ; green = 100 ; blue = 100 ; }
    else if (mood == 2) { red = 255 ; green = 255 ; blue = 255 ; }
    else if (mood == 3) { red = 140 ; green = 255 ; blue = 33  ; }
    else if (mood == 4) { red = 151 ; green = 186 ; blue = 255 ; }

    speaker.AsObject().AddTextBox(answer, red, green, blue, answer_time);
    wait_time += ceil(answer_time);
  }

  data_engine["first-cinematic-done"] = cinematic_step + 1;
  if (finished == true)
  {
    level.AppendToConsole("Dialog is FINISHED you hear ? FINISHED !");
    intro_dialog_task.Interrupt();
  }
  else
  {
    level.AppendToConsole("This conversation isn't over");
    intro_dialog_task.SetIntervalDuration(wait_time);
  }
}

void initialize()
{
  Data data_engine = level.GetDataEngine();
  
  // If the first cut-scene has never been ran before, run it
  //if (data_engine["first-cinematic-done"].AsInt() == 0)
  //{
    data_engine["first-cinematic-done"] = 1;
    level.GetZoneManager().InsertObjectInZone(level.GetPlayer().AsObject(), "demo-intro-entry");
    level.GetPlayer().AsObject().AddTextBox("coucou tu veux voir ma bite", 255, 255, 255, 5);
    intro_dialog_task.SetCallback("execute_intro_dialog");
    intro_dialog_task.SetAsRepetitiveTask(true);
    intro_dialog_task.SetIntervalDuration(4);
    intro_dialog_task.Launch();
  //}
}