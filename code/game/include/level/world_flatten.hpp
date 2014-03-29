#ifndef  WORLD_FLATTEN_HPP
# define WORLD_FLATTEN_HPP

# include "globals.hpp"
# include "world/world.h"
# include <list>

struct WorldFlattener
{
  struct ZoneGroup
  {
    LPoint3f              size;
    std::list<MapObject*> objects;
  };
  
  struct TextureGroup
  {
    TextureGroup(const std::string& texture) : texture(texture) {}
    bool operator==(const std::string& comp) const { return (texture == comp); }
    
    std::string           texture;
    std::list<MapObject*> objects;
    std::list<ZoneGroup>  zone_groups;
  };
  
  struct BranchGroup
  {
    BranchGroup(void) {}
    BranchGroup(std::string root) : root(root) {}
    bool operator==(std::string comp) const { return (root == comp); }

    std::string             root;
    std::list<MapObject*>   objects;
    std::list<TextureGroup> texture_groups;
  };
  
  struct WallGroup : public BranchGroup
  {
    WallGroup(unsigned char floor) : floor(floor) {}
    bool operator==(unsigned char comp) const { return (floor == comp); }
    
    unsigned char           floor;
  };

public:
  WorldFlattener(World& world);

  void Flatten(void);

private:
  void MakeFloorGroups(void);

  template<typename T>
  void MakeTextureGroups(std::list<T>& groups)
  {
    auto it  = groups.begin();
    auto end = groups.end();

    for (; it != end ; ++it)
    {
      MakeTextureGroups(*it);
      MakeZoneGroups(*it);
    }
  }
  
  void MakeTextureGroups(BranchGroup&);
  void MakeZoneGroups(BranchGroup&);
  void MakeZoneGroups(TextureGroup&);
  void FindChildForObject(BranchGroup&, MapObject*);
  
  std::pair<MapObject*, MapObject*> SmallestCombination(const std::list<MapObject*>&);
  float                             GetCombinedArea(MapObject*, MapObject*);
  void                              FlattenObjects(MapObject*, MapObject*);
  
  template<typename T, typename P>
  static T&   RequireGroup(std::list<T>& array, P key)
  {
    auto it = std::find(array.rbegin(), array.rend(), key);

    if (it == array.rend())
    {
      array.push_back(T(key));
      it = array.rbegin();
    }
    return (*it);
  }

  World&                  world;
  std::list<BranchGroup>  floor_groups;
  std::list<TextureGroup> texture_groups;
  std::list<WallGroup>    wall_groups;
};


#endif