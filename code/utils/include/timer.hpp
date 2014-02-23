#ifndef  MY_TIMER_HPP
# define MY_TIMER_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>

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

#endif
