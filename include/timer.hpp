#ifndef  MY_TIMER_HPP
# define MY_TIMER_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include "observatory.hpp"

class Timer
{
public:
  Timer(void)
  {
    _globalClock = ClockObject::get_global_clock();
    _lastTime    = _globalClock->get_real_time();
  }

  double          GetElapsedTime(void) { return (_globalClock->get_real_time() - _lastTime); }
  void            Profile(std::string str) { cout << str+": " << GetElapsedTime() << " sec\n"; Restart(); }
  void            Restart(void)        { _lastTime = _globalClock->get_real_time();          }
  
  static void     asConstructor(void* memory) { new(memory) Timer(); }
  static void     asDestructor(void* memory)  {                      }

private:
  double          _lastTime;
  PT(ClockObject) _globalClock;
};

class TimeManager
{
public:
  struct Task
  {
    bool           loop;
    unsigned short it;

    unsigned short lastY, lastMo, lastD, lastH, lastM, lastS;
    unsigned short timeY, timeMo, timeD, timeH, timeM, timeS;
    
    void NextStep(void)
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
    
    Observatory::Signal<void>                  Interval;
    Observatory::Signal<void (unsigned short)> IntervalIt;
    
  private:
    friend class TimeManager;
    Task()  {}
    ~Task() {}
  };
  
  typedef std::list<Task*> Tasks;

  TimeManager(void)
  {
    year = month = day = hour = minute = second = 0;
    fseconds = 0.f;
  }
  
  ~TimeManager(void)
  {
    std::for_each(tasks.begin(), tasks.end(), [](Task* task) { delete task; });
  }
  
  void AddElapsedSeconds(float s)
  {
    fseconds += s;
    if (fseconds >= 1.f)
    {
      AddElapsedTime(1);
      fseconds -= 1.f;
    }
  }

  void AddElapsedTime(unsigned short s, unsigned short m = 0, unsigned short h = 0, unsigned short d = 0, unsigned short mo = 0, unsigned short y = 0)
  {
    second += s;
    minute += m;
    hour   += h;
    month  += mo;
    year   += y;
    CorrectValues();
  }
  
  static unsigned short GetDaysPerMonth(unsigned short m)
  {
    if (m == 2)
      return (29);
    return (m % 2 != 0 ? 30 : 31);
  }
  
  unsigned short GetYear(void)   const { return (year);   }
  unsigned short GetMonth(void)  const { return (month);  }
  unsigned short GetDay(void)    const { return (day);    }
  unsigned short GetHour(void)   const { return (hour);   }
  unsigned short GetMinute(void) const { return (minute); }
  unsigned short GetSecond(void) const { return (second); }
  
  Task* AddTask(bool loop, unsigned short s, unsigned short m = 0, unsigned short h = 0, unsigned short d = 0, unsigned short mo = 0, unsigned short y = 0)
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
  
  void DelTask(Task* task)
  {
    Tasks::iterator it = std::find(tasks.begin(), tasks.end(), task);
    
    if (it != tasks.end())
    {
      delete task;
      safeIt = tasks.erase(it);
    }
  }
  
  void ExecuteTasks()
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

private:
  void CorrectValues(void)
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
      month += 1;
      day -= GetDaysPerMonth(month);
      if (month > 12)
      {
	month = 1;
	year += 1;
      }
    }
  }

  float           fseconds;
  unsigned short  year, month, day, hour, minute, second;
  Tasks           tasks;
  Tasks::iterator safeIt;
};

#endif