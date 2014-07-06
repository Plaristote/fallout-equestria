#include "scheduled_task.hpp"
#include "time_manager.hpp"
#include "circular_value_set.hpp"
#include "executor.hpp"

using namespace std;

ScheduledTask::ScheduledTask() :
  time_manager(*TimeManager::CurrentTimeManager),
  task(0),
  interval_duration(1),
  interval_until_next_execution(0),
  is_repetitive(false),
  is_being_executed(false),
  is_being_removed(false),
  task_level(TASK_LVL_WORLDMAP)
{
}

ScheduledTask::~ScheduledTask()
{
  // Do not remove while the task is being executed.
  // Asynchronous removal (see EndActiveTask) requires this object to exist.
  EndPassiveTask();
}

void ScheduledTask::Launch()
{
  is_being_removed = false;
  Schedule();
}

void ScheduledTask::Interrupt()
{
  EndTask();
}

void ScheduledTask::Schedule()
{
  if (task == 0) // Don't schedule a task if one is already running
  {
    if (is_repetitive)
      task = time_manager.AddRepetitiveTask(task_level, interval_until_next_execution);
    else
      task = time_manager.AddTask(task_level, interval_until_next_execution);
    task->Interval.Connect(*this, &ScheduledTask::IntervalCallback);
  }
  else
    task->next_run = time_manager.GetDateTime() + interval_until_next_execution;
}

void ScheduledTask::IntervalCallback()
{
  is_being_executed = true;
  Run();
  is_being_executed = false;
}

void ScheduledTask::EndTask()
{
  if (is_being_executed)
    EndActiveTask();
  else
    EndPassiveTask();
}

void ScheduledTask::EndPassiveTask()
{
  if (task)
  {
    time_manager.DelTask(task);
    //delete task; // TODO Task removal should be the responsability of whoever created it
    task = 0;
  }
}

void ScheduledTask::EndActiveTask()
{
  is_being_removed = true;
  Executor::ExecuteLater([this]()
  {
    if (is_being_removed == true) // Abort removal if is_being_removed has been unset
      EndPassiveTask();
  });
}

void ScheduledTask::SetAsRepetitiveTask(bool is_repetitive)
{
  this->is_repetitive = is_repetitive;
  if (task)
    task->loop = is_repetitive;
}

void ScheduledTask::Run()
{
  if (!is_repetitive)
    EndTask();
  else if (interval_duration.seconds != interval_until_next_execution.seconds)
    RefreshIntervalOnTaskObject();
}

void ScheduledTask::RefreshIntervalOnTaskObject()
{
  task->length = DateTime(interval_duration);
}

void ScheduledTask::SetIntervalDuration(DateTime::TimeUnit time)
{
  interval_duration = interval_until_next_execution = time;
  if (task)
    RefreshIntervalOnTaskObject();
}

void ScheduledTask::SetIntervalDurationInSeconds(unsigned int seconds)
{
  interval_duration.seconds = interval_until_next_execution.seconds = seconds;
  if (task)
    RefreshIntervalOnTaskObject();
}

void ScheduledTask::SetAsHourlyTask(DateTime::Minutes minute)
{
  CircularValueSet value_set(DateTime::Hours(1).seconds);
  DateTime         current_time = time_manager.GetDateTime();
  unsigned int     current_second, target_second;

  current_second                = DateTime::Minutes(current_time.GetMinute()).seconds + current_time.GetSecond();
  target_second                 = minute.seconds;
  interval_until_next_execution = DateTime::Seconds(value_set.AdditionDistance(target_second, current_second));
  interval_duration             = DateTime::Hours(1);
  SetAsRepetitiveTask(true);
}

void ScheduledTask::SetAsDailyTask(DateTime::Hours hour, DateTime::Minutes minute)
{
  CircularValueSet value_set(DateTime::Days(1).seconds);
  DateTime         current_time = time_manager.GetDateTime();
  unsigned int     current_second, target_second;

  current_second                = current_time.GetTimeOfTheDay().seconds;
  target_second                 = hour.seconds + minute.seconds;
  interval_until_next_execution = DateTime::Seconds(value_set.AdditionDistance(target_second, current_second));
  interval_duration             = DateTime::Days(1);
  SetAsRepetitiveTask(true);
}

void ScheduledTask::SetAsWeeklyTask(unsigned short day_of_the_week, DateTime::TimeUnit time_of_the_day)
{
  CircularValueSet value_set(DateTime::Weeks(1).seconds);
  DateTime         current_time            = time_manager.GetDateTime();
  unsigned int     current_second, target_second;

  current_second                = current_time.GetTimeOfTheWeek().seconds;
  target_second                 = DateTime::Days(day_of_the_week - 1).seconds + time_of_the_day.seconds;
  interval_until_next_execution = DateTime::Seconds(value_set.AdditionDistance(target_second, current_second));
  interval_duration             = DateTime::Weeks(1);
  SetAsRepetitiveTask(true);
}

void ScheduledTask::Serialize(Utils::Packet& packet)
{
  unsigned int duration_1 = interval_duration.seconds;
  unsigned int duration_2 = interval_until_next_execution.seconds;
  bool         has_task   = task != 0;

  packet << has_task;
  if (has_task)
    task->Serialize(packet);
  packet << duration_1 << duration_2;
}

void ScheduledTask::Unserialize(Utils::Packet& packet)
{
  bool has_task;

  if (!task)
    task        = time_manager.AddTask(0, DateTime::Seconds(1));
  packet >> has_task;
  if (has_task)
  {
    task->Unserialize(packet);
    is_repetitive = task->loop;
    task_level    = task->level;
  }
  packet >> interval_duration.seconds >> interval_until_next_execution.seconds;
}
