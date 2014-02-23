#include "timer.hpp"
#define PROFILING

using namespace std;

void Timer::Profile(std::string str)
{
#ifdef PROFILING
  cout << str+": " << GetElapsedTime() << " sec\n"; Restart();
#endif
}
