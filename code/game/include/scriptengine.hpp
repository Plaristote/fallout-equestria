#ifndef  SCRIPTENGINE_HPP
# define SCRIPTENGINE_HPP

# include "globals.hpp"
# include <angelscript.h>
# include <scriptbuilder/scriptbuilder.h>
# include <scriptstdstring/scriptstdstring.h>
# include "observatory.hpp"
# include <stdarg.h>

namespace Script
{
  void     Call(asIScriptContext*, asIScriptFunction*, const std::string fmt, ...);
  
  class Engine
  {
  public:
    static asIScriptEngine* Get(void) { return (_engine);  }
    static void             Initialize(void);
    static void             Finalize(void);
    static asIScriptModule* LoadModule(const std::string& name, const std::string& filepath);

    static Sync::Signal<void (const std::string)> ScriptError;

  private:
    static void             MessageCallback(const asSMessageInfo* msg, void* param);
    
    static asIScriptEngine* _engine;
  };
  
  class ModuleManager
  {
  public:
    static asIScriptModule* Require(const std::string& name, const std::string& filepath);
    static void             Release(asIScriptModule* module);
    static void             OutputFunctionList(asIScriptModule* module);
    
  private:
    struct LoadedModule
    {
      bool operator==(asIScriptModule* module) const { return (ptr == module);   }
      bool operator==(const std::string& str)  const { return (filepath == str); }
      
      std::string       filepath;
      asIScriptModule*  ptr;
      unsigned short    users;
    };
    typedef std::list<LoadedModule> Modules;
    
    static Modules _modules;
  };
  
  template<typename T>
  class StdList : public std::list<T>
  {
  public:
    StdList(void) {}
    StdList(std::list<T> list) : std::list<T>(list)
    {}

    void Add(T t)
    {
      this->push_back(t);
    }

    struct asIterator
    {
      static void Constructor(void* memory) { new(memory) asIterator();  }
      static void Destructor (void* memory) {                            }
      static T    Assign(asIterator it)     { return (*(it.it));         }

      const asIterator& operator=(const asIterator& copy)
      {
	it = copy.it;
	return (*this);
      }
      
      T    Value(void)
      {
	return (*it);
      }

      bool Equal(asIterator comp) const  { return (comp.it == it); }
      void Increment(void)               { ++it;                   }
      void Decrement(void)               { --it;                   }

      typename std::list<T>::iterator it;
      StdList<T>*                     self;
    };

    struct asRIterator
    {
      static void Constructor(void* memory) { new(memory) asRIterator(); }
      static void Destructor (void* memory) {                            }
      static T    Assign(asRIterator it)    { return (*(it.it));         }

      const asRIterator& operator=(const asRIterator& copy)
      {
	it = copy.it;
	return (*this);
      }
      
      T    Value(void)
      {
	return (*it);
      }

      bool Equal(asRIterator comp) const { return (comp.it == it); }
      void Increment(void)               { ++it;                   }
      void Decrement(void)               { --it;                   }

      typename std::list<T>::reverse_iterator it;
      StdList<T>*                             self;
    };
    
    static void Register(asIScriptEngine* engine, const std::string& arrayName, const std::string& typeName)
    {
      const std::string className = (typeName[typeName.length() - 1] == '@') ? typeName.substr(0, typeName.length() - 1) : typeName;
      const std::string type      = arrayName;
      const std::string itType    = arrayName + "Iterator";
      const std::string ritType   = arrayName + "RIterator";

      engine->RegisterObjectType(type.c_str(), sizeof(StdList), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR);
      engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StdList<T>::Constructor), asCALL_CDECL_OBJLAST);
      engine->RegisterObjectBehaviour(type.c_str(), asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(StdList<T>::Destructor),  asCALL_CDECL_OBJLAST);
      
      engine->RegisterObjectMethod(type.c_str(), ("void Add(" + typeName + ")").c_str(), asMETHOD(StdList<T>,Add), asCALL_THISCALL);
      
