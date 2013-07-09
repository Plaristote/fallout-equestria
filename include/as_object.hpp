#ifndef  AS_OBJECT_HPP
# define AS_OBJECT_HPP

# include "scriptengine.hpp"
#include <boost/concept_check.hpp>

namespace AngelScript
{
  class Object;
  struct ContextLock
  {
  public:
    ContextLock(asIScriptContext* context, asIScriptModule* module, AngelScript::Object* object)
    {
      old_context     = current_context;
      old_module      = current_module;
      current_context = context;
      current_module  = module;
      old_object      = current_object;
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
    static Object*           CurrentObject(void) { return (current_object);  }

  private:
    asIScriptContext*        old_context;
    asIScriptModule*         old_module;
    static asIScriptContext* current_context;
    static asIScriptModule*  current_module;
    AngelScript::Object*        old_object;
    static AngelScript::Object* current_object;
  };

  struct Exception : public std::exception
  {
  public:
    enum Code
    {
      UndeclaredFunction,
      UnloadableFunction
    };

    Exception(Code code, const std::string& target = "")
    {
      switch (code)
      {
      case UndeclaredFunction:
        message = "The function '" + target + "' hasn't been decalred.";
      case UnloadableFunction:
        message = "The function '" + target + "' couldn't be loaded.";
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

  class Object
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
    ~Object();
  protected:
    void Initialize(void);    
  public:
    void asDefineMethod(const std::string& name, const std::string& declaration);
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
      ReturnType(asIScriptContext* context) : context(context) {}

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
    };

    ReturnType Call(const std::string& name, unsigned int argc = 0, ...);
  private:
    const std::string filepath;
    asIScriptContext* context;
    asIScriptModule*  module;
    bool              required_module, required_context;
    Functions         functions;
  };
}

#endif
