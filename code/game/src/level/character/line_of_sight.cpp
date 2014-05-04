#include "level/characters/line_of_sight.hpp"
#include "level/objects/character.hpp"
#include "world/world.h"

using namespace std;

LineOfSight::LineOfSight(World& world, NodePath parent_node, NodePath self) :
  world(world),
  self_nodepath(self)
{
  InitializeCollisionNode();
  collision_nodepath = parent_node.attach_new_node(collision_node);
  collision_nodepath.set_pos(0, 0, 0);
  //collision_nodepath.show();
}

LineOfSight::~LineOfSight()
{
  collision_node->remove_solid(0);
  collision_nodepath.remove_node();
}

void LineOfSight::InitializeCollisionNode()
{
  if (collision_node.is_null())
  {
    InitializeRay();
    collision_node = new CollisionNode("losRay");
    collision_node->set_from_collide_mask(CollideMask(ColMask::FovBlocker | ColMask::FovTarget));
    collision_node->set_into_collide_mask(0);
    collision_node->add_solid(ray);
  }
}

void LineOfSight::InitializeRay()
{
  if (ray.is_null())
  {
    ray            = new CollisionSegment();
    ray->set_point_a(0, 0, 0);
    ray->set_point_b(-10, 0, 0);
  }
}

bool LineOfSight::HasLineOfSight(const InstanceDynamicObject* target) const
{
  CollisionTraverser        collision_traverser;
  PT(CollisionHandlerQueue) handler_queue     = new CollisionHandlerQueue();
  bool                      has_line_of_sight = true;
  NodePath                  target_nodepath   = target->GetNodePath();
  LVector3                  self_position     = self_nodepath.get_pos();
  LVector3                  target_position   = target_nodepath.get_pos();

  ray->set_point_a(self_position.get_x(),   self_position.get_y(),   self_position.get_z()   + 4.f);
  ray->set_point_b(target_position.get_x(), target_position.get_y(), target_position.get_z() + 4.f);
  collision_traverser.add_collider(collision_nodepath, handler_queue);
  collision_traverser.traverse(world.floors_node);
  handler_queue->sort_entries();
  for (int i = 0 ; i < handler_queue->get_num_entries() && has_line_of_sight == true ; ++i)
  {
    CollisionEntry* entry = handler_queue->get_entry(i);
    NodePath        node  = entry->get_into_node_path();
    unsigned int    mask  = node.get_collide_mask().get_word();
    
    if (mask & ColMask::FovBlocker)
    {
      if (mask & ColMask::CheckCollisionOnModel)
        has_line_of_sight = DoesRayTraverseModel(node);
      else
	has_line_of_sight = false;
    }
  }
  return (has_line_of_sight);
}

bool LineOfSight::DoesRayTraverseModel(NodePath model) const
{
  MapObject* map_object = world.GetMapObjectFromCollisionNode(model);
  
  if (map_object)
  {
    CollisionTraverser        model_traverser;
    PT(CollisionHandlerQueue) handler_queue = new CollisionHandlerQueue();
    CollideMask               initial_collide_mask = map_object->render.get_collide_mask();

    map_object->render.set_collide_mask(initial_collide_mask | CollideMask(ColMask::FovBlocker));
    model_traverser.add_collider(collision_nodepath, handler_queue);
    model_traverser.traverse(map_object->render);
    map_object->render.set_collide_mask(initial_collide_mask);
    if (handler_queue->get_num_entries() > 0)
      return (false);
  }
  return (true);
}
