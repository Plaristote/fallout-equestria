#ifndef  OBSERVATORY_HPP
# define OBSERVATORY_HPP

# include <panda3d/cmath.h>
# include <list>
# include <algorithm>
# include <iostream>

namespace Observatory
{
  typedef void* ObserverId;

  template<class p1 = void>
  class Signal
  {
    struct InterfaceObserver
    {
      virtual ~InterfaceObserver() {}
      virtual void operator()() = 0;

      ObserverId id;
    };

    class FunctionObserver : public InterfaceObserver
    {
      public:
        typedef void (*Function)();

        FunctionObserver(Function function) : _function(function) {}

        void operator()()
        {
          _function();
        }

      private:
        Function _function;
    };

    template<class ObserverClass>
    class Observer : public InterfaceObserver
    {
    public:
      typedef void (ObserverClass::*Method)();

      Observer(ObserverClass& observer, Method method) : _observer(observer), _method(method) {}

      void operator()() { (_observer.*_method)(); }

    private:
      ObserverClass& _observer;
      Method         _method;
    };

    typedef std::list<InterfaceObserver*> Observers;
  public:
    ~Signal()
    {
      typename Observers::iterator it  = _observers.begin();
      typename Observers::iterator end = _observers.end();

      for (; it != end ; ++it)
        delete *it;
    }

    void       Emit()
    {
      _iterator = _observers.begin();
      while (_iterator != _observers.end())
      {
        typename Observers::iterator trace = _iterator;

        (**_iterator)();
        if (trace == _iterator)
          _iterator++;
      }
    }

    template<class ObserverClass>
    ObserverId Connect(ObserverClass& observerInstance, typename Observer<ObserverClass>::Method method)
    {
      InterfaceObserver* observer = new Observer<ObserverClass>(observerInstance, method);

      return (AddObserver(observer));
    }

    ObserverId Connect(typename FunctionObserver::Function function)
    {
      InterfaceObserver* observer = new FunctionObserver(function);

      return (AddObserver(observer));
    }

    void       Disconnect(ObserverId id)
    {
      typename Observers::iterator toDel = _observers.begin();
      typename Observers::iterator end   = _observers.end();

      for (; toDel != end ; ++toDel)
      {
        if (*toDel == id)
        {
          InterfaceObserver* observer = *toDel;

          if (toDel == _iterator)
            _iterator = _observers.erase(toDel);
          else
            _observers.erase(toDel);
          delete observer;
          break ;
        }
      }
    }

    void       DisconnectAll(void)
    {
      typename Observers::iterator toDel = _observers.begin();
      typename Observers::iterator end   = _observers.end();

      while (_observers.begin() != end)
        _observers.erase(_observers.begin());
    }

    inline int ObserverCount(void) const
    {
      return (_observers.size());
    }

  private:
    ObserverId AddObserver(InterfaceObserver* observer)
    {
      ObserverId id = observer;

      _observers.push_back(observer);
      return (id);
    }

    typename Observers::iterator _iterator;
    Observers                    _observers;
  };

  template<class P1, class P2>
  class Signal<P1 (P2)>
  {
    struct InterfaceObserver
    {
      virtual ~InterfaceObserver() {}
      virtual void operator()(P2 p2) = 0;

      ObserverId id;
    };

    template<typename ObserverClass>
    class Observer : public InterfaceObserver
    {
    public:
      typedef void (ObserverClass::*Method)(P2);

      Observer(ObserverClass& observer, Method method) : _observer(observer), _method(method) {}

      void operator()(P2 p2) { (_observer.*_method)(p2); }

    private:
      ObserverClass& _observer;
      Method         _method;
    };

    typedef std::list<InterfaceObserver*> Observers;
  public:
    ~Signal()
    {
      typename Observers::iterator it  = _observers.begin();
      typename Observers::iterator end = _observers.end();

      for (; it != end ; ++it)
        delete *it;
    }

