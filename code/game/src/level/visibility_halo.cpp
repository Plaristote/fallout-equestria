#include "level/visibility_halo.hpp"
#include "level/objectnode.hpp"
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

bool VisibilityHalo::IsObjectCuttable(const MapObject &object) const
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
  CPT(RenderAttrib) attribute = StencilAttrib::make(1, StencilAttrib::SCF_not_equal, StencilAttrib::SO_keep, StencilAttrib::SO_keep,    StencilAttrib::SO_keep,    1, 1, 0);

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

void VisibilityHalo::Run(void)
{
  if (!(halo.is_empty()))
  {
    NodePath target_nodepath = target->GetNodePath();

    halo.set_pos(target_nodepath.get_pos());
    halo.set_hpr(target_nodepath.get_hpr());
  }
}
