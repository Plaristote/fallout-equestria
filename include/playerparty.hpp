#ifndef  PLAYERPARTY_HPP
# define PLAYERPARTY_HPP

# include "globals.hpp"
# include "serializer.hpp"

struct DynamicObject;
class  InstanceDynamicObject;

class Party
{
public:
  typedef std::list<DynamicObject*> DynamicObjects;
  
  void                  Join(InstanceDynamicObject* i);
  void                  Join(DynamicObject*);
  void                  Leave(InstanceDynamicObject* i);  
  void                  Leave(DynamicObject*);

  const DynamicObjects& ConstGetObjects(void) const { return (_objects); }
  DynamicObjects&       GetObjects(void)            { return (_objects); }

protected:
  DynamicObjects _objects;
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
  
  void                  Serialize(Utils::Packet&) const;
  void                  UnSerialize(Utils::Packet&);

  bool                  _local_objects;
};

#endif