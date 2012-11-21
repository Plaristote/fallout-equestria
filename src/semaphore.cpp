#include "semaphore.hpp"

#ifdef __linux__
Semaphore::Semaphore(unsigned int initial_count, unsigned int max_count)
{
  sem_init(&_handle, 0, initial_count);
  _max_count = max_count;
}

Semaphore::~Semaphore(void)
{
  sem_destroy(&_handle);
}

bool Semaphore::TryWait(void)
{
  return (sem_trywait(&_handle) == 0);
}

bool Semaphore::Wait(void)
{
  return (sem_wait(&_handle) == 0);
}

bool Semaphore::Post(void)
{
  int value;

  if (!(sem_getvalue(&_handle, &value)))
  {
    if (value < _max_count)
      return (sem_post(&_handle) == 0);
  }
  return (false);
}
#endif

#ifdef _WIN32
Semaphore::Semaphore(unsigned int initial_count, unsigned int maximum_count)
{
  _handle = CreateSemaphore(NULL, initial_count, maximum_count, NULL);
}

Semaphore::~Semaphore(void)
{
  CloseHandle(_handle);
}

bool Semaphore::TryWait(void)
{
  return (WaitForSingleObject(_handle, 0));
}

bool Semaphore::Wait(void)
{
  return (WaitForSingleObject(_handle, INFINITE));
}

bool Semaphore::Post(void)
{
  return (ReleaseSemaphore(_handle, 1, NULL));
}
#endif

