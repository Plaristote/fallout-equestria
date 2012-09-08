#ifndef  MOUSE_HPP
# define MOUSE_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/mouseWatcher.h>
# include <panda3d/collisionRay.h>
# include <panda3d/collisionHandlerQueue.h>
# include <panda3d/collisionTraverser.h>

# include "observatory.hpp"

struct MouseHovering
{
  void Reset(void)
  { hasWaypoint = hasDynObject = false; }
  
  void SetWaypoint(NodePath np)
  {
    hasWaypoint = true;
    waypoint    = np;
  }
  
  void SetDynObject(NodePath np)
  {
    hasDynObject = true;
    dynObject    = np;
  }
  
  bool     hasWaypoint, hasDynObject;
  NodePath waypoint;
  NodePath dynObject;
};

class World;

class Mouse
{
public:
  Mouse(WindowFramework* window);
  ~Mouse();

  void                      Run(void);
  const MouseHovering&      Hovering(void) const { return (_hovering); }
  
  void                      SetMouseState(char);
  
  void                      ClosestWaypoint(World*);

  Observatory::Signal<void> ButtonLeft;
  Observatory::Signal<void> ButtonMiddle;
  Observatory::Signal<void> ButtonRight;

  static void               CallbackButton1(const Event*, void* ptr)
  {
    reinterpret_cast<Mouse*>(ptr)->ButtonLeft.Emit();
  }

  static void               CallbackButton2(const Event*, void* ptr)
  {
    reinterpret_cast<Mouse*>(ptr)->ButtonMiddle.Emit();
  }

  static void               CallbackButton3(const Event*, void* ptr)
  {
    reinterpret_cast<Mouse*>(ptr)->ButtonRight.Emit();
  }

private:
  WindowFramework*          _window;
  NodePath                  _camera;
  LPoint2f                  _lastMousePos;
  PT(MouseWatcher)          _mouseWatcher;
  PT(CollisionRay)          _pickerRay;
  PT(CollisionNode)         _pickerNode;
  NodePath                  _pickerPath;
  CollisionTraverser        _collisionTraverser;
  PT(CollisionHandlerQueue) _collisionHandlerQueue;
  
  MouseHovering             _hovering;
};

#endif