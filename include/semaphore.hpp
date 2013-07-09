#ifndef  SEMAPHORE_HPP
# define SEMAPHORE_HPP

# include "globals.hpp"

# ifdef __linux__
#  include <semaphore.h>
#  include <thread>
# endif

# ifdef _WIN32
#  include <Winsock2.h>
#  include <windows.h>
#  include <stdio.h>
# endif

namespace Sync
{
  class Semaphore
  {
  # ifdef __linux__
    typedef sem_t           Handle;
    typedef std::thread::id ThreadId;
  # endif
  # ifdef _WIN32
    typedef HANDLE Handle;
    typedef DWORD  ThreadId; 
  # endif
  public:
    class Lock
    {
    public:
      Lock(Semaphore& semaphore) : _semaphore(semaphore)
      {
        _semaphore.Wait();
      }

      ~Lock(void)
      {
        _semaphore.Post();
      }

    private:
      Semaphore& _semaphore;
    };

    Semaphore(unsigned int initial_count = 1, unsigned int maximum_count = 1);
    ~Semaphore(void);

    bool TryWait(void);
    bool Wait(void);
    bool Post(void);

    void SetDeadlockSafety(bool to_set) { _deadlock_safety = to_set; }

  private:
    inline bool     SelfLock(void) const;
    static ThreadId GetCurrentThreadId(void);

    Handle          _handle;
    unsigned int    _max_count;
    bool            _deadlock_safety;
    bool            _thread_lock;
    ThreadId        _thread_id;
  };
}

#endif
