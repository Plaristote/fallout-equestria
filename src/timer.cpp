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
  day    += d;
  month  += mo;
  year   += y;
  CorrectValues();
}

TimeManager::Task* TimeManager::AddTask(unsigned char level, bool loop, unsigned short s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
{
  Task* task = new Task;

  task->level  = level;
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
  cout << "[" << task << "] Del Task" << endl;
  Tasks::iterator it = std::find(tasks.begin(), tasks.end(), task);
  
  if (it != tasks.end())
  {
    delete task;
    safeIt = tasks.erase(it);
  }
}

void TimeManager::ExecuteTasks(void)
{
  //Timer           profiler;
  Tasks::iterator it  = tasks.begin();
  Tasks::iterator end = tasks.end();

  /*if (it != end)
  {
    Task& ftask = **it;
    cout << "FTime" << endl;
    cout << year << "/" << month << "/" << day << " ";
    cout << hour << ":" << minute  << ":" << second << endl;
    cout << "FTask" << endl;
    cout << ftask.lastY << "/" << ftask.lastMo << "/" << ftask.lastD << " ";
    cout << ftask.lastH << ":" << ftask.lastM  << ":" << ftask.lastS << endl << endl;
  }*/
  // TODO Write that shit again while not drunk.
  while (it != end)
  {
    Task&          task = **it;

    bool dostuff = true;
    if (year < task.lastY)
      dostuff = false;
    else
    {
      if (year == task.lastY && month < task.lastMo)
	dostuff = false;
      else
      {
        if (year == task.lastY && month == task.lastMo && day < task.lastD)
          dostuff = false;
        else
        {
          if (year == task.lastY && month == task.lastMo && hour < task.lastH)
            dostuff = false;
          else
          {
            if (year == task.lastY && month == task.lastMo && hour == task.lastH && minute < task.lastM)
              dostuff = false;
            else
            {
              if (year == task.lastY && month == task.lastMo && hour == task.lastH && minute == task.lastM && second < task.lastS)
                dostuff = false;
            }
          }
        }
      }
    }
    if (dostuff)
    {
      safeIt = it;
      //cout << "[" << &task << "] Executing Callbacks" << endl;
      task.Interval.Emit();
      //cout << "[" << &task << "] Executed Callbacks" << endl;
      task.IntervalIt.Emit(++task.it);
      if (safeIt != it)
      {
	//cout << "[" << &task << "] Task has been deleted" << endl;
	it = safeIt;
	continue ;
      }
      if (!(task.loop))
      {
	//cout << "[" << &task << "] Task isn't looping" << endl;
	it = tasks.erase(it);
	continue ;
      }
      task.NextStep();
      //cout << "[" << &task << "] Next step will be at: ";
      //cout << task.lastY << "/" << task.lastMo << "/" << task.lastD << " ";
      //cout << task.lastH << ":" << task.lastM  << ":" << task.lastS << endl;
    }
    else
      ++it;
  }
  //profiler.Profile("Timer");
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
    year += (month / 12);
    month = month % 12;
    if (month == 0)
    { month = 1; }
  }
  while (day > GetDaysPerMonth(month, year))
  {
    day -= GetDaysPerMonth(month, year);
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
