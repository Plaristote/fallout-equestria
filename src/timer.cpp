#include "timer.hpp"

using namespace std;

void TimeManager::AddElapsedSeconds(float s)
{
  fseconds += s;
  if (fseconds >= 1.f)
  {
    AddElapsedTime(1);
    fseconds -= 1.f;
  }
}

void TimeManager::AddElapsedTime(unsigned short s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
{
  second += s;
  minute += m;
  hour   += h;
  month  += mo;
  year   += y;
  CorrectValues();
}

TimeManager::Task* TimeManager::AddTask(bool loop, unsigned short s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
{
  Task* task = new Task;
  
  task->loop   = loop;
  task->it     = 0;
  task->timeY  = y;
  task->timeMo = mo;
  task->timeD  = d;
  task->timeH  = h;
  task->timeM  = m;
  task->timeS  = s;
  task->lastY  = year   + y;
  task->lastMo = month  + mo;
  task->lastD  = day    + d;
  task->lastH  = hour   + h;
  task->lastM  = minute + m;
  task->lastS  = second + s;
  tasks.push_back(task);
  return (task);
}

void TimeManager::DelTask(Task* task)
{
  Tasks::iterator it = std::find(tasks.begin(), tasks.end(), task);
  
  if (it != tasks.end())
  {
    delete task;
    safeIt = tasks.erase(it);
  }
}

void TimeManager::ExecuteTasks(void)
{
  Tasks::iterator it  = tasks.begin();
  Tasks::iterator end = tasks.end();

  while (it != end)
  {
    Task&          task = **it;

    if (task.lastY <= year && task.lastMo <= month && task.lastD <= day && task.lastM <= minute && task.lastS <= second)
    {
      safeIt = it;
      task.Interval.Emit();
      task.IntervalIt.Emit(++task.it);
      if (safeIt != it)
      {
	it = safeIt;
	continue ;
      }
      if (!(task.loop))
      {
	it = tasks.erase(it);
	continue ;
      }
      task.NextStep();
    }
    ++it;
  }
}

void TimeManager::CorrectValues(void)
{
  if (second > 59)
  {
    minute += (second / 60);
    second  = second % 60;
  }
  if (minute > 59)
  {
    hour += (minute / 60);
    minute= minute % 60;
  }
  if (hour > 23)
  {
    day += (hour / 24);
    hour = hour % 24;
  }
  if (month > 12)
  {
    year += (month / 13);
    month = month % 13;
  }
  while (day > GetDaysPerMonth(month))
  {
    day -= GetDaysPerMonth(month);
    month += 1;
    if (month > 12)
    {
      month = 1;
      year += 1;
    }
  }
}

/*
 * TimeManager::Task
 */
void TimeManager::Task::NextStep(void)
{
  lastY  += timeY;
  lastMo += timeMo;
  lastD  += timeD;
  lastM  += timeM;
  lastS  += timeS;
  
  if (lastS > 59)
  {
    lastM += (lastS / 60);
    lastS  = lastS % 60;
  }
  if (lastM > 59)
  {
    lastH+= (lastM / 60);
    lastM = lastM % 60;
  }
  if (lastH > 23)
  {
    lastD += (lastH / 24);
    lastH = lastH % 24;
  }
  if (lastMo > 12)
  {
    lastY += (lastMo / 13);
    lastMo = lastMo % 13;
  }
  while (lastD > TimeManager::GetDaysPerMonth(lastMo))
  {
    lastMo += 1;
    lastD -= TimeManager::GetDaysPerMonth(lastMo);
    if (lastMo > 12)
    {
      lastMo = 1;
      lastY  += 1;
    }
  }
}
