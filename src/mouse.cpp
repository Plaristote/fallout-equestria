#include "mouse.hpp"
#include <tilemap.hpp>

using namespace std;

Mouse::Mouse(WindowFramework* window) : _window(window)
{
  MouseWatcher::init_type();
  _camera       = window->get_camera_group();
  _mouseWatcher = dynamic_cast<MouseWatcher*>(window->get_mouse().node());
  _pickerNode   = new CollisionNode("mouseRay");
  _pickerPath   = _camera.attach_new_node(_pickerNode);
  //_pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());
  _pickerNode->set_from_collide_mask(CollideMask(MyCollisionMask::Object | MyCollisionMask::Tiles));
  _pickerRay    = new CollisionRay();
  _pickerNode->add_solid(_pickerRay);
  _collisionHandlerQueue = new CollisionHandlerQueue();
  _collisionTraverser.add_collider(_pickerPath, _collisionHandlerQueue);

  _posx = _posy = -1;

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
    for (int i = 1 ; i < _collisionHandlerQueue->get_num_entries() ; ++i)
    {
      CollisionEntry* entry = _collisionHandlerQueue->get_entry(i);

      NodePath testNode      = entry->get_from_node_path();
      NodePath testNode2     = entry->get_into_node_path();
      NodePath oldPickedUnit = _lastPickedUnit;

      if ((_hasPickedUnit = testNode.has_net_tag("character")))
        _lastPickedUnit = testNode.find_net_tag("character");
      else if ((_hasPickedUnit = testNode2.has_net_tag("character")))
        _lastPickedUnit = testNode2.find_net_tag("character");
      if (_hasPickedUnit)
      {
        if (oldPickedUnit.node() != _lastPickedUnit.node())
        {
			cout << "New unit hovered: " << _lastPickedUnit.get_name() << endl;
          UnitHovered.Emit(_lastPickedUnit);
        }
        break ;
      }

      if (testNode2.has_net_tag("tile"))
      {
        string pos_x = testNode2.get_net_tag("pos_x");
        string pos_y = testNode2.get_net_tag("pos_y");

        stringstream stream1, stream2;
        int          iposx, iposy;

        stream1 << pos_x;
        stream1 >> iposx;
        stream2 << pos_y;
        stream2 >> iposy;

        if (_posx != iposx || _posy != iposy)
        {
          pos_x = iposx;
          pos_y = iposy;
          CaseHovered.Emit(iposx, iposy);
        }
      }

      _lastPickedUnit = testNode2;
    }
  }
}
