#include "semaphore.hpp"
#include <iostream>

using namespace Sync;

#ifdef __linux__
Semaphore::Semaphore(unsigned int initial_count, unsigned int max_count)
{
  sem_init(&_handle, 0, initial_count);
  _max_count           = max_count;
  _deadlock_safety     = false;
  _current_thread_lock = 0;
}

Semaphore::Semaphore(const Semaphore& copy)
{
  _max_count           = copy._max_count;
  _deadlock_safety     = false;
  _current_thread_lock = 0;
  sem_init(&_handle, 0, _max_count);
  SetDeadlockSafety(copy._deadlock_safety);
}

Semaphore::~Semaphore(void)
{
  if (_current_thread_lock)
    delete _current_thread_lock;
  sem_destroy(&_handle);
}

bool Semaphore::TryWait(void)
{
  return (sem_trywait(&_handle) == 0);
}

bool Semaphore::Wait(void)
{
  bool result;

  if (_deadlock_safety && IsLockedByCurrentThread())
    return (true);
  result = sem_wait(&_handle) == 0;
  if (_deadlock_safety)
    SetCurrentThread();
  return (result);
}

bool Semaphore::Post(void)
{
  int             value;

  if (!(sem_getvalue(&_handle, &value)))
  {
    if (value < (int)_max_count && sem_post(&_handle) == 0)
    {
      if (_deadlock_safety)
        ClearCurrentThread();
      return (true);
    }
  }
  return (false);
}
#endif

#ifdef _WIN32
Semaphore::Semaphore(unsigned int initial_count, unsigned int maximum_count)
{
  _handle              = CreateSemaphore(NULL, initial_count, maximum_count, NULL);
  _max_count           = maximum_count;
  _deadlock_safety     = false;
  _current_thread_lock = 0;
}

Semaphore::Semaphore(const Semaphore& copy)
{
  int initial_count = copy._max_count; // WARNING Perhaps change this behaviour
  
  _handle              = CreateSemaphore(NULL, initial_count, copy._max_count, NULL);
  _max_count           = copy._max_count;
  _deadlock_safety     = false;
  _current_thread_lock = 0;
  SetDeadlockSafety(copy._deadlock_safety);
}

Semaphore::~Semaphore(void)
{
  CloseHandle(_handle);
  if (_current_thread_lock != 0)
    delete _current_thread_lock;
}

bool Semaphore::TryWait(void)
{
  DWORD result;

  if (_deadlock_safety)
  {
    bool reentering;

    if (!(_current_thread_lock->TryWait()))
      return (false);
    reentering = GetCurrentThreadId() == _current_thread;
    _current_thread_lock->Post();
    if (reentering)
      return (true);
  }

  result       = WaitForSingleObject(_handle, 0);

  if (result != WAIT_FAILED && result != WAIT_TIMEOUT)
  {
    if (_deadlock_safety)
    {
      if (TrySetCurrentThread() == false)
      {
        ReleaseSemaphore(_handle, 1, NULL);
        return (false);
      }
    }
    return (true);
  }
  return (false);
}

bool Semaphore::Wait(void)
{
  bool success;

  if (_deadlock_safety && IsLockedByCurrentThread())
    return (true);
  success = WaitForSingleObject(_handle, INFINITE) != WAIT_FAILED;
  if (_deadlock_safety)
    SetCurrentThread();
  return (success);
}

bool Semaphore::Post(void)
{
  if (_deadlock_safety)
    ClearCurrentThread();
  return (ReleaseSemaphore(_handle, 1, NULL) != 0);
}
#endif

/*
 * Re-entrency
 */
bool Semaphore::IsLockedByCurrentThread()
{
  bool result = false;

  if (_current_thread_lock)
  {
    _current_thread_lock->Wait();
    result = CurrentThreadId() == _current_thread;
    _current_thread_lock->Post();
  }
  return (result);
}

void Semaphore::SetCurrentThread()
{
  if (_current_thread_lock)
  {
    _current_thread_lock->Wait();
    _current_thread = CurrentThreadId();
    _current_thread_lock->Post();
  }
}

bool Semaphore::TrySetCurrentThread()
{
  if (_current_thread_lock)
  {
    if (_current_thread_lock->TryWait() == false)
      return (false);
    _current_thread = CurrentThreadId();
    _current_thread_lock->Post();
    return (true);
  }
  return (false);
}

void Semaphore::ClearCurrentThread()
{
  if (_current_thread_lock)
  {
    _current_thread_lock->Wait();
#ifdef _WIN32
    _current_thread = NULL;
#else
    std::thread::id nobody;

    _current_thread = nobody;
#endif
    _current_thread_lock->Post();
  }
}

Semaphore::ThreadId Semaphore::CurrentThreadId()
{
#ifdef _WIN32
  return (GetCurrentThreadId());
#else
  return (std::this_thread::get_id());
#endif
}

void Semaphore::SetDeadlockSafety(bool value)
{
  if (value != _deadlock_safety)
  {
    _deadlock_safety = value;
    if (value == true && _current_thread_lock == 0)
    {
      _current_thread_lock = new Semaphore;
    }
    else if (_current_thread_lock != 0)
    {
      delete _current_thread_lock;
      _current_thread_lock = 0;
    }
  }
}
