#include "time_manager.hpp"

using namespace std;

TimeManager* TimeManager::CurrentTimeManager = 0;

void TimeManager::AddElapsedSeconds(float s)
{
  fseconds += s;
  if (fseconds >= 1.f)
  {
#ifdef __llvm__
    float floored = ::floor(fseconds);
#else
    float floored = std::floor(fseconds);
#endif

    AddElapsedTime(floored);
    fseconds -= floored;
  }
}

void TimeManager::AddElapsedTime(DateTime::TimeUnit time)
{
  current_time = current_time + time;
}

TimeManager::Task* TimeManager::AddRepetitiveTask(unsigned char level, DateTime::TimeUnit interval)
{
  TimeManager::Task* task = AddTask(level, interval);

  task->loop = true;
  return (task);
}

TimeManager::Task* TimeManager::AddTask(unsigned char level, DateTime::TimeUnit interval)
{
  Task* task = new Task;

  task->level    = level;
  task->loop     = false;
  task->it       = 0;
  task->length   = DateTime(interval);
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
  Tasks::iterator it  = tasks.begin();
  Tasks::iterator end = tasks.end();

  while (it != end)
  {
    Task&          task = **it;

    //cout << "Checking task: " << task.next_run.ToString() << ", current_time: " << current_time.ToString() << endl;
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
}

/*
 * TimeManager::Task
 */
void TimeManager::Task::NextStep(void)
{
  if (length.GetTimestamp() == 0)
    length = length + 1;
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
