#ifndef  SCHEDULED_TASK_HPP
# define SCHEDULED_TASK_HPP

# include "time_manager.hpp"

class ScheduledTask
{
public:
  ScheduledTask();
  virtual ~ScheduledTask();

  void               SetAsLevelSpecificTask(void)  { task_level = TASK_LVL_CITY;     }
  void               SetAsGlobalTask(void)         { task_level = TASK_LVL_WORLDMAP; }
  void               SetAsRepetitiveTask(bool is_repetitive);
  void               SetIntervalDuration(DateTime::TimeUnit time);
  void               SetIntervalDurationInSeconds(unsigned int seconds);

  void               SetAsWeeklyTask(unsigned short day_of_the_week, DateTime::TimeUnit time_of_the_day);
  void               SetAsDailyTask(DateTime::Hours hour, DateTime::Minutes minute);
  void               SetAsHourlyTask(DateTime::Minutes minute);

  void               Launch(void);
  void               Interrupt(void);

protected:
  virtual void       Run(void);
  void               EndTask(void);
  const TimeManager& GetTimeManager(void) const { return (time_manager); }

private:
  void               Schedule(void);
  void               IntervalCallback(void);
  void               EndActiveTask(void);
  void               EndPassiveTask(void);
  void               RefreshIntervalOnTaskObject(void);

  TimeManager&       time_manager;
  TimeManager::Task* task;
  DateTime::TimeUnit interval_duration, interval_until_next_execution;
  bool               is_repetitive, is_being_executed, is_being_removed;
  unsigned char      task_level;
};

#endif
