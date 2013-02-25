#ifndef  SEMAPHORE_HPP
# define SEMAPHORE_HPP

# ifdef __linux__
#  include <semaphore.h>
# endif

# ifdef _WIN32
#  include "directory.hpp"
//#  include <Windows.h>
//#  include <stdio.h>
# endif

class Semaphore
{
# ifdef __linux__
  typedef sem_t  Handle;
# endif
# ifdef _WIN32
  typedef HANDLE Handle;
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

private:
  Handle       _handle;
  unsigned int _max_count;
};


#endif
