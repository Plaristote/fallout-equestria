#ifndef  PIPBUCK_CLOCK_APP
# define PIPBUCK_CLOCK_APP

# include "pipbuck/pipbuck.hpp"
# include "time_manager.hpp"

class PipbuckClockApp : public Pipbuck::App
{
public:
  PipbuckClockApp(Data script);
  ~PipbuckClockApp() {}
  
  std::string GetAppId(void) { return (_appid); }
  
  void RunAsMainTask(Rocket::Core::Element*, DataEngine&);
  void RunAsBackgroundTask(DataEngine&) {}

  bool Started(DataEngine&);
  void Exited(DataEngine&);
  void Unfocused(DataEngine&);
  void Focused(Rocket::Core::Element*, DataEngine&);
private:
  const std::string _appid;
  TimeManager&      _time_manager;
  std::string       _inner_rml;
  
  RocketListener        EventWaitMore, EventWaitLess, EventTimeUnitSelected, EventWait;
  
  void                  WaitChange(int chance);
  void                  Wait(Rocket::Core::Event&);
  void                  SelectTimeUnit(Rocket::Core::Event&);
  
  void                  SetValue(Rocket::Core::Element* element, unsigned short value);

  Rocket::Core::Element *_year, *_month, *_day, *_hour, *_minute, *_second;
  Rocket::Core::Element *_wait_days, *_wait_hours, *_wait_minutes;
  Rocket::Core::Element *_selected_wait;
  Rocket::Core::Element *_wait_more, *_wait_less, *_do_wait;
  Rocket::Core::Element *_span_error;
  
  unsigned int          _minutes_to_spend;
};

#endif