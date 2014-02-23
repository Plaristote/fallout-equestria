#ifndef  TIME_MANAGER_HPP
# define TIME_MANAGER_HPP

# include "globals.hpp"
# include "observatory.hpp"
# include "datetime.hpp"

# define TASK_LVL_WORLDMAP 1
# define TASK_LVL_CITY     2

class TimeManager
{
public:
  struct Task
  {
    ~Task() {}

    bool           loop;
    unsigned short it;
    DateTime       next_run, length;
    unsigned char  level;

    void NextStep(void);

    Sync::Signal<void>                  Interval;
    Sync::Signal<void (unsigned short)> IntervalIt;
    
    void Serialize(Utils::Packet&);
    void Unserialize(Utils::Packet&);

  private:
    friend class TimeManager;
    Task()  {}
  };

  typedef std::list<Task*> Tasks;

  TimeManager(void)
  {
    fseconds = 0.f;
  }

  ~TimeManager(void)
  {
    ClearTasks(0);
  }
  
  void            ClearTasks(unsigned char level)
  {
    Tasks::iterator it, end;
    
    for (it = tasks.begin(), end = tasks.end() ; it != end ;)
    {
      Task* task = *it;
      
      if (task->level >= level)
      {
        delete task;
        it = tasks.erase(it);
      }
      else
        ++it;
    }
  }
  
  void            SetTime(unsigned short s, unsigned short m, unsigned short h, unsigned short d, unsigned short mo, unsigned short y)
  {
    current_time = DateTime(y, mo, d, h, m, s);
  }

  DateTime        GetDateTime(void) const { return (current_time); }

  void            AddElapsedSeconds(float s);
  void            AddElapsedTime(DateTime::TimeUnit interval);

  Task*           AddRepetitiveTask(unsigned char level, DateTime::TimeUnit interval);
  Task*           AddTask(unsigned char level, DateTime::TimeUnit interval);
  void            DelTask(Task* task);  
  void            ExecuteTasks(void);
private:
  void            CorrectValues(void);

  float           fseconds;
  DateTime        current_time;
  Tasks           tasks;
  Tasks::iterator safeIt;
};

#endif
