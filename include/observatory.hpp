#ifndef  OBSERVATORY_HPP
# define OBSERVATORY_HPP

# include <panda3d/cmath.h>
# include <list>
# include <queue>
# include <algorithm>
# include <iostream>

namespace Observatory
{
  typedef void* ObserverId;
  
  class ObserverHandler;
  
  class ISignal
  {
  public:
    virtual void ExecuteRecordedCalls(void) = 0;
  };
  
  template<class p1 = void>
  class Signal : public ISignal
  {
    friend class ObserverHandler;
    
    struct RecordedCall
    {
      bool byte;
    };

    typedef std::queue<RecordedCall> RecordedCalls;

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
    Signal(bool async = true) : _async(async) {}    
    
    ~Signal()
    {
      typename Observers::iterator it  = _observers.begin();
      typename Observers::iterator end = _observers.end();

      for (; it != end ; ++it)
        delete *it;
    }

    void       Emit()
    {
      if (_async)
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
      else
	_recordedCalls.push(RecordedCall());
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
      {
	delete *(_observers.begin());
        _observers.erase(_observers.begin());
      }
      _iterator = _observers.end();
    }

    inline int ObserverCount(void) const
    {
      return (_observers.size());
    }
    
    void       ExecuteRecordedCalls(void)
    {
      while (_recordedCalls.size())
      {
	RecordedCall& params = _recordedCalls.front();

	_iterator = _observers.begin();
	while (_iterator != _observers.end())
	{
	  typename Observers::iterator trace = _iterator;

	  (**_iterator)();
	  if (trace == _iterator)
	    _iterator++;
	}
	_recordedCalls.pop();
      }
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
    RecordedCalls                _recordedCalls;
    bool                         _async;
  };

  template<class P1, class P2>
  class Signal<P1 (P2)> : public ISignal
  {
    friend class ObserverHandler;

    struct RecordedCall
    {
      RecordedCall(P2 p1) : p1(p1) {}

      P2 p1;
    };

    typedef std::queue<RecordedCall> RecordedCalls;    
    
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
    Signal(bool async = true) : _async(async) {}
    
    ~Signal()
    {
      typename Observers::iterator it  = _observers.begin();
      typename Observers::iterator end = _observers.end();

      for (; it != end ; ++it)
        delete *it;
    }

    void       Emit(P2 p2)
    {
      if (_async)
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
      else
	_recordedCalls.push(RecordedCall(p2));
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
      {
	delete *(_observers.begin());
        _observers.erase(_observers.begin());
      }
      _iterator = _observers.end();
    }    

    inline int ObserverCount(void) const
    {
      return (_observers.size());
    }
    
    void       ExecuteRecordedCalls(void)
    {
      while (_recordedCalls.size())
      {
	RecordedCall& params = _recordedCalls.front();

	_iterator = _observers.begin();
	while (_iterator != _observers.end())
	{
	  typename Observers::iterator trace = _iterator;

	  (**_iterator)(params.p1);
	  if (trace == _iterator)
	    _iterator++;
	}
	_recordedCalls.pop();
      }
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
    RecordedCalls                _recordedCalls;
    bool                         _async;
  };

  template<class P1, class P2, class P3>
  class Signal<P1 (P2, P3)> : public ISignal
  {
    friend class ObserverHandler;

    struct RecordedCall
    {
      RecordedCall(P2 p1, P3 p2) : p1(p1), p2(p2) {}

      P2 p1;
      P3 p2;
    };

    typedef std::queue<RecordedCall> RecordedCalls;    
    
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
    Signal(bool async = true) : _async(async) {}

    ~Signal()
    {
      typename Observers::iterator it  = _observers.begin();
      typename Observers::iterator end = _observers.end();

      for (; it != end ; ++it)
        delete *it;
    }

    void       Emit(P2 p2, P3 p3)
    {
      if (_async)
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
      else
	_recordedCalls.push(RecordedCall(p2, p3));
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
      {
	delete *(_observers.begin());
        _observers.erase(_observers.begin());
      }
      _iterator = _observers.end();
    }    

    inline int ObserverCount(void) const
    {
      return (_observers.size());
    }
    
