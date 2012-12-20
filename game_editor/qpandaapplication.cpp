#include "qpandaapplication.h"

PandaFramework QPandaApplication::_framework;
bool           QPandaApplication::_continue;

QPandaApplication::QPandaApplication(int argc, char **argv) : QApplication(argc, argv)
{
  _framework.open_framework(argc, argv);
  _continue      = true;
  _panda_enabled = true;
  _timer.setInterval(50);
  connect(&_timer, SIGNAL(timeout()), this, SLOT(PandaRefresh()));
}

QPandaApplication::~QPandaApplication()
{
  _framework.close_framework();
}

void           QPandaApplication::PandaRefresh(void)
{
    if (_continue)
    {
      Thread* current_thread = Thread::get_current_thread();

      _framework.do_frame(current_thread);
      _timer.start();
    }
}

int            QPandaApplication::exec(void)
{
    _timer.start();
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
  _panda_enabled = enabled;
}
