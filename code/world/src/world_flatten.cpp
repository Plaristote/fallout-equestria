#include "world/world_flatten.hpp"

using namespace std;

LPoint3 NodePathSize(NodePath np);

struct BoundingZone
{
  BoundingZone operator+(BoundingZone other) const
  {
    LPoint3f lowest_point  = GetLowestPoint(other);
    LPoint3f highest_point = GetHighestPoint(other);

    other.position = position;
    other.size     = highest_point - lowest_point;
    return (other);
  }

  LPoint3f GetLowestPoint(BoundingZone other) const
  {
    LPoint3f self_lowest  = GetLowestExtremity();
    LPoint3f other_lowest = other.GetLowestExtremity();
    
    self_lowest.set_x(self_lowest.get_x() < other_lowest.get_x() ? self_lowest.get_x() : other_lowest.get_x());
    self_lowest.set_y(self_lowest.get_y() < other_lowest.get_y() ? self_lowest.get_y() : other_lowest.get_y());
    self_lowest.set_z(self_lowest.get_z() < other_lowest.get_z() ? self_lowest.get_z() : other_lowest.get_z());
    return (self_lowest);
  }

  LPoint3f GetHighestPoint(BoundingZone other) const
  {
    LPoint3f self_highest  = GetHighestExtremity();
    LPoint3f other_highest = other.GetHighestExtremity();
    
    self_highest.set_x(self_highest.get_x() < other_highest.get_x() ? self_highest.get_x() : other_highest.get_x());
    self_highest.set_y(self_highest.get_y() < other_highest.get_y() ? self_highest.get_y() : other_highest.get_y());
    self_highest.set_z(self_highest.get_z() < other_highest.get_z() ? self_highest.get_z() : other_highest.get_z());
    return (self_highest);
  }

  LPoint3f GetLowestExtremity(void) const
  {
    return (position - (size / 2));
  }
  
  LPoint3f GetHighestExtremity(void) const
  {
    return (position + (size / 2));
  }
  
  void SetFromObject(MapObject* object, NodePath root)
  {
    position = object->nodePath.get_pos(root);
    size     = NodePathSize(object->render);
  }
  
  LPoint3f position;
  LPoint3f size;
};

WorldFlattener::WorldFlattener(World& world) : world(world)
{
}

void WorldFlattener::Flatten(void)
{
  MakeFloorGroups();
  MakeTextureGroups(floor_groups);
  MakeTextureGroups(wall_groups);
  flatten_map.OutputFlattenMap();
  flatten_map.FlattenWorld(world);
}

void WorldFlattener::FindChildForObject(BranchGroup& group, MapObject* parent)
{
  auto it  = world.objects.begin();
  auto end = world.objects.end();
  
  for (; it != end ; ++it)
  {
    MapObject& object = *it;

    if (object.render.is_empty() || object.IsCuttable())
      continue ;
    if (it->parent == parent->name &&
        (find(group.objects.begin(), group.objects.end(), &object) == group.objects.end()))
    {
      group.objects.push_back(&object);
      FindChildForObject(group, &object);
    }
  }
}

void WorldFlattener::MakeFloorGroups(void)
{
  auto it  = world.objects.begin();
  auto end = world.objects.end();

  for (; it != end ; ++it)
  {
    MapObject&  object = *it;
    
    if (object.parent == "")
    {
      BranchGroup group(object.name);
      
      group.objects.push_back(&object);
      FindChildForObject(group, &object);
      floor_groups.push_back(group);
    }
  }
  
  for (it = world.objects.begin() ; it != end ; ++it)
  {
    MapObject&  object = *it;

    if (object.IsCuttable())
    {
      WallGroup& group = RequireGroup(wall_groups, object.floor);

      group.objects.push_back(&object);
      FindChildForObject(group, &object);
    }
  }
  
  for_each(floor_groups.begin(), floor_groups.end(), [](BranchGroup& group)
  {
    cout << "[GROUP] " << group.root << endl;
    for_each(group.objects.begin(), group.objects.end(), [](MapObject* object)
    {
      cout << object->name << endl;
    });
    cout << endl;
  });
  
  for_each(wall_groups.begin(), wall_groups.end(), [](WallGroup& group)
  {
    cout << "[GROUP] " << group.root << endl;
    for_each(group.objects.begin(), group.objects.end(), [](MapObject* object)
    {
      cout << object->name << endl;
    });
    cout << endl;
  });

  cout << "Regular groups: " << floor_groups.size() << ", wall groups: " << wall_groups.size() << endl;
}

