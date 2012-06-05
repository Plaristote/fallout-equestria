#include "qpandaapplication.h"

PandaFramework QPandaApplication::_framework;
bool           QPandaApplication::_continue;

QPandaApplication::QPandaApplication(int argc, char **argv) : QApplication(argc, argv)
{
  _framework.open_framework(argc, argv);
  _continue = true;
}

QPandaApplication::~QPandaApplication()
{
  _framework.close_framework();
}

int            QPandaApplication::exec(void)
{
    Thread*    currentThread = Thread::get_current_thread();

    do
    {
        processEvents();
    } while (_continue && _framework.do_frame(currentThread));
    return (0);
}

void           QPandaApplication::Close(void)
{
    _continue = false;
}