      engine->RegisterObjectType(itType.c_str(), sizeof(asIterator), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR);
      engine->RegisterObjectBehaviour(itType.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StdList<T>::asIterator::Constructor), asCALL_CDECL_OBJLAST);
      engine->RegisterObjectBehaviour(itType.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(StdList<T>::asIterator::Destructor), asCALL_CDECL_OBJLAST);
      
      engine->RegisterObjectType(ritType.c_str(), sizeof(asRIterator), asOBJ_VALUE | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR);
      engine->RegisterObjectBehaviour(ritType.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(StdList<T>::asRIterator::Constructor), asCALL_CDECL_OBJLAST);
      engine->RegisterObjectBehaviour(ritType.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(StdList<T>::asRIterator::Destructor), asCALL_CDECL_OBJLAST);      

      engine->RegisterObjectMethod(type.c_str(), ("const " + type + " &opAssign(const " + type + " &in)").c_str(), asMETHODPR(StdList<T>,operator=, (const StdList<T>&), const StdList<T>&), asCALL_THISCALL);
      engine->RegisterObjectMethod(type.c_str(), "int Size()", asMETHOD(StdList<T>,Size), asCALL_THISCALL);
      engine->RegisterObjectMethod(type.c_str(), (itType + " Begin()").c_str(), asMETHOD(StdList<T>,Begin), asCALL_THISCALL);
      engine->RegisterObjectMethod(type.c_str(), (itType + " End()").c_str(), asMETHOD(StdList<T>,End), asCALL_THISCALL);
      engine->RegisterObjectMethod(type.c_str(), (ritType + " RBegin()").c_str(), asMETHOD(StdList<T>,RBegin), asCALL_THISCALL);
      engine->RegisterObjectMethod(type.c_str(), (ritType + " REnd()").c_str(), asMETHOD(StdList<T>,REnd), asCALL_THISCALL);
      engine->RegisterObjectMethod(type.c_str(), (typeName + " opIndex(int)").c_str(), asMETHODPR(StdList<T>,operator[],(unsigned int i),T), asCALL_THISCALL);

      engine->RegisterObjectMethod(itType.c_str(), ("const " + itType + " &opAssign(const " + itType + " &in)").c_str(), asMETHODPR(StdList<T>::asIterator,operator=, (const StdList<T>::asIterator&), const StdList<T>::asIterator&), asCALL_THISCALL);
      engine->RegisterObjectMethod(itType.c_str(), (typeName + " Value()").c_str(), asMETHOD(StdList<T>::asIterator,Value), asCALL_THISCALL);
      engine->RegisterObjectMethod(itType.c_str(), "void opPostInc()", asMETHOD(StdList<T>::asIterator,Increment), asCALL_THISCALL);
      engine->RegisterObjectMethod(itType.c_str(), "void opPostDec()", asMETHOD(StdList<T>::asIterator,Decrement), asCALL_THISCALL);
      engine->RegisterObjectMethod(itType.c_str(), ("bool opEquals(" + itType + ")").c_str(),  asMETHOD(StdList<T>::asIterator,Equal),     asCALL_THISCALL);
      engine->RegisterObjectMethod(className.c_str(), (typeName + " opAssign(" + itType + ")").c_str(), asFUNCTION(StdList<T>::asIterator::Assign), asCALL_CDECL_OBJFIRST);

      engine->RegisterObjectMethod(ritType.c_str(), ("const " + ritType + " &opAssign(const " + ritType + " &in)").c_str(), asMETHODPR(StdList<T>::asRIterator,operator=, (const StdList<T>::asRIterator&), const StdList<T>::asRIterator&), asCALL_THISCALL);
      engine->RegisterObjectMethod(ritType.c_str(), (typeName + " Value()").c_str(), asMETHOD(StdList<T>::asRIterator,Value), asCALL_THISCALL);
      engine->RegisterObjectMethod(ritType.c_str(), "void opPostInc()", asMETHOD(StdList<T>::asRIterator,Increment), asCALL_THISCALL);
      engine->RegisterObjectMethod(ritType.c_str(), "void opPostDec()", asMETHOD(StdList<T>::asRIterator,Decrement), asCALL_THISCALL);
      engine->RegisterObjectMethod(ritType.c_str(), ("bool opEquals(" + ritType + ")").c_str(),  asMETHOD(StdList<T>::asRIterator,Equal),     asCALL_THISCALL);      
      engine->RegisterObjectMethod(className.c_str(), (typeName + " opAssign(" + ritType + ")").c_str(), asFUNCTION(StdList<T>::asRIterator::Assign), asCALL_CDECL_OBJFIRST);
    }

    static void Constructor(void* memory) { new(memory) StdList();            }
    static void Destructor (void* memory) { ((StdList*)memory)->~StdList(); }

    ~StdList() {}
    
    const StdList& operator=(const StdList& cpy)
    {
      if (this != &cpy)
	std::list<T>::operator=(cpy);
      return (*this);
    }

    unsigned int Size(void) { return std::list<T>::size(); }
    
    T          operator[](unsigned int i)
    {
      auto it  = std::list<T>::begin();
      auto end = std::list<T>::end();

      for (unsigned int cur = 0 ; it != end && cur < i ; ++it, ++cur);
      return (*it);
    }

    asIterator Begin(void)
    {
      asIterator toRet;

      toRet.it   = std::list<T>::begin();
      toRet.self = this;
      return (toRet);
    }
    
    asRIterator RBegin(void)
    {
      asRIterator toRet;

      toRet.it   = std::list<T>::rbegin();
      toRet.self = this;
      return (toRet);
    }
    
    asIterator End(void)
    {
      asIterator toRet;

      toRet.it   = std::list<T>::end();
      toRet.self = this;
      return (toRet);
    }
    
    asRIterator REnd(void)
    {
      asRIterator toRet;

      toRet.it   = std::list<T>::rend();
      toRet.self = this;
      return (toRet);
    }

  private:
  };  
}

#endif
