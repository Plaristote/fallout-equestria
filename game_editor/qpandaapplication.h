#ifndef QPANDAAPPLICATION_H
# define QPANDAAPPLICATION_H

# include <panda3d/cmath.h>
# include <QApplication>
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>

class QPandaApplication : public QApplication
{
    Q_OBJECT
public:
    QPandaApplication(int argc, char **argv);
    ~QPandaApplication();

    int                    exec(void);

    static PandaFramework& Framework(void) { return (_framework); }
    static void            Close(void);

public slots:
    void                   Terminate(void) { Close(); }

private:
    static bool           _continue;
    static PandaFramework _framework;
};

#endif // QPANDAAPPLICATION_H
