#ifndef  PLAYERPARTY_HPP
# define PLAYERPARTY_HPP

# include "globals.hpp"
# include "serializer.hpp"

class DynamicObject;

class PlayerParty
{
public:
  typedef std::list<DynamicObject*> DynamicObjects;
  PlayerParty(const std::string& savepath);
  ~PlayerParty(void);
  
  static void           Create(const std::string& savepath, const std::string& name, const std::string& model, const std::string& texture);

  bool                  Save(const std::string& savepath) const;
  
  void                  Join(DynamicObject*);
  void                  Leave(DynamicObject*);
  
  void                  SetHasLocalObjects(bool val) { _local_objects = val; }

  const DynamicObjects& ConstGetObjects(void) const { return (_objects); }
  DynamicObjects&       GetObjects(void)            { return (_objects); }
private:
  PlayerParty() {};
  
  void                  Serialize(Utils::Packet&) const;
  void                  UnSerialize(Utils::Packet&);

  DynamicObjects        _objects;
  bool                  _local_objects;
};

#endif