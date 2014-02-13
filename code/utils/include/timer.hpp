#ifndef  MY_TIMER_HPP
# define MY_TIMER_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include "observatory.hpp"
# include "datetime.hpp"

# define TASK_LVL_WORLDMAP 1
# define TASK_LVL_CITY     2

class Timer
{
public:
  Timer(void)
  {
    _globalClock = ClockObject::get_global_clock();
    _lastTime    = _globalClock->get_real_time();
  }

  double          GetElapsedTime(void) const { return (_globalClock->get_real_time() - _lastTime); }
  void            Profile(std::string str);
  void            Restart(void)              { _lastTime = _globalClock->get_real_time();          }
  void            SetLastTime(double v)      { _lastTime = v; }
  float           GetLastTime(void) const    { return (_lastTime); }
  
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
    DateTime       next_run, length;
    unsigned char  level;

    void NextStep(void);

    Sync::Signal<void>                  Interval;
    Sync::Signal<void (unsigned short)> IntervalIt;
    
    void Serialize(Utils::Packet&);
    void Unserialize(Utils::Packet&);

  private:
    friend class TimeManager;
    Task()  {}
    ~Task() {}
  };

  typedef std::list<Task*> Tasks;

  TimeManager(void)
  {
    fseconds = 0.f;
  }

  ~TimeManager(void)
  {
    ClearTasks(0);
  }
  
  static unsigned short GetDaysPerMonth(unsigned short m, unsigned short year = 1)
  {
    if (m == 2)
      return (year % 4 == 0 ? 29 : 28);
    return ((m <= 7 ? m % 2 == 0 : m % 2 != 0) ? 30 : 31); 
  }
  
  void            ClearTasks(unsigned char level)
  {
    Tasks::iterator it, end;
    
    for (it = tasks.begin(), end = tasks.end() ; it != end ;)
    {
      Task* task = *it;
      
      if (task->level >= level)
      {
	delete task;
	it = tasks.erase(it);
      }
      else
	++it;
    }
  }
  
  void            SetTime(unsigned short s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
  {
    current_time = DateTime(y, mo, d, h, m, s);
  }

  unsigned short  GetYear(void)   const { return (current_time.GetYear());   }
  unsigned short  GetMonth(void)  const { return (current_time.GetMonth());  }
  unsigned short  GetDay(void)    const { return (current_time.GetDay());    }
  unsigned short  GetHour(void)   const { return (current_time.GetHour());   }
  unsigned short  GetMinute(void) const { return (current_time.GetMinute()); }
  unsigned short  GetSecond(void) const { return (current_time.GetSecond()); }

  void            AddElapsedSeconds(float s);
  void            AddElapsedTime(unsigned short s, unsigned short m = 0, unsigned short h = 0, unsigned short d = 0, unsigned short mo = 0, unsigned short y = 0);
  
  Task*           AddTask(unsigned char level, bool loop, unsigned int s, unsigned short m = 0, unsigned short h = 0, unsigned short d = 0, unsigned short mo = 0, unsigned short y = 0);
  void            DelTask(Task* task);  
  void            ExecuteTasks(void);
private:
  void            CorrectValues(void);

  float           fseconds;
  DateTime        current_time;
  Tasks           tasks;
  Tasks::iterator safeIt;
};

#endif
