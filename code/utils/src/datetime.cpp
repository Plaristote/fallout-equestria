#include "datetime.hpp"
#include <sstream>

using namespace std;

DateTime DateTime::operator+(const DateTime& date) const
{
  DateTime result(*this);

  result.year   += date.year;
  result.month  += date.month;
  result.day    += date.day;
  result.hour   += date.hour;
  result.minute += date.minute;
  result.second += date.second;
  result.adjust_values();
  return (result);
}

DateTime DateTime::operator+(unsigned int seconds) const
{
  DateTime result(*this);
  
  result.second += seconds;
  result.adjust_values();
  return (result);
}

DateTime DateTime::operator+(TimeUnit time_unit) const
{
  DateTime result(*this);

  result.second += time_unit.seconds;
  result.adjust_values();
  return (result);
}

bool DateTime::operator>=(const DateTime& time) const
{
  RecursiveChecker<int>::CompFunc comp([](int a, int b) { return (a > b); });

  return (recursive_compare(comp, time));
}

bool DateTime::operator<=(const DateTime& time) const
{
  RecursiveChecker<int>::CompFunc comp([](int a, int b) { return (a < b); });

  return (recursive_compare(comp, time));
}

bool DateTime::operator>(const DateTime& item) const
{
  return (*this != item && *this > item);
}

bool DateTime::operator<(const DateTime& item) const
{
  return (*this != item && *this < item);
}

bool DateTime::operator!=(const DateTime& item) const
{
  return (second != item.second || minute != item.minute || hour != item.hour || day != item.day || month != item.month || year != item.year);
}

bool DateTime::operator==(const DateTime& item) const
{
  return (!(*this != item));
}

std::string DateTime::ToString(const std::string& format) const
{
  std::stringstream result;

  for (unsigned int i = 0 ; i < format.size() ; ++i)
  {
    if (format[i] == '%')
    {
      short number;
      
      switch (format[i + 1])
      {
        case 'd':
          number  = day;
          break ;
        case 'M':
          number  = month;
          break ;
        case 'y':
          number  = year;
          break ;
        case 'h':
          number  = hour;
          break ;
        case 'm':
          number  = minute;
          break ;
        case 's':
          number  = second;
          break ;
        default:
          number -= 1;
          result << format[i] << format[i + 1];
          break ;
      }
      if (number > -1)
      {
        if (number < 10)
          result << '0';
        result << number;
      }
      ++i;
    }
    else
      result << format[i];
  }
  return (result.str());
}

unsigned short DateTime::days_per_months(unsigned short month, unsigned short year)
{
  if (month == 2)
    return (year % 4 == 0 ? 29 : 28);
  return ((month <= 7 ? month % 2 == 0 : month % 2 != 0) ? 30 : 31); 
}

bool DateTime::recursive_compare(RecursiveChecker<int>::CompFunc comp, const DateTime& time) const
{
  RecursiveChecker<int>::Queue    checkers;

  checkers.push(RecursiveChecker<int>(year,   time.year,   comp));
  checkers.push(RecursiveChecker<int>(month,  time.month,  comp));
  checkers.push(RecursiveChecker<int>(day,    time.day,    comp));
  checkers.push(RecursiveChecker<int>(hour,   time.hour,   comp));
  checkers.push(RecursiveChecker<int>(minute, time.minute, comp));
  checkers.push(RecursiveChecker<int>(second, time.second, comp));    
  return (checkers.front().Execute(checkers));
}

void DateTime::adjust_values(void)
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
  while (day > days_per_months(month, year))
  {
    day -= days_per_months(month, year);
    month += 1;
    if (month > 12)
    {
      month = 1;
      year += 1;
    }
  }
}

void DateTime::Serialize(Utils::Packet& packet)
{
  packet << year << month << day << hour << minute << second;
}

void DateTime::Unserialize(Utils::Packet& packet)
{
  packet >> year >> month >> day >> hour >> minute >> second;
}
