#ifndef QPANDAAPPLICATION_H
# define QPANDAAPPLICATION_H

# include <panda3d/cmath.h>
# include <QApplication>
# include <QTimer>
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>

class QPandaApplication : public QApplication
{
    Q_OBJECT
public:
    QPandaApplication(int argc, char **argv);
    ~QPandaApplication();

    int                    exec(void);

    static PandaFramework& Framework(void) { return (*_framework); }
    static void            Close(void);

    static void            SetPandaEnabled(bool enabled);

public slots:
    void                   Terminate(void) { Close(); }
    void                   PandaRefresh(void);
    void                   EnablePanda(void)  { SetPandaEnabled(true);  }
    void                   DisablePanda(void) { SetPandaEnabled(false); }

private:
    PandaFramework         framework;
    static bool            _continue;
    static PandaFramework* _framework;
    static QTimer          _timer;
    static bool            _panda_enabled;
};

#endif // QPANDAAPPLICATION_H
