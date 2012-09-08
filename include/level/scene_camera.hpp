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

class InstanceDynamicObject;

class SceneCamera
{
public:
  SceneCamera(WindowFramework* window, NodePath camera);

  void            Run(float elapsedTime);
  void            SetEnabledScroll(bool set);

  void            SwapCameraView(void);

  void            SetLimits(unsigned int minX, unsigned int minY, unsigned int maxX, unsigned maxY)
  {
    _minPosX = minX;
    _minPosY = minY;
    _maxPosX = maxX;
    _maxPosY = maxY;
  }
  
  void            CenterCameraInstant(LPoint3f);
  void            CenterCameraOn(NodePath np);
  void            FollowNodePath(NodePath np);
  void            StopFollowingNodePath(void);
  
  void            CenterOnObject(InstanceDynamicObject*);
  void            FollowObject(InstanceDynamicObject*);
  
private:
  void            RunScroll(float elapsedTime);
  void            RunFollow(float elapsedTime);
  
  WindowFramework* _window;
  GraphicsWindow*  _graphicWindow;
  NodePath         _camera;
  LPoint3f         _cameraPos;
  float            _cameraMovementSpeed;
  bool             _scrollEnabled;

  unsigned char    _currentCameraAngle;
  LPoint3f         _currentHpr;
  LPoint3f         _objectiveHpr;

  unsigned int     _minPosX, _minPosY, _maxPosX, _maxPosY;

  bool             _centeringCamera;
  bool             _followingNodePath;
  NodePath         _toFollow;
  LPoint3f         _currentPos;
  LPoint3f         _objectivePos;
};

#endif