    void       Emit(P2 p2)
    {
      _iterator = _observers.begin();
      while (_iterator != _observers.end())
      {
        typename Observers::iterator trace = _iterator;

        (**_iterator)(p2);
        if (trace == _iterator)
          _iterator++;
      }
    }

    template<typename ObserverClass>
    ObserverId Connect(ObserverClass& observerInstance, typename Observer<ObserverClass>::Method method)
    {
      InterfaceObserver* observer = new Observer<ObserverClass>(observerInstance, method);

      return (AddObserver(observer));
    }

    void       Disconnect(ObserverId id)
    {
      typename Observers::iterator toDel = _observers.begin();
      typename Observers::iterator end   = _observers.end();

      for (; toDel != end ; ++toDel)
      {
        if (*toDel == id)
        {
          InterfaceObserver* observer = *toDel;

          if (toDel == _iterator)
            _iterator = _observers.erase(toDel);
          else
            _observers.erase(toDel);
          delete observer;
          break ;
        }
      }
    }

    void       DisconnectAll(void)
    {
      typename Observers::iterator toDel = _observers.begin();
      typename Observers::iterator end   = _observers.end();

      while (_observers.begin() != end)
        _observers.erase(_observers.begin());
    }    

    inline int ObserverCount(void) const
    {
      return (_observers.size());
    }

  private:
    ObserverId AddObserver(InterfaceObserver* observer)
    {
      ObserverId id = observer;

      _observers.push_back(observer);
      return (id);
    }

    typename Observers::iterator _iterator;
    Observers                    _observers;
  };

  template<class P1, class P2, class P3>
  class Signal<P1 (P2, P3)>
  {
    struct InterfaceObserver
    {
      virtual ~InterfaceObserver() {}
      virtual void operator()(P2 p2, P3 p3) = 0;

      ObserverId id;
    };

    template<typename ObserverClass>
    class Observer : public InterfaceObserver
    {
    public:
      typedef void (ObserverClass::*Method)(P2, P3);

      Observer(ObserverClass& observer, Method method) : _observer(observer), _method(method) {}

      void operator()(P2 p2, P3 p3) { (_observer.*_method)(p2, p3); }

    private:
      ObserverClass& _observer;
      Method         _method;
    };

    typedef std::list<InterfaceObserver*> Observers;
  public:
    ~Signal()
    {
      typename Observers::iterator it  = _observers.begin();
      typename Observers::iterator end = _observers.end();

      for (; it != end ; ++it)
        delete *it;
    }

    void       Emit(P2 p2, P3 p3)
    {
      _iterator = _observers.begin();
      while (_iterator != _observers.end())
      {
        typename Observers::iterator trace = _iterator;

        (**_iterator)(p2, p3);
        if (trace == _iterator)
          _iterator++;
      }
    }

    template<typename ObserverClass>
    ObserverId Connect(ObserverClass& observerInstance, typename Observer<ObserverClass>::Method method)
    {
      InterfaceObserver* observer = new Observer<ObserverClass>(observerInstance, method);

      return (AddObserver(observer));
    }

    void       Disconnect(ObserverId id)
    {
      typename Observers::iterator toDel = _observers.begin();
      typename Observers::iterator end   = _observers.end();

      for (; toDel != end ; ++toDel)
      {
        if (*toDel == id)
        {
          InterfaceObserver* observer = *toDel;

          if (toDel == _iterator)
            _iterator = _observers.erase(toDel);
          else
            _observers.erase(toDel);
          delete observer;
          break ;
        }
      }
    }

    void       DisconnectAll(void)
    {
      typename Observers::iterator toDel = _observers.begin();
      typename Observers::iterator end   = _observers.end();

      while (_observers.begin() != end)
        _observers.erase(_observers.begin());
    }    

    inline int ObserverCount(void) const
    {
      return (_observers.size());
    }

  private:
    ObserverId AddObserver(InterfaceObserver* observer)
    {
      ObserverId id = observer;

      _observers.push_back(observer);
      return (id);
    }

    typename Observers::iterator _iterator;
    Observers                    _observers;
  };  
}

#endif
