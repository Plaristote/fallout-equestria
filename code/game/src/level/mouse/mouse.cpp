#include "level/mouse/mouse.hpp"
#include "world/world.h"
#include <mousecursor.hpp>
#include <timer.hpp>
#include <panda3d/cardMaker.h>
#include <panda3d/collisionPlane.h>

using namespace std;

Mouse::Mouse(WindowFramework* window) : _window(window)
{
  cout << "- Initializing mouse..." << endl;
  MouseWatcher::init_type();
  _lastMousePos.set_x(0);
  _lastMousePos.set_y(0);
  _camera                = window->get_camera_group();
  _hovering.Reset();
  _mouseWatcher          = dynamic_cast<MouseWatcher*>(window->get_mouse().node());
  _pickerNode            = new CollisionNode("mouseRay");
  _pickerPath            = _camera.attach_new_node(_pickerNode);
  _pickerNode->set_from_collide_mask(CollideMask(/*ColMask::Waypoint | */ColMask::DynObject));
  _pickerNode->set_into_collide_mask(0);
  _pickerRay             = new CollisionRay();
  _pickerNode->add_solid(_pickerRay);
  _collisionHandlerQueue = new CollisionHandlerQueue();
  _collisionTraverser.add_collider(_pickerPath, _collisionHandlerQueue);

  EventHandler* events = EventHandler::get_global_event_handler();

  if (!(events->add_hook("mouse1", &Mouse::CallbackButton1, (void*)this)))
    cerr << "Failed to registrate mouse1 event" << endl;
  if (!(events->add_hook("mouse2", &Mouse::CallbackButton2, (void*)this)))
    cerr << "Failed to registrate mouse2 event" << endl;
  if (!(events->add_hook("mouse3", &Mouse::CallbackButton3, (void*)this)))
    cerr << "Failed to registrate mouse3 event" << endl;
  if (!(events->add_hook("wheel_up", &Mouse::CallbackWheelUp, (void*)this)))
    cerr << "Failed to register wheel_up event" << endl;
  if (!(events->add_hook("wheel_down", &Mouse::CallbackWheelDown, (void*)this)))
    cerr << "Failed to register wheel_down event" << endl;

  SetMouseState('a');
  cout << "-> Done." << endl;
}

Mouse::~Mouse()
{
  _pickerPath.remove_node();
  
  EventHandler* events = EventHandler::get_global_event_handler();
  
  
  events->remove_hook("mouse1",     &Mouse::CallbackButton1,   (void*)this);
  events->remove_hook("mouse2",     &Mouse::CallbackButton2,   (void*)this);
  events->remove_hook("mouse3",     &Mouse::CallbackButton3,   (void*)this);
  events->remove_hook("wheel_up",   &Mouse::CallbackWheelUp,   (void*)this);
  events->remove_hook("wheel_down", &Mouse::CallbackWheelDown, (void*)this);
  
  MouseCursor::Get()->SetCursorType("cursor-interaction");
}

void Mouse::SetMouseState(char i)
{
  std::string type;
  
  switch (i)
  {
    case 'a':
      type = "cursor-action";
      break ;
    case 'i':
      type = "cursor-interaction-none";
      break ;
    case 't':
      type = "cursor-target";
      break ;
    default:
      type = "cursor-interaction";
      break ;
  }
  if (MouseCursor::Get())
    MouseCursor::Get()->SetCursorType(type);
}

std::string NodePathFullName(NodePath nodepath, NodePath root)
{
  NodePath    cur  = nodepath.get_parent();
  std::string name = nodepath.get_name();

  while (cur != root)
  {
    name = cur.get_name() + '/' + name;
    cur = cur.get_parent();
  }
  return (name);
}

LPoint2f Mouse::GetPosition(void) const
{
  if (_window->get_graphics_window())
  {
    MouseData  pointer = _window->get_graphics_window()->get_pointer(0);
    LPoint2f   cursorPos(pointer.get_x(), pointer.get_y());

    return (cursorPos);
  }
  return (LPoint2f(0.f, 0.f));
}

void Mouse::Move(float x, float y)
{
  if (_window->get_graphics_window())
  {
    LPoint2f position = GetPosition();

    position.set_x(position.get_x() + x);
    position.set_y(position.get_y() + y);
    _window->get_graphics_window()->move_pointer(0, x, y);
  }
}

LPoint2f Mouse::GetPositionRatio(void) const
{
  LPoint2f position = GetPosition();
  
  if (_window->get_graphics_window())
  {
    int      size_x   = _window->get_graphics_window()->get_x_size();
    int      size_y   = _window->get_graphics_window()->get_y_size();

    position.set_x( ((position.get_x() / size_x) - 0.5) * 2);
    position.set_y(-(((position.get_y() / size_y) - 0.5) * 2));
  }
  return (position);
}

