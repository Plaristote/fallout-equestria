#ifndef  PLAYERPARTY_HPP
# define PLAYERPARTY_HPP

# include "globals.hpp"
# include "serializer.hpp"
# include "observatory.hpp"
# include <map>

struct DynamicObject;
class  InstanceDynamicObject;

class Party
{
public:
  typedef std::list<DynamicObject*>          DynamicObjects;
  typedef std::map<std::string, std::string> Statsheets;

  void                  Join(InstanceDynamicObject* i);
  void                  Join(DynamicObject*);
  void                  Leave(InstanceDynamicObject* i);  
  void                  Leave(DynamicObject*);

  void                  Export(const std::string& name) const;
  static Party*         Import(const std::string& name);

  const DynamicObjects& ConstGetObjects(void) const { return (_objects); }
  DynamicObjects&       GetObjects(void)            { return (_objects); }
  Statsheets            GetStatsheets(void)   const;

  Sync::Signal<void>    Updated;
  
protected:
  void                  Serialize(Utils::Packet&) const;
  void                  UnSerialize(Utils::Packet&);

  DynamicObjects        _objects;
  std::string           _name;
};

class PlayerParty : public Party
{
public:
  typedef std::list<DynamicObject*> DynamicObjects;
  PlayerParty(const std::string& savepath);
  ~PlayerParty(void);
  
  static void           Create(const std::string& savepath, const std::string& name, const std::string& model, const std::string& texture);
  bool                  Save(const std::string& savepath) const;
  void                  SetHasLocalObjects(bool val) { _local_objects = val; }
  
private:
  PlayerParty() {};
  
  bool                  _local_objects;
};

#endif