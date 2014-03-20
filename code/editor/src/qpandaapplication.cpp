#include "qpandaapplication.h"

PandaFramework* QPandaApplication::_framework = 0;
bool            QPandaApplication::_continue;
bool            QPandaApplication::_panda_enabled;
QTimer          QPandaApplication::_timer;

QPandaApplication::QPandaApplication(QApplication& application, int argc, char **argv) : application(application)
{
  _framework     = new PandaFramework;
  _framework->open_framework(argc, argv);
  _continue      = true;
  _panda_enabled = false;
  _timer.setInterval(25);
  connect(&_timer, SIGNAL(timeout()), this, SLOT(PandaRefresh()));
}

QPandaApplication::~QPandaApplication()
{
  _framework->close_framework();
  delete _framework;
}

void           QPandaApplication::PandaRefresh(void)
{
  if (_continue && _panda_enabled)
  {
    Thread* current_thread = Thread::get_current_thread();

    _framework->do_frame(current_thread);
    _timer.start();
  }
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