void Mouse::ClosestWaypoint(World* world, short current_floor)
{
  //if (_mouseWatcher->has_mouse())
  {
    Timer test;

    PStatCollector collector("Level:Mouse:FindWaypoint"); collector.start();
    PT(CollisionRay)          pickerRay;
    PT(CollisionNode)         pickerNode;
    NodePath                  pickerPath;
    CollisionTraverser        collisionTraverser;
    PT(CollisionHandlerQueue) collisionHandlerQueue = new CollisionHandlerQueue();
    LPoint2f                  cursorPos             = GetPositionRatio();
    static bool               updated               = false;
    static LPoint2f           last_update;

    if (ABS(cursorPos.get_x() - last_update.get_x()) > 0.05 || ABS(cursorPos.get_y() - last_update.get_y()) > 0.05)
      updated         = false;
    if (!(updated == false))
      return ;
    last_update       = cursorPos;
    updated           = true;
    pickerNode        = new CollisionNode("mouseRay2");
    pickerPath        = _camera.attach_new_node(pickerNode);
    pickerRay         = new CollisionRay();
    pickerNode->set_from_collide_mask(CollideMask(ColMask::WpPlane));
    pickerNode->set_into_collide_mask(0);
    pickerRay->set_from_lens(_window->get_camera(0), cursorPos.get_x(), cursorPos.get_y());
    pickerNode->add_solid(pickerRay);

    collisionTraverser.add_collider(pickerPath, collisionHandlerQueue);
    collisionTraverser.traverse(_window->get_render());
    //collisionTraverser.traverse(world->floors[current_floor]);

    collisionHandlerQueue->sort_entries();

    if (_hovering.waypoint_ptr && _hovering.hasWaypoint)
      _hovering.waypoint_ptr->SetSelected(false);
    _hovering.hasWaypoint  = false;
    _hovering.waypoint_ptr = 0;
    for (int i = 0 ; i < collisionHandlerQueue->get_num_entries() ; ++i)
    {
      CollisionTraverser        model_traverser;
      PT(CollisionHandlerQueue) handler_queue = new CollisionHandlerQueue();

      CollisionEntry* entry      = collisionHandlerQueue->get_entry(i);
      NodePath        np         = entry->get_into_node_path();
      MapObject*      map_object = world->GetMapObjectFromNodePath(np);
      LPoint3         pos;
      static LPoint3  spheresize = NodePathSize(world->model_sphere);

      if (!map_object || map_object->nodePath.is_hidden())
        continue ;
      if (map_object->collider == MapObject::MODEL)
      {
        CollideMask initial_collide_mask = map_object->render.get_collide_mask();

        map_object->render.set_collide_mask(initial_collide_mask | CollideMask(ColMask::WpPlane));
        model_traverser.add_collider(pickerPath, handler_queue);
        model_traverser.traverse(map_object->render);
        map_object->render.set_collide_mask(initial_collide_mask);
        if (handler_queue->get_num_entries() == 0)
          continue ;
        entry = handler_queue->get_entry(0);
      }
      pos = entry->get_surface_point(world->window->get_render()) - spheresize;
      _hovering.waypoint_ptr = world->waypoint_graph.GetClosest(pos);
      if (_hovering.waypoint_ptr)
      {
        _hovering.SetWaypoint(_hovering.waypoint_ptr->nodePath);
        _hovering.waypoint_ptr->SetSelected(true);
      }
      break ;
    }

    // Detaching seems to be causing some memory issues.
    //pickerPath.detach_node();
    test.Profile("Closest waypoint");
    collector.stop();
  }
}

void Mouse::Run(void)
{
  PStatCollector collector("Level:Mouse:Run");

  collector.start();
  //if (pointer.get_in_window())
  {
      LPoint2f cursorPos = GetPositionRatio();

    if (cursorPos != _lastMousePos)
    {
      _lastMousePos = cursorPos;
      _pickerRay->set_from_lens(_window->get_camera(0), cursorPos.get_x(), cursorPos.get_y());
      _collisionTraverser.traverse(_window->get_render());
      _collisionHandlerQueue->sort_entries();
      //_hovering.Reset();
      _hovering.hasDynObject = false;
      _hovering.dynObject    = NodePath();
      for (int i = 0 ; i < _collisionHandlerQueue->get_num_entries() ; ++i)
      {
        CollisionEntry* entry = _collisionHandlerQueue->get_entry(i);
        NodePath        into  = entry->get_into_node_path();

        if (into.is_hidden())
          continue ;
        switch (into.get_collide_mask().get_word())
        {
          case ColMask::DynObject:
          if (!(_hovering.hasDynObject))
            _hovering.SetDynObject(into);
          break ;
        }
      }
    }
  }
  collector.stop();
}
