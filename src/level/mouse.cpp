#include "level/mouse.hpp"
#include "level/world.h"
#include "globals.hpp"
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
  _pickerNode            = new_CollisionNode("mouseRay");
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
    default:
      break ;
  }
  if (MouseCursor::Get())
    MouseCursor::Get()->SetCursorTexture(texture);
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
  MouseData  pointer = _window->get_graphics_window()->get_pointer(0);
  LPoint2f   cursorPos(pointer.get_x(), pointer.get_y());

  return (cursorPos);
}

LPoint2f Mouse::GetPositionRatio(void) const
{
  int      size_x   = _window->get_graphics_window()->get_x_size();
  int      size_y   = _window->get_graphics_window()->get_y_size();
  LPoint2f position = GetPosition();

  position.set_x( ((position.get_x() / size_x) - 0.5) * 2);
  position.set_y(-(((position.get_y() / size_y) - 0.5) * 2));
  return (position);
}

void Mouse::ClosestWaypoint(World* world, short currentFloor)
{
  //if (_mouseWatcher->has_mouse())
  {
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
    cout << "MousePos: " << cursorPos.get_x() << ", " << cursorPos.get_y() << endl;
    last_update       = cursorPos;
    updated           = true;
    pickerNode        = new_CollisionNode("mouseRay2");
    pickerPath        = _camera.attach_new_node(pickerNode);
    pickerRay         = new CollisionRay();
    pickerNode->set_from_collide_mask(CollideMask(ColMask::WpPlane));
    pickerNode->set_into_collide_mask(0);
    pickerRay->set_from_lens(_window->get_camera(0), cursorPos.get_x(), cursorPos.get_y());
    pickerNode->add_solid(pickerRay);

    collisionTraverser.add_collider(pickerPath, collisionHandlerQueue);
    collisionTraverser.traverse(_window->get_render());

    collisionHandlerQueue->sort_entries();

    if (_hovering.waypoint_ptr && _hovering.hasWaypoint)
      _hovering.waypoint_ptr->SetSelected(false);
    _hovering.hasWaypoint = false;
    for (int i = 0 ; i < collisionHandlerQueue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry      = collisionHandlerQueue->get_entry(i);
      NodePath        np         = entry->get_into_node_path();
      MapObject*      map_object = world->GetMapObjectFromNodePath(np);
      LPoint3         pos;
      static LPoint3  spheresize = NodePathSize(World::model_sphere);

      if (!map_object)
        continue ;
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
