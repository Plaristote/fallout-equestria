#include "test.hpp"
#include "timer.hpp"
#include <sstream>

using namespace std;

TimeManager timer;

void TestsTimeManager(UnitTest& tester)
{
  tester.AddTest("TimeManager", "AddElapsedSeconds", []() -> string
  {
    timer.SetTime(0, 0, 0, 0, 0, 0); // Reset for testing
    timer.AddElapsedSeconds(0.5f);
    if (timer.GetSecond() != 0)
      return ("TimeManager added a real-time second even though a second hasn't been completed yet");
    timer.AddElapsedSeconds(0.5f);
    if (timer.GetSecond() != 1)
      return ("A full second has elapsed (2*0.5), but the time isn't 0:0:1");
    timer.AddElapsedSeconds(0.9f);
    timer.AddElapsedSeconds(0.2f);
    if (timer.GetSecond() != 2)
      return ("0.5 + 0.5 + 0.9 + 0.2 seconds returned something different to 2 seconds");
    timer.AddElapsedSeconds(0.9f);
    if (timer.GetSecond() != 3)
      return ("2.1 + 0.9 returned something different to 3 seconds");
    return ("");
  });

  tester.AddTest("TimeManager", "AddElapsedTime", []() -> string
  {
    timer.SetTime(0, 0, 0, 1, 1, 0); // Reset for testing
    timer.AddElapsedTime(72);
    if (timer.GetSecond() != 12 || timer.GetMinute() != 1)
      return ("Failed to properly compute the time elapsed from seconds");
    timer.AddElapsedTime(0, 0, 24);
    if (timer.GetHour() != 0 || timer.GetDay() != 2)
      return ("Failed to properly compute the date from elapsed hours");
    timer.AddElapsedTime(0, 0, 0, 31);
    if (timer.GetMonth() != 2 || timer.GetDay() != 2)
      return ("Failed to properly compute the date from elapsed days");
    timer.AddElapsedTime(0, 0, 0, 0, 12);
    if (timer.GetYear() != 1 || timer.GetMonth() != 2)
      return ("Failed to properly compute the date from elapsed months");
    timer.AddElapsedTime(0, 0, 0, 0, 0, 1);
    if (timer.GetYear() != 2)
      return ("Failed to properly compute the date from elapsed years");
    return ("");
  });

  tester.AddTest("TimeManager", "GetDaysPerMonth", []() -> string
  {
    unsigned short days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    for (unsigned short i = 0 ; i < 12 ; ++i)
    {
      unsigned short ret;

      if ((ret = TimeManager::GetDaysPerMonth(i + 1)) != days[i])
      {
        stringstream stream;

        stream << "Month " << (i + 1) << " has " << days[i] << " days, GetDaysPerMonth returned " << ret << endl;
        return (stream.str());
      }
    }
    return ("");
  });

  tester.AddTest("TimeManager", "DaysPerMonth (bisextile)", []() -> string
  {
    if (!((TimeManager::GetDaysPerMonth(2, 2012) == 29) && (TimeManager::GetDaysPerMonth(2, 4) == 29)))
      return ("Bisextile February doesn't have 29 days");
    if (!((TimeManager::GetDaysPerMonth(2, 2013) == 28) && (TimeManager::GetDaysPerMonth(2, 2) == 28)))
      return ("Non-bisextile February doesn't have 28 days");
    return ("");
  });
}