void WorldFlattener::MakeTextureGroups(WorldFlattener::BranchGroup& floor_group)
{
  auto it  = floor_group.objects.begin();
  auto end = floor_group.objects.end();

  for (;it != end ; ++it)
  {
    MapObject*    object        = *it;
    TextureGroup& texture_group = RequireGroup(floor_group.texture_groups, object->strTexture);

    texture_group.objects.push_back(object);
  }
}

void WorldFlattener::MakeZoneGroups(WorldFlattener::BranchGroup& floor_group)
{
  auto it  = floor_group.texture_groups.begin();
  auto end = floor_group.texture_groups.end();
  
  for (; it != end ; ++it)
    MakeZoneGroups(*it);
}

static float ZoneHeuristic(BoundingZone first, BoundingZone second)
{
  LPoint3f size = (first + second).size;

  return (size.get_x() * size.get_y());
}

float WorldFlattener::GetCombinedArea(MapObject* first, MapObject* second)
{
  BoundingZone zone_1, zone_2;

  zone_1.SetFromObject(first, world.window->get_render());
  zone_2.SetFromObject(second, world.window->get_render());
  return (ZoneHeuristic(zone_1, zone_2));
}

std::pair<MapObject*, MapObject*> WorldFlattener::SmallestCombination(const std::list<MapObject*>& objects)
{
  auto                        first  = objects.begin();
  auto                        end    = objects.end();
  pair<MapObject*,MapObject*> best_pair(nullptr, nullptr);
  float                       smallest_value = -1;

  for (; first != end ; ++first)
  {
    auto         second   = first;

    second++;
    for (; second != end ; ++second)
    {
      float        value;

      value = GetCombinedArea(*first, *second);
      if (value < smallest_value || smallest_value == -1)
      {
        smallest_value = value;
        best_pair      = pair<MapObject*,MapObject*>(*first, *second);
      }
    }
  }
  return (best_pair);
}

void WorldFlattener::FlattenObjects(MapObject* first, MapObject* second)
{
  NodePath target            = first->render;
  LPoint3f relative_position = second->nodePath.get_pos(target);
  LPoint3f relative_hpr      = second->nodePath.get_hpr(target);

  second->render.reparent_to(target);
  second->render.set_pos(relative_position);
  second->render.set_hpr(relative_hpr);
  target.clear_model_nodes();
  target.flatten_strong();
  second->render = target;
}

void WorldFlattener::MakeZoneGroups(WorldFlattener::TextureGroup& group)
{
  list<MapObject*> list     = group.objects;
  float            max_size = 150;

  do
  {
    auto  smallest_combination = SmallestCombination(list);
    
    if (smallest_combination.first == 0 || smallest_combination.second == 0)
    {
      cout << "No more objects left" << endl;
      break ;
    }
    else
    {
      float size               = GetCombinedArea(smallest_combination.first, smallest_combination.second);

      cout << "Combined area for " << smallest_combination.first->name << ", " << smallest_combination.second->name << " is " << size << endl;
      if (size > max_size)
        break ;
      flatten_map.AddToGroup(smallest_combination.first->name, smallest_combination.second->name);
      list.remove(smallest_combination.second);
    }
  } while (list.size() > 1);
}
