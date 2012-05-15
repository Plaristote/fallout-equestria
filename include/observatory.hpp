#ifndef  OBSERVATORY_HPP
# define OBSERVATORY_HPP

# include <list>
# include <algorithm>
# include <iostream>

# define CONNECT_HANDLE(handler, signal, object, method) ((handler)(signal, (signal).Connect((object), (method))))

namespace Observatory
{
  typedef void* ObserverId;

  /*! \class Signal
   * \brief Sort of template for the Observer DP. Awesome stuff inside.
   */
  template<typename ...Args>
  class Signal
  {
    struct InterfaceObserver
    {
      virtual ~InterfaceObserver() {}
      virtual void operator()(Args... args) = 0;

      ObserverId id;
    };
    
    class FunctionObserver : public InterfaceObserver
    {
      public:
        typedef void (*Function)(Args... args);
	
	FunctionObserver(Function function) : _function(function) {}
      
        void operator()(Args... args)
        {
	  _function(args...);
	}
      
      private:
        Function _function;
    };

    template<typename ObserverClass>
    class Observer : public InterfaceObserver
    {
    public:
      typedef void (ObserverClass::*Method)(Args... args);

      Observer(ObserverClass& observer, Method method) : _observer(observer), _method(method) {}

      void operator()(Args... args) { (_observer.*_method)(args...); }

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

    void       Emit(Args... args)
    {
      _iterator = _observers.begin();
      while (_iterator != _observers.end())
      {
        typename Observers::iterator trace = _iterator;

	(**_iterator)(args...);
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

  /*! \class ObserverHandlers
   * \brief Manager for easy disconnection of Observers using Signal
   */
  class ObserverHandlers
  {
    struct IHandler
    {
      virtual ~IHandler() {}
      virtual void Disconnect(void) = 0;

      bool operator==(ObserverId right) const { return (id == right); }

      ObserverId id;
    };

    template<class Signal>
    struct Handler : public IHandler
    {
      Handler(Signal& signal, ObserverId id) : signal(signal) { this->id = id; }

      void Disconnect(void) { signal.Disconnect(id); }

      Signal&    signal;
    };

    typedef std::list<IHandler*> Handlers;

  public:
    ~ObserverHandlers() { DisconnectAll(); }

    template<typename Signal>
    ObserverId operator()(Signal& signal, ObserverId id)
    {
      IHandler* handler = new Handler<Signal>(signal, id);

      _handlers.push_back(handler);
      return (id);
    }

    void DisconnectAll()
    {
      Handlers::iterator it;
      Handlers::iterator end = _handlers.end();

      while ((it = _handlers.begin()) != end)
        DisconnectAndRemove(it);
    }

    void Disconnect(ObserverId id)
    {
      Handlers::iterator it  = _handlers.begin();
      Handlers::iterator end = _handlers.end();

      for (; it != end ; ++it)
      {
        if ((*it)->id == id)
        {
          DisconnectAndRemove(it);
          break ;
        }
      }
    }

  private:
    void DisconnectAndRemove(Handlers::iterator it)
    {
      (*it)->Disconnect();
      delete (*it);
      _handlers.erase(it);
    }

    Handlers _handlers;
  };  
}

#endif
