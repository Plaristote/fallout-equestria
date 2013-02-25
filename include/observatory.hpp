#ifndef  OBSERVATORY_HPP
# define OBSERVATORY_HPP

# include <panda3d/cmath.h>
# include <list>
# include <queue>
# include <algorithm>
# include <iostream>

# define S2P_TPL                class P1, class P2
# define S2P_TFUN               P1 (P2)
# define S2P_PARAMS             P2 p1
# define S2P_VALUES             p1
# define S2P_RECORD_CON         p1(p1)
# define S2P_RECORD_ATTR        P2 p1;
# define S2P_RECORD_VAL         params.p1

# define S3P_TPL                class P1, class P2, class P3
# define S3P_TFUN               P1 (P2, P3)
# define S3P_PARAMS             P2 p1, P3 p2
# define S3P_VALUES             p1, p2
# define S3P_RECORD_CON         p1(p1), p2(p2)
# define S3P_RECORD_ATTR        P2 p1; P3 p2;
# define S3P_RECORD_VAL         params.p1, params.p2

# define S4P_TPL                class P1, class P2, class P3, class P4
# define S4P_TFUN               P1 (P2, P3, P4)
# define S4P_PARAMS             P2 p1, P3 p2, P4 p3
# define S4P_VALUES             p1, p2, p3
# define S4P_RECORD_CON         p1(p1), p2(p2), p3(p3)
# define S4P_RECORD_ATTR        P2 p1; P3 p2; P4 p3;
# define S4P_RECORD_VAL         params.p1, params.p2, params.p3

