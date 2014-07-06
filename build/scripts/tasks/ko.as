void start(Task@ task)
{
  task.SetAsRepetitiveTask(true);
  task.SetIntervalDuration(1);
  task.Launch();
  initialize_ko_duration(get_target(task).GetStatistics(), 5);
}

void finalize(Task@ task)
{
  Character@ target     = @get_target(task);
  Data       statistics = target.GetStatistics();
  
  statistics["Variables"]["KO-duration"].Remove();
  statistics["Variables"]["Immobilized"].Remove();
}

void run(Task@ task)
{
  Character@ target      = @get_target(task);
  Data       statistics  = target.GetStatistics();
  int        ko_duration = statistics["Variables"]["KO-duration"].AsInt();

  if (ko_duration > 0)
  {
    statistics["Variables"]["Immobilized"] = 1;
    statistics["KO-duration"]              = ko_duration - 1;
    target.SetActionPoints(0);
  }
  else
    task.Interrupt();
}

void next_turn(Task@ task)
{
  run(task);
}

Character@ get_target(Task@ task)
{
  return (task.GetTarget().AsCharacter());
}

void initialize_ko_duration(Data statistics, int duration)
{
  Data data = statistics["KO-duration"];

  if (data.Nil() || data.AsInt() < duration)
    data = duration;
  statistics["Variables"]["Immobilized"] = 1;
}