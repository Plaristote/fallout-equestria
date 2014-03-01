#include "pipbuck/clock_app.hpp"
#include "directory.hpp"
#include "level/level.hpp"
#include "gametask.hpp"

using namespace std;

PipbuckClockApp::PipbuckClockApp(Data script) : _appid(script.Key()), _time_manager(GameTask::CurrentGameTask->GetTimeManager())
{
  EventWait.EventReceived.Connect            (*this, &PipbuckClockApp::Wait);
  EventWaitMore.EventReceived.Connect        ([this](Rocket::Core::Event&) { WaitChange(1);  });
  EventWaitLess.EventReceived.Connect        ([this](Rocket::Core::Event&) { WaitChange(-1); });
  EventTimeUnitSelected.EventReceived.Connect(*this, &PipbuckClockApp::SelectTimeUnit);
}

bool PipbuckClockApp::Started(DataEngine& de)
{
  _year = _month = _day = _hour = _minute = _second = 0;
  return (Filesystem::FileContent("data/pipbuck_clock.rml", _inner_rml));
}

void PipbuckClockApp::Focused(Rocket::Core::Element* root, DataEngine& de)
{
  _minutes_to_spend = 0;
  
  root->SetInnerRML(_inner_rml.c_str());
  
  _span_error = root->GetElementById("span-error");

  _year    = root->GetElementById("value-year");
  _month   = root->GetElementById("value-month");
  _day     = root->GetElementById("value-day");
  _hour    = root->GetElementById("value-hour");
  _minute  = root->GetElementById("value-minute");
  _second  = root->GetElementById("value-second");

  _wait_days    = root->GetElementById("value-wait-days");
  _wait_hours   = root->GetElementById("value-wait-hours");
  _wait_minutes = root->GetElementById("value-wait-minutes");
  
  _wait_more    = root->GetElementById("wait-more");
  _wait_less    = root->GetElementById("wait-less");
  _do_wait      = root->GetElementById("button-wait");
  
  if (_wait_days && _wait_hours && _wait_minutes)
  {
    _wait_days->AddEventListener   ("click", &EventTimeUnitSelected);
    _wait_hours->AddEventListener  ("click", &EventTimeUnitSelected);
    _wait_minutes->AddEventListener("click", &EventTimeUnitSelected);
    _selected_wait = 0;
  }
  if (_wait_more && _wait_less && _do_wait)
  {
    _wait_more->AddEventListener("click", &EventWaitMore);
    _wait_less->AddEventListener("click", &EventWaitLess);
    _do_wait->AddEventListener  ("click", &EventWait);
  }
}

void PipbuckClockApp::Exited(DataEngine& de)
{
  Unfocused(de);
}

void PipbuckClockApp::Unfocused(DataEngine& de)
{
  if (_wait_days && _wait_hours && _wait_minutes)
  {
    _wait_days->RemoveEventListener   ("click", &EventTimeUnitSelected);
    _wait_hours->RemoveEventListener  ("click", &EventTimeUnitSelected);
    _wait_minutes->RemoveEventListener("click", &EventTimeUnitSelected);
    _selected_wait = 0;
  }
  if (_wait_more && _wait_less && _do_wait)
  {
    _wait_more->RemoveEventListener("click", &EventWaitMore);
    _wait_less->RemoveEventListener("click", &EventWaitLess);
    _do_wait->RemoveEventListener  ("click", &EventWait);
  }
  _year = _month = _day = _hour = _minute = _second = _span_error = 0;  
}

void PipbuckClockApp::SelectTimeUnit(Rocket::Core::Event& event)
{
  Rocket::Core::Element* new_unit = event.GetCurrentElement();

  if (_selected_wait)
    _selected_wait->SetProperty("color", "white");
  _selected_wait = new_unit;
  _selected_wait->SetProperty("color", "yellow");
}

void PipbuckClockApp::WaitChange(int change)
{
  if (_selected_wait)
  {
    Rocket::Core::Variant* var = _selected_wait->GetAttribute("data-value");
    
    if (var)
    {
      int          value = var->Get<int>();
      stringstream strvalue;

      value += change;
      if (value >= 0)
      {
        strvalue << value;
        _selected_wait->SetAttribute<int>("data-value", value);
        _selected_wait->SetInnerRML(strvalue.str().c_str());
      }
    }
  }
}

void PipbuckClockApp::Wait(Rocket::Core::Event&)
{
  Rocket::Core::Variant* var_days    = _wait_days->GetAttribute("data-value");
  Rocket::Core::Variant* var_hours   = _wait_hours->GetAttribute("data-value");
  Rocket::Core::Variant* var_minutes = _wait_minutes->GetAttribute("data-value");
  
  if (var_days && var_hours && var_minutes)
  {
    int ndays    = var_days->Get<int>();
    int nhours   = var_hours->Get<int>();
    int nminutes = var_minutes->Get<int>();
    
    _minutes_to_spend = ndays * 1440 + nhours * 60 + nminutes;
  }
}

void PipbuckClockApp::SetValue(Rocket::Core::Element* element, unsigned short value)
{
  stringstream strval;
  
  strval << value;
  element->SetInnerRML(strval.str().c_str());
}

void PipbuckClockApp::RunAsMainTask(Rocket::Core::Element* root, DataEngine& de)
{
  DateTime current_time = _time_manager.GetDateTime();

  if (_year)   SetValue(_year,   current_time.GetYear());
  if (_month)  SetValue(_month,  current_time.GetMonth());
  if (_day)    SetValue(_day,    current_time.GetDay());
  if (_hour)   SetValue(_hour,   current_time.GetHour());
  if (_minute) SetValue(_minute, current_time.GetMinute());
  if (_second) SetValue(_second, current_time.GetSecond());
  if (_minutes_to_spend > 0)
  {
     if (Level::CurrentLevel != 0)
     {
       if (Level::CurrentLevel->GetState() == Level::Fight)
       {
        _span_error->SetInnerRML(i18n::T("You can't wait while fighting").c_str());
        _minutes_to_spend = 0;
        return ;
       }
       Level::CurrentLevel->SetState(Level::Interrupted);
     }
     _span_error->SetInnerRML("");
     if (_minutes_to_spend > 60)
     {
         _time_manager.AddElapsedTime(DateTime::Hours(1));
       _minutes_to_spend -= 60;
     }
     else
     {
       _time_manager.AddElapsedTime(DateTime::Minutes(1));
       _minutes_to_spend--;
     }
     if (_minutes_to_spend == 0 && Level::CurrentLevel != 0)
       Level::CurrentLevel->SetState(Level::Normal);
  }
}