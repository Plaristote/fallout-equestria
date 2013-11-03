#include "mouse.h"
#include "world.h"

using namespace std;

Mouse::Mouse(WindowFramework* window, QObject *parent) : QObject(parent), _window(window)
{
  MouseWatcher::init_type();
  _camera       = window->get_camera_group();
  _mouseWatcher = dynamic_cast<MouseWatcher*>(window->get_mouse().node());
  _pickerNode   = new CollisionNode("mouseRay");
  _pickerPath   = _camera.attach_new_node(_pickerNode);
  _pickerNode->set_from_collide_mask(CollideMask(ColMask::DynObject | ColMask::Object));
  _pickerRay    = new CollisionRay();
#ifndef _WIN32
  _pickerNode->add_solid(_pickerRay);
#endif
  _collisionHandlerQueue = new CollisionHandlerQueue();
  _collisionTraverser.add_collider(_pickerPath, _collisionHandlerQueue);
}

LPoint2f Mouse::GetPosition(void) const
{
  return (_mouseWatcher->get_mouse());
}

NodePath NodePathGetChild(NodePath np, const std::string& name)
{
  for (int i = 0 ; i < np.get_num_children() ; ++i)
  {
    NodePath child = np.get_child(i);

    if (child.get_name() == name)
        return (child);
  }
  return (NodePath());
}

void Mouse::GetWaypointHoveredAt(LPoint2f cursorPos, NodePath wp_base)
{
  wp_base.set_collide_mask(CollideMask(ColMask::Waypoint));
  _pickerNode->set_from_collide_mask(CollideMask(ColMask::Waypoint));
  _pickerRay->set_from_lens(_window->get_camera(0), cursorPos.get_x(), cursorPos.get_y());
  _collisionTraverser.traverse(wp_base);
  _collisionHandlerQueue->sort_entries();
  for (int i = 0 ; i < _collisionHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry = _collisionHandlerQueue->get_entry(i);
    NodePath        into  = entry->get_into_node_path();

    emit WaypointHovered(into);
  }
}

void Mouse::GetHoveredAt(LPoint2f cursorPos)
{
  _pickerNode->set_from_collide_mask(CollideMask(ColMask::DynObject | ColMask::Object));
  _pickerRay->set_from_lens(_window->get_camera(0), cursorPos.get_x(), cursorPos.get_y());
  _collisionTraverser.traverse(_window->get_render());
  _collisionHandlerQueue->sort_entries();
  for (int i = 0 ; i < _collisionHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry = _collisionHandlerQueue->get_entry(i);
    NodePath into         = entry->get_into_node_path();

    switch (into.get_collide_mask().get_word())
    {
    case ColMask::DynObject:
        emit UnitHovered(into);
        break ;
    case (ColMask::Object | ColMask::WpPlane):
    case ColMask::Object:
        emit ObjectHovered(into);
        break ;
    }
    break ;
  }
}

void Mouse::Run(void)
{
  if (_mouseWatcher->has_mouse())
  {
    LPoint2f cursorPos = _mouseWatcher->get_mouse();

    GetHoveredAt(cursorPos);
  }
}
