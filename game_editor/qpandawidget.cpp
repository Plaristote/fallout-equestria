#include "qpandawidget.h"
#include <QResizeEvent>
#ifdef Q_WS_X11
# include <Qt/qx11info_x11.h>
# include <X11/Xlib.h>
#endif
#include <iostream>
#include "qpandaapplication.h"
#include <QMessageBox>

QPandaWidget::QPandaWidget(QWidget *parent) : QWidget(parent)
{
    EventHandler& events = QPandaApplication::Framework().get_event_handler();

    _window        = 0;
    _loadingFailed = false;
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setFocusPolicy(Qt::StrongFocus);
    events.add_hook("mouse1",    QPandaWidget::CallbackMouse, this);
    events.add_hook("mouse2",    QPandaWidget::CallbackMouse, this);
    events.add_hook("mouse3",    QPandaWidget::CallbackMouse, this);
    events.add_hook("mouse1-up", QPandaWidget::CallbackMouse, this);
    events.add_hook("mouse2-up", QPandaWidget::CallbackMouse, this);
    events.add_hook("mouse3-up", QPandaWidget::CallbackMouse, this);
}

QPandaWidget::~QPandaWidget()
{
    EventHandler& events = QPandaApplication::Framework().get_event_handler();

    events.remove_hooks_with(this);
    if (_window)
      QPandaApplication::Framework().close_window(_window);
}

void QPandaWidget::UpdateSize(void)
{
    if (_window)
    {
        WindowProperties wp;

        _window->get_graphics_window()->request_properties(wp);
        wp.set_size(_size.width(), _size.height());
        wp.set_origin(0, 0);
        _window->get_graphics_window()->set_properties_now(wp);
    }
}

void QPandaWidget::resizeEvent(QResizeEvent* event)
{
    _size = event->size();
    UpdateSize();
    QWidget::resizeEvent(event);
}

void QPandaWidget::showEvent(QShowEvent* event)
{
    if (_window == 0)
    {
        WindowProperties wp;

#ifdef Q_WS_X11
        XFlush(QX11Info::display());
#endif
        QPandaApplication::Framework().get_default_window_props(wp);
        wp.set_parent_window((size_t)this->winId());
        _window = QPandaApplication::Framework().open_window(wp, 0);
        if (_window != 0)
        {
          UpdateSize();
          Initialized();
        }
        else
        {
          _loadingFailed = true;
          QMessageBox::warning(this, "Panda3D Fatal Error", "PandaFramework couldn't open a window");
        }
    }
    QWidget::showEvent(event);
}

void QPandaWidget::closeEvent(QCloseEvent*)
{
    QPandaApplication::Close();
}

// MOUSE MANAGEMENT
void QPandaWidget::CallbackMouse(const Event* event, void* ptr)
{
    QPandaWidget*     instance = reinterpret_cast<QPandaWidget*>(ptr);
    MouseData         data     = instance->_window->get_graphics_window()->get_pointer(0);

    if (data.get_in_window())
    {
        QPoint            pos(data.get_x(), data.get_y());
        std::string       name     = event->get_name();
        Qt::MouseButton   button;
        QMouseEvent::Type type;
        char              buttonN;

        if (name.substr(name.size() - 2) == "up")
        {
            type = QMouseEvent::MouseButtonRelease;
            name = name.substr(0, name.size() - 3);
        }
        else
            type = QMouseEvent::MouseButtonPress;

        buttonN  = name[name.size() - 1];
        name = name.substr(0, name.size() - 1);

        switch (buttonN)
        {
        default:
        case '1':
            button = Qt::LeftButton;
            break ;
        case '2':
            button = Qt::RightButton;
            break ;
        case '3':
            button = Qt::MiddleButton;
            break ;
        }

        if (type == QMouseEvent::MouseButtonPress)
        {
          QMouseEvent event(type, pos, button, 0, Qt::NoModifier);

          instance->mousePressEvent(&event);
        }
        else
        {
          QMouseEvent event(type, pos, button, 0, Qt::NoModifier);

          instance->mouseReleaseEvent(&event);
        }
    }
}
