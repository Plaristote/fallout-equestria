#include "timer.hpp"
#define PROFILING

using namespace std;

void Timer::Profile(std::string str)
{
#ifdef PROFILING
  cout << str+": " << GetElapsedTime() << " sec\n"; Restart();
#endif
}

void TimeManager::AddElapsedSeconds(float s)
{
  fseconds += s;
  if (fseconds >= 1.f)
  {
    float floored = std::floor(fseconds);
    
    AddElapsedTime(floored);
    fseconds -= floored;
  }
}

void TimeManager::AddElapsedTime(unsigned short s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
{
  current_time = current_time + DateTime(y, mo, d, h, m, s);
}

TimeManager::Task* TimeManager::AddTask(unsigned char level, bool loop, unsigned int s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
{
  Task* task = new Task;

  task->level    = level;
  task->loop     = loop;
  task->it       = 0;
  task->length   = DateTime(y, mo, d, h, m, s);
  task->next_run = current_time + task->length;
  tasks.push_back(task);
  return (task);
}

void TimeManager::DelTask(Task* task)
{
  cout << "[" << task << "] Del Task" << endl;
  Tasks::iterator it = std::find(tasks.begin(), tasks.end(), task);
  
  if (it != tasks.end())
  {
    delete task;
    safeIt = tasks.erase(it);
  }
}

void TimeManager::ExecuteTasks(void)
{
  //Timer           profiler;
  Tasks::iterator it  = tasks.begin();
  Tasks::iterator end = tasks.end();

  while (it != end)
  {
    Task&          task = **it;

    if (task.next_run <= current_time)
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
    else
      ++it;
  }
  //profiler.Profile("Timer");
}

/*
 * TimeManager::Task
 */
void TimeManager::Task::NextStep(void)
{
  next_run = next_run + length;
}

void TimeManager::Task::Serialize(Utils::Packet& packet)
{
  char looping = loop;

  next_run.Serialize(packet);
  length.Serialize(packet);
  packet << level << it << looping;
}

void TimeManager::Task::Unserialize(Utils::Packet& packet)
{
  char looping;

  next_run.Unserialize(packet);
  length.Unserialize(packet);
  packet >> level >> it >> looping;
  loop = looping != 0;
}