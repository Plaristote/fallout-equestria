#ifndef  LINE_OF_SIGHT_HPP
# define LINE_OF_SIGHT_HPP

# include "globals.hpp"
# include <panda3d/collisionRay.h>
# include <panda3d/collisionSegment.h>
# include <panda3d/collisionTraverser.h>
# include <panda3d/collisionHandlerQueue.h>

class InstanceDynamicObject;
class World;

class LineOfSight
{
public:
  LineOfSight(World&, NodePath parent_node, NodePath self_nodepath);
  ~LineOfSight();

  bool                      HasLineOfSight(const InstanceDynamicObject* target) const;
  
private:
  void                      InitializeCollisionNode();
  void                      InitializeRay();
  
  bool                      DoesRayTraverseModel(NodePath) const;
  
  World&                    world;
  NodePath                  self_nodepath;
  NodePath                  collision_nodepath;
  PT(CollisionNode)         collision_node;
  PT(CollisionSegment)      ray;
};

#endif
