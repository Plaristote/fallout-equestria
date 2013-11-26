#include "qpandaapplication.h"

PandaFramework* QPandaApplication::_framework;
bool            QPandaApplication::_continue;
bool            QPandaApplication::_panda_enabled;
QTimer          QPandaApplication::_timer;

QPandaApplication::QPandaApplication(int argc, char **argv) : QApplication(argc, argv)
{
  framework.open_framework(argc, argv);
  _framework     = &framework;
  _continue      = true;
  _panda_enabled = false;
  _timer.setInterval(25);
  connect(&_timer, SIGNAL(timeout()), this, SLOT(PandaRefresh()));
}

QPandaApplication::~QPandaApplication()
{
  framework.close_framework();
}

void           QPandaApplication::PandaRefresh(void)
{
  if (_continue && _panda_enabled)
  {
    Thread* current_thread = Thread::get_current_thread();

    framework.do_frame(current_thread);
    _timer.start();
  }
}

int            QPandaApplication::exec(void)
{
    //_timer.start();
    return (QApplication::exec());
}

void           QPandaApplication::Close(void)
{
    _continue = false;
}

void           QPandaApplication::SetPandaEnabled(bool enabled)
{
  if (_panda_enabled == false && enabled)
    _timer.start();
  else if (_panda_enabled == true && !enabled)
    _timer.stop();
  _panda_enabled = enabled;
}
