#ifndef QPANDAWIDGET_H
# define QPANDAWIDGET_H

# include <QWidget>
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <iostream>
# include <panda3d/mouseWatcher.h>

class QPandaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPandaWidget(QWidget *parent = 0);
    virtual ~QPandaWidget();

    virtual void     resizeEvent(QResizeEvent*);
    virtual void     showEvent(QShowEvent*);

    QPaintEngine*    paintEngine() const { return (0); }
    void             paintEvent(QPaintEvent*) {}
    virtual void     closeEvent(QCloseEvent*);

    WindowFramework* Window(void) const { return (_window); }

    // PERSONAL ADDITION
    virtual void     mousePressEvent(QMouseEvent* e)   { MousePressed(e); }
    virtual void     mouseReleaseEvent(QMouseEvent* e) { MouseRelease(e); }
signals:
    void MousePressed(QMouseEvent*);
    void MouseRelease(QMouseEvent*);
    // END PERSONNAL ADDITION

signals:
    void             Initialized(void);

private:
    void             UpdateSize(void);
    static void      CallbackMouse(const Event* event, void* ptr);

    WindowFramework* _window;
    QSize            _size;

    PT(MouseWatcher) _mouseWatcher;
};

#endif // QPANDAWIDGET_H
