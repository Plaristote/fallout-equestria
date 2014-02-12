
void initialize()
{
  Cout("INITIALIWING LEVEL IN CAPS LOCK");
  ApiDefineFunction("DailyTask", "void DailyTask()");
  ApiDefineFunction("HourlyTask", "void HourlyTask()");
  PushDailyTask("DailyTask", 12, 30, 0);
  PushHourlyTask("HourlyTask", 15, 15);
}

void DailyTask()
{
  Cout("The Daily Task HAS JUST BEEN EXECUTED LOOK AT ME THE DAILY TASK THE DAILY TASK THE DAILY TASK THE DAILY TASK THE DAILY TASK THE DAILY TASK");
}

void HourlyTask()
{
  Cout("The hourly Task HAS just been executed look at me the hourly task the hourly task THE HOURLY TASK");
}

void finalize()
{
}

void run(float elapsed_time)
{
}
