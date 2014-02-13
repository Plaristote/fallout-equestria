#include "thread.hpp"

using namespace Sync;
using namespace std;

#ifdef _WIN32
void MyThread::Launch(void)
{
  _handle = CreateThread(NULL, 0, &MyThread::CallbackLaunch, this, 0, NULL);
  if (_handle == NULL)
    LaunchFailed();
}

void MyThread::Join(void)
{
  WaitForSingleObject(_handle, INFINITE);
}
#else
void MyThread::Launch(void)
{
  if ((pthread_create(&_handle, NULL, &MyThread::CallbackLaunch, this)) < 0)
    LaunchFailed();
}

void MyThread::Join(void)
{
  pthread_join(_handle, 0);
}
#endif

void MyThread::LaunchFailed(void)
{
}
