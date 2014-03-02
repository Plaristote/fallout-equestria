#ifndef  TASK_METABOLISM_HPP
# define TASK_METABOLISM_HPP

# include "globals.hpp"
# include "scheduled_task.hpp"
# include "cmap/statcontroller.hpp"

class Metabolism : public ScheduledTask
{
public:
  Metabolism(StatController* target) : target(target)
  {
    SetAsDailyTask(DateTime::Hours(13), DateTime::Minutes(37));
    Launch();
  }

protected:
  void      Run(void)
  {
    target->RunMetabolism();
    ScheduledTask::Run();
  }

private:
  StatController* target;
};

#endif