#ifndef  MY_TIMER_HPP
# define MY_TIMER_HPP

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

  double          GetElapsedTime(void) { return (_globalClock->get_real_time() - _lastTime); }
  void			  Profile(std::string str) { cout << str+": " << GetElapsedTime() << " sec\n"; Restart(); }
  void            Restart(void)        { _lastTime = _globalClock->get_real_time();          }

private:
  double          _lastTime;
  PT(ClockObject) _globalClock;
};

#endif