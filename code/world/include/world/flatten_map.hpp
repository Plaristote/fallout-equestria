#ifndef  FLATTEN_MAP_HPP
# define FLATTEN_MAP_HPP

# include "serializer.hpp"
# include "world/world.h"
# include <list>

class FlattenMap : public Utils::Serializable
{
  struct Group : public Utils::Serializable
  {
    bool operator==(const std::string& comp) const { return (parent == comp); }
    
    void Serialize(Utils::Packet& packet) const { packet << parent << children; }
    void Unserialize(Utils::Packet& packet)     { packet >> parent >> children; }
    
    std::string            parent;
    std::list<std::string> children;
  };

  typedef std::vector<Group> Groups;
public:
  void FlattenWorld(World& world);
  void FlattenGroup(World& world, const Group& group) const;
  void FlattenObjects(MapObject* first, MapObject* second) const;
  void AddToGroup(const std::string& group_name, const std::string& object);
  void AddToGroup(Group& group, const std::string& object);

  void OutputFlattenMap(void) const;

  void Serialize(Utils::Packet& packet) const  { packet << groups; }
  void Unserialize(Utils::Packet& packet)      { packet >> groups; }

private:
  Groups groups;
};


#endif