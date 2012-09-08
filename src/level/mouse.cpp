#include "level/mouse.hpp"
#include "level/world.h"
#include "globals.hpp"
#include <mousecursor.hpp>
#include <panda3d/cardMaker.h>
#include <panda3d/collisionPlane.h>

using namespace std;

Mouse::Mouse(WindowFramework* window) : _window(window)
{
  MouseWatcher::init_type();
  _lastMousePos.set_x(0);
  _lastMousePos.set_y(0);
  _camera       = window->get_camera_group();
  _hovering.Reset();
  _mouseWatcher = dynamic_cast<MouseWatcher*>(window->get_mouse().node());
  _pickerNode   = new CollisionNode("mouseRay");
  _pickerPath   = _camera.attach_new_node(_pickerNode);
  _pickerNode->set_from_collide_mask(CollideMask(ColMask::Waypoint | ColMask::DynObject));
  _pickerNode->set_into_collide_mask(0);
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

  SetMouseState('a');
}

Mouse::~Mouse()
{
  _pickerPath.remove_node();
  
  EventHandler* events = EventHandler::get_global_event_handler();
  
  events->remove_hook("mouse1", &Mouse::CallbackButton1, (void*)this);
  events->remove_hook("mouse2", &Mouse::CallbackButton2, (void*)this);
  events->remove_hook("mouse3", &Mouse::CallbackButton3, (void*)this);
  
  MouseCursor::Get()->SetCursorTexture("textures/cursor-interaction.png");
}

void Mouse::SetMouseState(char i)
{
  std::string texture;
  
  switch (i)
  {
    case 'a':
      texture = "textures/cursor-action.png";
      break ;
    case 'i':
      texture = "textures/cursor-interaction.png";
      break ;
    case 't':
      texture = "textures/cursor-target.png";
      break ;
  }
  if (MouseCursor::Get())
    MouseCursor::Get()->SetCursorTexture(texture);
}

void Mouse::ClosestWaypoint(World* world)
{
  PT(CollisionPlane)        pickerPlane;
  PT(CollisionRay)          pickerRay;
  PT(CollisionNode)         planeNode;
  PT(CollisionNode)         pickerNode;
  NodePath                  planePath;
  NodePath                  pickerPath;
  CollisionTraverser        collisionTraverser;
  PT(CollisionHandlerQueue) collisionHandlerQueue = new CollisionHandlerQueue();
  
  pickerNode   = new CollisionNode("mouseRay2");
  pickerPath   = _camera.attach_new_node(_pickerNode);
  //pickerNode->set_from_collide_mask(CollideMask(ColMask::WpPlane));
  pickerNode->set_into_collide_mask(0);
  pickerRay    = new CollisionRay();
  pickerNode->add_solid(pickerRay);

  LPlane plane = world->GetWaypointPlane();

  pickerPlane = new CollisionPlane(plane);
  planeNode = new CollisionNode("pickerPlane");
  //planeNode->set_into_collide_mask(CollideMask(ColMask::WpPlane));
  planeNode->add_solid(pickerPlane);
  planePath = _window->get_render().attach_new_node(planeNode);
  
  collisionTraverser.add_collider(pickerPath, collisionHandlerQueue);
  collisionTraverser.traverse(_window->get_render());
  
  collisionHandlerQueue->sort_entries();

  _hovering.Reset();
  for (int i = 0 ; i < collisionHandlerQueue->get_num_entries() ; ++i)
  {
    CollisionEntry* entry = collisionHandlerQueue->get_entry(i);
    NodePath        np    = entry->get_into_node_path();
    LPoint3         pos   = entry->get_surface_point(np);

    if (!(planePath.is_ancestor_of(np.node())) && planePath != np)
      continue ;
    pos.set_x(pos.get_x() + np.get_x());
    pos.set_y(pos.get_y() + np.get_y());
    pos.set_z(pos.get_z() + np.get_z());
    std::cout << "Position: (" << pos.get_x() << ", " << pos.get_y() << ", " << pos.get_z() << ")\n";
    NodePath tmp = world->GetWaypointClosest(pos)->nodePath;
    std::cout << tmp.get_x() << ", " << tmp.get_y() << std::endl;

    _hovering.SetWaypoint(tmp);
    break ;
  }

  planePath.detach_node();
}

void Mouse::Run(void)
{
  if (_mouseWatcher->has_mouse())
  {
    LPoint2f cursorPos   = _mouseWatcher->get_mouse();
    bool     hadWaypoint = _hovering.hasWaypoint;

    if (cursorPos == _lastMousePos)
      return ;

    _lastMousePos = cursorPos;
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
    if (!_hovering.hasWaypoint && hadWaypoint)
    {
      LPoint2f dist = _hovering.waypoint.get_pos().get_xy() - cursorPos;

      if (ABS(dist.get_x()) < 0.5f && ABS(dist.get_y()) < 0.5f)
	_hovering.hasWaypoint = true;
    }
  }
}
