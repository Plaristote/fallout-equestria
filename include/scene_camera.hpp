#ifndef  SCENE_CAMERA_HPP
# define SCENE_CAMERA_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/mouseWatcher.h>
# include <panda3d/collisionRay.h>
# include <panda3d/collisionHandlerQueue.h>
# include <panda3d/collisionTraverser.h>

# include "observatory.hpp"

enum
{
  MotionNone   = 0,
  MotionTop    = 1,
  MotionBottom = 2,
  MotionLeft   = 4,
  MotionRight  = 8
};

class SceneCamera
{
public:
  SceneCamera(WindowFramework* window, NodePath camera) : _window(window), _graphicWindow(window->get_graphics_window()), _camera(camera)
  {
    _scrollEnabled = true;
  }

  void            Run(float elapsedTime);
  void            SetEnabledScroll(bool set) { _scrollEnabled = set; }

private:
  void            RunScroll(float elapsedTime);
  
  WindowFramework* _window;
  GraphicsWindow*  _graphicWindow;
  NodePath         _camera;
  bool             _scrollEnabled;
};

#endif