    void       ExecuteRecordedCalls(void)
    {
      while (_recordedCalls.size())
      {
	RecordedCall& params = _recordedCalls.front();

	_iterator = _observers.begin();
	while (_iterator != _observers.end())
	{
	  typename Observers::iterator trace = _iterator;

	  (**_iterator)(params.p1, params.p2);
	  if (trace == _iterator)
	    _iterator++;
	}
	_recordedCalls.pop();
      }
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
    RecordedCalls                _recordedCalls;
    bool                         _async;
  };
  
  template<class P1, class P2, class P3, class P4>
  class Signal<P1 (P2, P3, P4)> : public ISignal
  {
    friend class ObserverHandler;

    struct RecordedCall
    {
      RecordedCall(P2 p1, P3 p2, P4 p3) : p1(p1), p2(p2), p3(p3) {}

      P2 p1;
      P3 p2;
      P4 p3;
    };
    
    typedef std::queue<RecordedCall> RecordedCalls;
    
    struct InterfaceObserver
    {
      virtual ~InterfaceObserver() {}
      virtual void operator()(P2 p2, P3 p3, P4 p4) = 0;

      ObserverId id;
    };

    template<typename ObserverClass>
    class Observer : public InterfaceObserver
    {
    public:
      typedef void (ObserverClass::*Method)(P2, P3, P4);

      Observer(ObserverClass& observer, Method method) : _observer(observer), _method(method) {}

      void operator()(P2 p2, P3 p3, P4 p4) { (_observer.*_method)(p2, p3, p4); }

    private:
      ObserverClass& _observer;
      Method         _method;
    };

    typedef std::list<InterfaceObserver*> Observers;
  public:
    Signal(bool async = true) : _async(async) {}
    
    ~Signal()
    {
      typename Observers::iterator it  = _observers.begin();
      typename Observers::iterator end = _observers.end();

      for (; it != end ; ++it)
        delete *it;
    }

    void       Emit(P2 p2, P3 p3, P4 p4)
    {
      if (_async)
      {
	_iterator = _observers.begin();
	while (_iterator != _observers.end())
	{
	  typename Observers::iterator trace = _iterator;

	  (**_iterator)(p2, p3, p4);
	  if (trace == _iterator)
	    _iterator++;
	}
      }
      else
	_recordedCalls.push(RecordedCall(p2, p3, p4));
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
      {
	delete *(_observers.begin());
	_observers.erase(_observers.begin());
      }
      _iterator = _observers.end();
    }    

    inline int ObserverCount(void) const
    {
      return (_observers.size());
    }
    
    void       ExecuteRecordedCalls(void)
    {
      while (_recordedCalls.size())
      {
	RecordedCall& params = _recordedCalls.front();

	_iterator = _observers.begin();
	while (_iterator != _observers.end())
	{
	  typename Observers::iterator trace = _iterator;

	  (**_iterator)(params.p1, params.p2, params.p3);
	  if (trace == _iterator)
	    _iterator++;
	}
	_recordedCalls.pop();
      }
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
    RecordedCalls                _recordedCalls;
    bool                         _async;
  };
  
  class ObserverHandler
  {
    struct IObserverPair
    {
      virtual ~IObserverPair() {};
      virtual void Disconnect(void) = 0;
    };

    template<class C>
    struct ObserverPair : public IObserverPair
    {
      ObserverPair(C& signal, ObserverId id) : signal(signal), id(id) {}
      void Disconnect(void) { signal.Disconnect(id); }
      C&         signal;
      ObserverId id;
    };
    
    typedef std::list<IObserverPair*> Observers;
  public:
    ~ObserverHandler()
    {
      DisconnectAll();
    }
    
    void       DisconnectAll(void)
    {
      std::for_each(_observers.begin(), _observers.end(), [](IObserverPair* observer)
      {
	observer->Disconnect();
	delete observer;
      });
      _observers.clear();
    }
    
    template<class C>
    inline void Add(C& signal, ObserverId id)
    {
      _observers.push_back(new ObserverPair<C>(signal, id));
    }
    
    template<class C, typename ObserverClass>
    inline void Connect(C& signal, ObserverClass& observerInstance, typename C::template Observer<ObserverClass>::Method method)
    {
      ObserverId id = signal.Connect(observerInstance, method);
      
      Add(signal, id);
    }

  private:
    Observers _observers;
  };
}

#endif
