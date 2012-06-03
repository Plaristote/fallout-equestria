#include "mouse.hpp"
#include <tilemap.hpp>
#include "world.h"

using namespace std;

Mouse::Mouse(WindowFramework* window) : _window(window)
{
  MouseWatcher::init_type();
  _camera       = window->get_camera_group();
  _mouseWatcher = dynamic_cast<MouseWatcher*>(window->get_mouse().node());
  _pickerNode   = new CollisionNode("mouseRay");
  _pickerPath   = _camera.attach_new_node(_pickerNode);
  //_pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());
  _pickerNode->set_from_collide_mask(CollideMask(ColMask::Waypoint | ColMask::DynObject));
  _pickerRay    = new CollisionRay();
  _pickerNode->add_solid(_pickerRay);
  _collisionHandlerQueue = new CollisionHandlerQueue();
  _collisionTraverser.add_collider(_pickerPath, _collisionHandlerQueue);

  EventHandler* events = EventHandler::get_global_event_handler();

  if (!(events->add_hook("mouse1", &Mouse::CallbackButton1, (void*)this)))
    cerr << "Failed to registrate mouse1 event" << endl;
  if (!(events->add_hook("mouse2", &Mouse::CallbackButton2, (void*)this)))
    cerr << "Failed to registrate mouse2 event" << endl;
  if (!(events->add_hook("mouse3", &Mouse::CallbackButton3, (void*)this)))
    cerr << "Failed to registrate mouse2 event" << endl;
}

void Mouse::Run(void)
{
  if (_mouseWatcher->has_mouse())
  {
    LPoint2f cursorPos = _mouseWatcher->get_mouse();

    _pickerRay->set_from_lens(_window->get_camera(0), cursorPos.get_x(), cursorPos.get_y());
    _collisionTraverser.traverse(_window->get_render());
    _collisionHandlerQueue->sort_entries();
    _hovering.Reset();
    for (int i = 0 ; i < _collisionHandlerQueue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry = _collisionHandlerQueue->get_entry(i);

      NodePath into          = entry->get_into_node_path();

      switch (into.get_collide_mask().get_word())
      {
	case ColMask::Waypoint:
	  if (!(_hovering.hasWaypoint))
	    _hovering.SetWaypoint(into);
	  break ;
	case ColMask::DynObject:
	  if (!(_hovering.hasDynObject))
	    _hovering.SetDynObject(into);
	  break ;
      }
    }
  }
}
