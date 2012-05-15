#ifndef  MOUSE_HPP
# define MOUSE_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/mouseWatcher.h>
# include <panda3d/collisionRay.h>
# include <panda3d/collisionHandlerQueue.h>
# include <panda3d/collisionTraverser.h>

# include "observatory.hpp"

class Mouse
{
public:
  Mouse(WindowFramework* window);

  void                      Run(void);

  Observatory::Signal<>             ButtonLeft;
  Observatory::Signal<>             ButtonRight;
  Observatory::Signal<NodePath>     UnitHovered;
  Observatory::Signal<int, int>     CaseHovered;

  static void               CallbackButton1(const Event*, void* ptr)
  {
    reinterpret_cast<Mouse*>(ptr)->ButtonLeft.Emit();
  }

  static void               CallbackButton2(const Event*, void* ptr)
  {
    reinterpret_cast<Mouse*>(ptr)->ButtonRight.Emit();
  }

private:
  WindowFramework*          _window;
  NodePath                  _camera;
  PT(MouseWatcher)          _mouseWatcher;
  PT(CollisionRay)          _pickerRay;
  PT(CollisionNode)         _pickerNode;
  NodePath                  _pickerPath;
  CollisionTraverser        _collisionTraverser;
  PT(CollisionHandlerQueue) _collisionHandlerQueue;
  bool                      _hasPickedUnit;
  NodePath                  _lastPickedUnit;

  int                       _posx, _posy;
};

#endif