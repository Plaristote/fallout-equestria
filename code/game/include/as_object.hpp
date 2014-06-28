#ifndef  AS_OBJECT_HPP
# define AS_OBJECT_HPP

# include "scriptengine.hpp"
# include <memory>
# include "observatory.hpp"

namespace AngelScript
{
  struct Exception : public std::exception
  {
  public:
    enum Code
    {
      UndeclaredFunction,
      UnloadableFunction,
      AngelScriptException,
      InternalError,
      CannotLoadContext,
      CannotLoadModule
    };

    Exception(Code code, const std::string& target = "")
    {
      switch (code)
      {
      case UndeclaredFunction:
        message = "The function '" + target + "' hasn't been decalred.";
        break ;
      case UnloadableFunction:
        message = "The function '" + target + "' couldn't be loaded.";
        break ;
      case InternalError:
        message = "An internal error has occured when executing function " + target;
        break ;
      case AngelScriptException:
        message = "An exception has been thrown inside the function " + target;
        break ;
      case CannotLoadContext:
        message = "Context couldn't be loaded.";
        break ;
      case CannotLoadModule:
        message = "Cannot load file " + target;
        break ;
      }
    }

    const char* what(void) const throw() { return (message.c_str()); }
  private:
    std::string message;
  };

  template<typename TYPE> struct TypeFlags         { enum { value = 'O' }; };
  template<>              struct TypeFlags<int>    { enum { value = 'i' }; };
  template<>              struct TypeFlags<bool>   { enum { value = 'b' }; };
  template<>              struct TypeFlags<float>  { enum { value = 'f' }; };
  template<>              struct TypeFlags<long>   { enum { value = 'l' }; };
  template<>              struct TypeFlags<double> { enum { value = 'd' }; };

  struct IType
  {
    virtual ~IType(void) {}
    virtual unsigned char Flag(void) const { return ('0'); }
    virtual void*         Ptr(void) = 0;
  };

  template<typename TYPE>
  class Type : public IType
  {
  public:
    Type(TYPE value) : value(value) {}

    operator TYPE() const
    {
      return (value);
    }

    unsigned char Flag(void) const
    {
      return (TypeFlags<TYPE>::value);
    }

    void* Ptr(void) { return (&value); }

  private:
    TYPE value;
  };

  template<typename TYPE>
  class Type<TYPE*> : public IType
  {
  public:
    Type(TYPE* instance) : instance(instance) {}
    
    operator void*() const
    {
      return (instance);
    }
    
    void* Ptr(void) { return (instance); }
    
  private:
    TYPE* instance;
  };

  class Object : public Sync::ObserverHandler
  {
    struct Function
    {
      std::string        signature;
      asIScriptFunction* function;
    };

    typedef std::map<std::string, Function> Functions;
  public:
    Object(const std::string& filepath);
    Object(asIScriptContext* context, const std::string& filepath);
    Object(asIScriptContext* context, asIScriptModule* module);
    Object(const Object& copy);
    ~Object();
    
    Sync::Signal<void>    ObjectDestroyed;

    std::weak_ptr<Object> GetPtr(void) { return (shared_ptr); }

    void                  LoadFromFile(const std::string& filepath);
  protected:
    void Initialize(void);
  public:
    void asDefineMethod(const std::string& name, const std::string& declaration);
    void asUndefineMethod(const std::string& name);
    bool IsDefined(const std::string& name) const
    {
      if (!module)
        return (false);
      return (functions.find(name) != functions.end());
    }
    
    asIScriptContext* GetContext(void) { return (context); }
    asIScriptModule*  GetModule(void)  { return (module);  }

    void OutputMethods(std::ostream& out)
    {
      auto it  = functions.begin();
      auto end = functions.end();

      for (; it != end ; ++it)
      {
        out << "  " << it->first << ' ' << it->second.signature << ' ' << it->second.function << endl;
      }
    }

    struct ReturnType
    {
      ReturnType(asIScriptContext* context, bool has_previous_context) : context(context), pop_context_on_release(has_previous_context)
      {
        reference_count  = new int;
        *reference_count = 1;
      }

      ReturnType(const ReturnType& copy)
      {
        context                = copy.context;
        pop_context_on_release = copy.pop_context_on_release;
        reference_count        = copy.reference_count;
        *reference_count      += 1;
      }

      ~ReturnType()
      {
        *reference_count -= 1;
        if (*reference_count == 0)
        {
          if (pop_context_on_release)
            context->PopState();
          delete reference_count;
        }
      }

      template<typename TYPE>
      operator TYPE() const
      {
        return (reinterpret_cast<TYPE>(context->GetReturnObject()));
      }

      operator bool()   const { return (context->GetReturnByte() != 0); }
      operator int()    const { return (context->GetReturnWord());  }
      operator long()   const { return (context->GetReturnDWord()); }
      operator float()  const { return (context->GetReturnFloat()); }
      operator double() const { return (context->GetReturnFloat()); }

    protected:
      asIScriptContext* context;
      bool              pop_context_on_release;
      int*              reference_count;
    };

    ReturnType Call(const std::string name, unsigned int argc = 0, ...);
  private:
    std::string             filepath;
    asIScriptContext*       context;
    asIScriptModule*        module;
    bool                    required_module, required_context;
    Functions               functions;
    std::shared_ptr<Object> shared_ptr;
  };

  struct ContextLock
  {
    //typedef std::weak_ptr<AngelScript::Object> ObjectPtr;
    typedef AngelScript::Object* ObjectPtr;
  public:
    ContextLock(asIScriptContext* context, asIScriptModule* module, AngelScript::Object* object)
    {
      old_context     = current_context;
      old_module      = current_module;
      current_context = context;
      current_module  = module;
      old_object      = current_object;
      //current_object  = object->GetPtr();
      current_object  = object;
    }

    ~ContextLock(void)
    {
      current_context = old_context;
      current_module  = old_module;
      current_object  = old_object;
    }

    static asIScriptContext* Context(void)       { return (current_context); }
    static asIScriptModule*  Module(void)        { return (current_module);  }
//    static Object*           CurrentObject(void) { return ((current_object.expired()) ? 0 : &(*(current_object.lock()))); }
    static Object*           CurrentObject(void) { return (current_object); }

  private:
    asIScriptContext*           old_context;
    asIScriptModule*            old_module;
    static asIScriptContext*    current_context;
    static asIScriptModule*     current_module;
    ObjectPtr                   old_object;
    static ObjectPtr            current_object;
  };

}

#endif
