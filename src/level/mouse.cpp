#include "level/mouse.hpp"
#include "level/world.h"
#include "globals.hpp"
#include <mousecursor.hpp>
#include <timer.hpp>
#include <panda3d/cardMaker.h>
#include <panda3d/collisionPlane.h>
#include <boost/concept_check.hpp>

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
  _pickerNode->set_from_collide_mask(CollideMask(/*ColMask::Waypoint | */ColMask::DynObject));
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

void Mouse::ClosestWaypoint(World* world, short currentFloor)
{
  if (_mouseWatcher->has_mouse())
  {
    PStatCollector collector("Level:Mouse:FindWaypoint"); collector.start();
    PT(CollisionRay)          pickerRay;
    PT(CollisionNode)         pickerNode;
    NodePath                  pickerPath;
    CollisionTraverser        collisionTraverser;
    PT(CollisionHandlerQueue) collisionHandlerQueue = new CollisionHandlerQueue();
    LPoint2f                  cursorPos             = _mouseWatcher->get_mouse();
    static bool               updated               = false;
    static LPoint2f           last_update;

    //std::cout << "Difference -> " << ABS(cursorPos.get_x() - last_update.get_x()) << std::endl;
    if (ABS(cursorPos.get_x() - last_update.get_x()) > 0.05 || ABS(cursorPos.get_y() - last_update.get_y()) > 0.05)
      updated         = false;
//    if (cursorPos != _lastMousePos)
  //    updated         = false;
    if (!(updated == false))
      return ;
    last_update       = cursorPos;
    updated           = true;
    pickerNode        = new CollisionNode("mouseRay2");
    pickerPath        = _camera.attach_new_node(pickerNode);
    pickerRay         = new CollisionRay();
    pickerNode->set_from_collide_mask(CollideMask(ColMask::Object));
    pickerNode->set_into_collide_mask(0);
    pickerRay->set_from_lens(_window->get_camera(0), cursorPos.get_x(), cursorPos.get_y());
    pickerNode->add_solid(pickerRay);

    collisionTraverser.add_collider(pickerPath, collisionHandlerQueue);
    collisionTraverser.traverse(_window->get_render());

    collisionHandlerQueue->sort_entries();

    Timer timer;
    
    _hovering.hasWaypoint = false;
    for (int i = 0 ; i < collisionHandlerQueue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry      = collisionHandlerQueue->get_entry(i);
      NodePath        np         = entry->get_into_node_path();
      MapObject*      map_object = world->GetMapObjectFromNodePath(np);
      LPoint3         pos;

      if (!map_object)
        continue ;
      pos = entry->get_surface_point(world->floors[map_object->floor]);
      Timer timer2;
      _hovering.waypoint_ptr = world->waypoint_graph.GetClosest(pos);
      //timer2.Profile("Assessing closest waypoint");
      //_hovering.waypoint_ptr = world->GetWaypointClosest(pos, map_object->floor);
      if (_hovering.waypoint_ptr)
        _hovering.SetWaypoint(_hovering.waypoint_ptr->nodePath);
      break ;
    }
    
    //timer.Profile("Waypoint Picking");

    //pickerPath.detach_node(); // TODO find out why hasDynObject stops working after this...
                                //      this leak has to go away
    collector.stop();
  }
}

void Mouse::Run(void)
{
  PStatCollector collector("Level:Mouse:Run");

  collector.start();
  if (_mouseWatcher->has_mouse())
  {
    LPoint2f cursorPos   = _mouseWatcher->get_mouse();

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

        NodePath into          = entry->get_into_node_path();

        switch (into.get_collide_mask().get_word())
        {
          case ColMask::DynObject:
            if (!(_hovering.hasDynObject))
            {
              _hovering.SetDynObject(into);
            }
            break ;
        }
      }
    }
  }
  collector.stop();
}
