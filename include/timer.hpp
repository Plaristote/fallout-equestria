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

    void NextStep(void);

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
    ClearTasks();
  }
  
  static unsigned short GetDaysPerMonth(unsigned short m)
  {
    if (m == 2)
      return (29);
    return (m % 2 != 0 ? 30 : 31);
  }
  
  void            ClearTasks(void)
  {
    std::for_each(tasks.begin(), tasks.end(), [](Task* task) { delete task; });
    tasks.clear();
  }
  
  void            SetTime(unsigned short s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
  {
    year = y; month = mo; day = d; hour = h; minute = m; second = s;
  }

  unsigned short  GetYear(void)   const { return (year);   }
  unsigned short  GetMonth(void)  const { return (month);  }
  unsigned short  GetDay(void)    const { return (day);    }
  unsigned short  GetHour(void)   const { return (hour);   }
  unsigned short  GetMinute(void) const { return (minute); }
  unsigned short  GetSecond(void) const { return (second); }

  void            AddElapsedSeconds(float s);
  void            AddElapsedTime(unsigned short s, unsigned short m = 0, unsigned short h = 0, unsigned short d = 0, unsigned short mo = 0, unsigned short y = 0);
  
  Task*           AddTask(bool loop, unsigned short s, unsigned short m = 0, unsigned short h = 0, unsigned short d = 0, unsigned short mo = 0, unsigned short y = 0);
  void            DelTask(Task* task);  
  void            ExecuteTasks(void);
private:
  void            CorrectValues(void);

  float           fseconds;
  unsigned short  year, month, day, hour, minute, second;
  Tasks           tasks;
  Tasks::iterator safeIt;
};

#endif