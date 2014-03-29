#include "level/visibility_halo.hpp"
#include "level/objects/instance_dynamic_object.hpp"
#include <panda3d/stencilAttrib.h>
#include <panda3d/colorBlendAttrib.h>
#include <panda3d/depthTestAttrib.h>
#include <panda3d/cullFaceAttrib.h>

using namespace std;

VisibilityHalo::VisibilityHalo() : target(0)
{
}

VisibilityHalo::~VisibilityHalo()
{
  halo.remove_node();
}

void VisibilityHalo::MarkCuttableObjects(World* world)
{
  CPT(RenderAttrib) attribute = StencilAttrib::make(1, StencilAttrib::SCF_not_equal, StencilAttrib::SO_keep, StencilAttrib::SO_keep,    StencilAttrib::SO_keep,    1, 1, 0);

  for_each(world->objects.begin(), world->objects.end(), [this, attribute](MapObject& object)
  {
    if (IsObjectCuttable(object))
      object.nodePath.set_attrib(attribute);
  });
}

bool VisibilityHalo::IsObjectCuttable(const MapObject &object)
{
  string name       = object.nodePath.get_name();
  string patterns[] = { "Wall", "wall", "Ceiling", "ceiling" };

  for (unsigned short i = 0 ; i < 4 ; ++i)
  {
    if (starts_with(name, patterns[i]))
      return (true);
  }
  return (false);
}

void VisibilityHalo::Initialize(WindowFramework* window, World* world)
{
  PandaNode*        node;
  CPT(RenderAttrib) attribute = StencilAttrib::make(1, StencilAttrib::SCF_always,    StencilAttrib::SO_zero, StencilAttrib::SO_replace, StencilAttrib::SO_replace, 1, 0, 1);

  halo = window->load_model(window->get_panda_framework()->get_models(), "misc/sphere");
  if ((halo.node()) != 0)
  {
    halo.set_scale(5);
    halo.set_bin("background", 0);
    halo.set_depth_write(0);
    halo.reparent_to(window->get_render());
    node        = halo.node();
    node->set_attrib(attribute);
    node->set_attrib(ColorBlendAttrib::make(ColorBlendAttrib::M_add));
    MarkCuttableObjects(world);
  }
}

void VisibilityHalo::SetTarget(InstanceDynamicObject* target)
{
  this->target = target;
  if (target)
    halo.reparent_to(target->GetNodePath());
  else
    halo.detach_node();
}
