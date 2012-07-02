#include "mouse.hpp"
#include "world.h"
#include "globals.hpp"
#include <panda3d/cardMaker.h>

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
  
  CardMaker cardMaker("cursorCardMaker");
  
  _cursorAction      = TexturePool::load_texture("textures/cursor-action.png");
  _cursorInteraction = TexturePool::load_texture("textures/cursor-interaction.png");
  _cursorTarget      = TexturePool::load_texture("textures/cursor-target.png");

  _cursorAction->set_magfilter(Texture::FT_nearest); 
  _cursorAction->set_minfilter(Texture::FT_nearest);

  _cursor = NodePath(cardMaker.generate());
  _cursor.set_scale(0.1, 1, 0.1);
  _cursor.reparent_to(window->get_render_2d());
  _cursor.set_transparency(TransparencyAttrib::M_alpha);
  SetMouseState('a');
  
  // Hiding the regular cursor
  WindowProperties props = window->get_graphics_window()->get_properties();
  props.set_cursor_hidden(true);
  window->get_graphics_window()->request_properties(props);
}

Mouse::~Mouse()
{
  _pickerPath.remove_node();
  _cursor.remove_node();
}

void Mouse::SetMouseState(char i)
{
  switch (i)
  {
    case 'a':
      _cursor.set_texture(_cursorAction);
      _cursorDecalage.set_x(-0.05);
      _cursorDecalage.set_y(-0.05);
      break ;
    case 'i':
      _cursor.set_texture(_cursorInteraction);
      _cursorDecalage.set_x(0);
      _cursorDecalage.set_y(-0.1);
      break ;
    case 't':
      _cursor.set_texture(_cursorTarget);
      _cursorDecalage.set_x(-0.05);
      _cursorDecalage.set_y(-0.05);
      break ;
  }
  if (_mouseWatcher->has_mouse())
  {
    LPoint2f cursorPos = _mouseWatcher->get_mouse();
    
    _cursor.set_pos(cursorPos.get_x() + _cursorDecalage.get_x(), 0, cursorPos.get_y() + _cursorDecalage.get_y());    
  }
}

void Mouse::Run(void)
{
  if (_mouseWatcher->has_mouse())
  {
    LPoint2f cursorPos   = _mouseWatcher->get_mouse();
    bool     hadWaypoint = _hovering.hasWaypoint;

    if (cursorPos == _lastMousePos)
      return ;

    _cursor.set_pos(cursorPos.get_x() + _cursorDecalage.get_x(), 0, cursorPos.get_y() + _cursorDecalage.get_y());

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
