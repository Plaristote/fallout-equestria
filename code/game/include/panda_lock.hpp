#ifndef  PANDA_LOCK_HPP
# define PANDA_LOCK_HPP

# include "semaphore.hpp"

class PandaLock
{
public:
  static void Require(void)
  {
    if (instance == 0)
      instance = new PandaLock;
    instance->semaphore.Wait();
  }

  static void Release(void)
  {
    if (instance != 0)
      instance->semaphore.Post();
  }
  
  static void Finalize(void)
  {
    delete instance;
    instance = 0;
  }

private:
  PandaLock(void) {}
  ~PandaLock(void) {}

  static PandaLock* instance;
  Sync::Semaphore   semaphore;
};

#endif
