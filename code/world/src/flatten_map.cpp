#include "world/flatten_map.hpp"

using namespace std;

void FlattenMap::FlattenWorld(World& world)
{
  auto it  = groups.begin();
  auto end = groups.end();
  
  for (; it != end ; ++it)
    FlattenGroup(world, *it);
}

void FlattenMap::FlattenGroup(World& world, const Group& group) const
{
  MapObject* root = world.GetMapObjectFromName(group.parent);

  if (root)
  {
    auto     it   = group.children.begin();
    auto     end  = group.children.end();

    for (; it != end ; ++it)
    {
      MapObject* other = world.GetMapObjectFromName(*it);
      
      FlattenObjects(root, other);
    }
  }
}

void FlattenMap::FlattenObjects(MapObject* first, MapObject* second) const
{
  if (!first->render.is_empty() && !second->render.is_empty())
  {
    NodePath target            = first->render;
    LPoint3f relative_position = second->nodePath.get_pos(target);
    LPoint3f relative_hpr      = second->nodePath.get_hpr(target);
    LPoint3f relative_scale    = second->nodePath.get_scale(target);

    second->render.reparent_to(target);
    second->render.set_pos(relative_position);
    second->render.set_hpr(relative_hpr);
    second->render.set_scale(relative_scale);
    target.clear_model_nodes();
    target.flatten_strong();
    second->render = NodePath();
  }
}

void FlattenMap::AddToGroup(const std::string& group_name, const std::string& object)
{
  auto group = std::find(groups.rbegin(), groups.rend(), group_name);

  if (group == groups.rend())
  {
    Group new_group;

    new_group.parent = group_name;
    groups.push_back(new_group);
    group = groups.rbegin();
  }
  AddToGroup(*group, object);
}

void FlattenMap::AddToGroup(Group& group, const std::string& object)
{
  auto object_group = std::find(groups.begin(), groups.end(), object);

  group.children.push_back(object);
  if (object_group != groups.end())
  {
    auto it  = object_group->children.begin();
    auto end = object_group->children.end();
    
    for (; it != end ; ++it)
      group.children.push_back(*it);
    groups.erase(object_group);
  }
}

void FlattenMap::OutputFlattenMap(void) const
{
  for_each(groups.begin(), groups.end(), [](const Group& group)
  {
    cout << "[GROUP] " << group.parent << endl;
    for_each(group.children.begin(), group.children.end(), [](const std::string& child)
    {
      cout << child << ' ';
    });
    cout << endl << endl;
  });
  cout << endl;
}