# define DECL_SIGNAL(TPL, TFUN, PARAMS, VALUES, RECORD_CON, RECORD_ATTR, RECORD_VAL) \
  template<TPL>                                                         \
  class Signal<TFUN> : public ISignal                                   \
  {                                                                     \
    friend class ObserverHandler;                                       \
                                                                        \
    struct RecordedCall                                                 \
    {                                                                   \
      RecordedCall(PARAMS) : RECORD_CON {}                              \
                                                                        \
      RECORD_ATTR                                                       \
    };                                                                  \
                                                                        \
    typedef std::queue<RecordedCall> RecordedCalls;                     \
                                                                        \
    struct InterfaceObserver                                            \
    {                                                                   \
      virtual ~InterfaceObserver() {}                                   \
      virtual void operator()(PARAMS) = 0;                              \
                                                                        \
      ObserverId id;                                                    \
    };                                                                  \
                                                                        \
    template<typename ObserverClass>                                    \
    class Observer : public InterfaceObserver                           \
    {                                                                   \
    public:                                                             \
      typedef void (ObserverClass::*Method)(PARAMS);                    \
                                                                        \
      Observer(ObserverClass& observer, Method method) : _observer(observer), _method(method) {} \
                                                                        \
      void operator()(PARAMS) { (_observer.*_method)(VALUES); }         \
                                                                        \
    private:                                                            \
      ObserverClass& _observer;                                         \
      Method         _method;                                           \
    };                                                                  \
                                                                        \
    class FunctionObserver : public InterfaceObserver                   \
    {                                                                   \
      public:                                                           \
        typedef std::function<void (PARAMS)> Function;                  \
                                                                        \
        FunctionObserver(Function function) : _function(function) {}    \
                                                                        \
        void operator()(PARAMS) { _function(VALUES); }                  \
                                                                        \
      private:                                                          \
        Function _function;                                             \
    };                                                                  \
                                                                        \
    typedef std::list<InterfaceObserver*> Observers;                    \
  public:                                                               \
    Signal(bool async = true) : _async(async) {}                        \
                                                                        \
    ~Signal()                                                           \
    {                                                                   \
      typename Observers::iterator it  = _observers.begin();            \
      typename Observers::iterator end = _observers.end();              \
                                                                        \
      for (; it != end ; ++it)                                          \
        delete *it;                                                     \
    }                                                                   \
                                                                        \
    void       SetAsync(bool set)  { _async = set;    }                 \
    bool       IsAsync(void) const { return (_async); }                 \
                                                                        \
    void       Emit(PARAMS)                                             \
    {                                                                   \
      if (_async)                                                       \
      {                                                                 \
        _iterator = _observers.begin();                                 \
        while (_iterator != _observers.end())                           \
        {                                                               \
          typename Observers::iterator trace = _iterator;               \
                                                                        \
          (**_iterator)(VALUES);                                        \
          if (trace == _iterator)                                       \
            _iterator++;                                                \
        }                                                               \
      }                                                                 \
      else                                                              \
        _recordedCalls.push(RecordedCall(VALUES));                      \
    }                                                                   \
                                                                        \
    template<typename ObserverClass>                                    \
    ObserverId Connect(ObserverClass& observerInstance, typename Observer<ObserverClass>::Method method) \
    {                                                                   \
      InterfaceObserver* observer = new Observer<ObserverClass>(observerInstance, method); \
                                                                        \
      return (AddObserver(observer));                                   \
    }                                                                   \
                                                                        \
    ObserverId Connect(typename FunctionObserver::Function function)    \
    {                                                                   \
      InterfaceObserver* observer = new FunctionObserver(function);     \
                                                                        \
      return (AddObserver(observer));                                   \
    }                                                                   \
                                                                        \
    void       Disconnect(ObserverId id)                                \
    {                                                                   \
      typename Observers::iterator toDel = _observers.begin();          \
      typename Observers::iterator end   = _observers.end();            \
                                                                        \
      for (; toDel != end ; ++toDel)                                    \
      {                                                                 \
        if (*toDel == id)                                               \
        {                                                               \
          InterfaceObserver* observer = *toDel;                         \
                                                                        \
          if (toDel == _iterator)                                       \
            _iterator = _observers.erase(toDel);                        \
          else                                                          \
            _observers.erase(toDel);                                    \
          delete observer;                                              \
          break ;                                                       \
        }                                                               \
      }                                                                 \
    }                                                                   \
                                                                        \
    void       DisconnectAll(void)                                      \
    {                                                                   \
      typename Observers::iterator toDel = _observers.begin();          \
      typename Observers::iterator end   = _observers.end();            \
                                                                        \
      while (_observers.begin() != end)                                 \
      {                                                                 \
        delete *(_observers.begin());                                   \
        _observers.erase(_observers.begin());                           \
      }                                                                 \
      _iterator = _observers.end();                                     \
    }                                                                   \
                                                                        \
    inline int ObserverCount(void) const                                \
    {                                                                   \
      return (_observers.size());                                       \
    }                                                                   \
                                                                        \
    void       ExecuteRecordedCalls(void)                               \
    {                                                                   \
      while (_recordedCalls.size())                                     \
      {                                                                 \
        RecordedCall& params = _recordedCalls.front();                  \
                                                                        \
        _iterator = _observers.begin();                                 \
        while (_iterator != _observers.end())                           \
        {                                                               \
          typename Observers::iterator trace = _iterator;               \
                                                                        \
          (**_iterator)(RECORD_VAL);                                    \
          if (trace == _iterator)                                       \
            _iterator++;                                                \
        }                                                               \
        _recordedCalls.pop();                                           \
      }                                                                 \
    }                                                                   \
                                                                        \
  private:                                                              \
    ObserverId AddObserver(InterfaceObserver* observer)                 \
    {                                                                   \
      ObserverId id = observer;                                         \
                                                                        \
      _observers.push_back(observer);                                   \
      return (id);                                                      \
    }                                                                   \
                                                                        \
    typename Observers::iterator _iterator;                             \
    Observers                    _observers;                            \
    RecordedCalls                _recordedCalls;                        \
    bool                         _async;                                \
  };

namespace Observatory
{
  typedef void* ObserverId;
  
  class ObserverHandler;
  
  class ISignal
  {
  public:
    virtual void ExecuteRecordedCalls(void) = 0;
  };
  
  struct Signals : public std::list<ISignal*>
  {
    void ExecuteRecordedCalls(void)
    {
      for_each(begin(), end(), [](ISignal* signal) { signal->ExecuteRecordedCalls(); });
    }
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
        typedef std::function<void ()> Function;

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
    
    void       SetAsync(bool set)  { _async = set;    }
    bool       IsAsync(void) const { return (_async); }

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
  
  DECL_SIGNAL(S2P_TPL, S2P_TFUN, S2P_PARAMS, S2P_VALUES, S2P_RECORD_CON, S2P_RECORD_ATTR, S2P_RECORD_VAL)
  DECL_SIGNAL(S3P_TPL, S3P_TFUN, S3P_PARAMS, S3P_VALUES, S3P_RECORD_CON, S3P_RECORD_ATTR, S3P_RECORD_VAL)
  DECL_SIGNAL(S4P_TPL, S4P_TFUN, S4P_PARAMS, S4P_VALUES, S4P_RECORD_CON, S4P_RECORD_ATTR, S4P_RECORD_VAL)
  
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

    template<class C>
    inline void Connect(C& signal, typename C::FunctionObserver::Function function)
    {
      ObserverId id = signal.Connect(function);

      Add(signal, id);
    }

  private:
    Observers _observers;
  };
}

#endif
