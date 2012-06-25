#include "globals.hpp"
#include "scene_camera.hpp"
#include <objectnode.hpp>

using namespace std;

# define CAMERA_HEIGHT 75

//HAIL MICROSOFT
LPoint3f cameraAngles[] = {
  LPoint3f(-40, -40, 0),
  LPoint3f(0, -60, 0),
/*  LPoint3f(40, -40, 0),
  LPoint3f(40 - 180, -40, 0),
  LPoint3f(-180, -60, 0),
  LPoint3f(-40 - 180, -40, 0),*/
};
bool scrollSwapAngles[] = {
  true,
  false,
/*  false,
  true,
  false,
  false,*/
};
bool scrollSwapAxis[] = {
  false,
  false,
/*  false,
  true,
  true,
  true,*/
};


SceneCamera::SceneCamera(WindowFramework* window, NodePath camera) : _window(window), _graphicWindow(window->get_graphics_window()), _camera(camera)
{
  _scrollEnabled = true;

  camera.set_pos(0, 0, CAMERA_HEIGHT);
  camera.set_hpr(-40, -40, 0);

  _cameraMovementSpeed = 30.f;
  _currentCameraAngle = 0;
  _currentHpr         = cameraAngles[_currentCameraAngle];
  _objectiveHpr       = _currentHpr;

  _minPosX = _minPosY = _maxPosX = _maxPosY = 0;
  
  _followingNodePath = false;
  _centeringCamera   = false;
}

void SceneCamera::SwapCameraView(void)
{
  if (++_currentCameraAngle >= GET_ARRAY_SIZE(cameraAngles))
    _currentCameraAngle = 0;
  _objectiveHpr = cameraAngles[_currentCameraAngle];

  _centeringCamera = true;
  if (_currentCameraAngle == 0)
  {
    _objectivePos.set_y(_camera.get_y() + 40 - 65);
    _objectivePos.set_x(_camera.get_x() + 0  - 60);
    _objectivePos.set_z(_camera.get_z());
  }
  else if (_currentCameraAngle == 1)
  {
    _objectivePos.set_y(_camera.get_y() + 65 - 40);
    _objectivePos.set_x(_camera.get_x() + 60 - 0);
    _objectivePos.set_z(_camera.get_z());
  }
}

void SceneCamera::Run(float elapsedTime)
{
  if (_scrollEnabled)
    RunScroll(elapsedTime);
  if (_currentHpr != _objectiveHpr)
  {

    if      (_objectiveHpr.get_x() > _currentHpr.get_x())
      _currentHpr.set_x(_currentHpr.get_x() + 1);
    else if (_objectiveHpr.get_x() < _currentHpr.get_x())
      _currentHpr.set_x(_currentHpr.get_x() - 1);

    if      (_objectiveHpr.get_y() > _currentHpr.get_y())
      _currentHpr.set_y(_currentHpr.get_y() + 1);
    else if (_objectiveHpr.get_y() < _currentHpr.get_y())
      _currentHpr.set_y(_currentHpr.get_y() - 1);

    if      (_objectiveHpr.get_z() > _currentHpr.get_z())
      _currentHpr.set_z(_currentHpr.get_z() + 1);
    else if (_objectiveHpr.get_z() < _currentHpr.get_z())
      _currentHpr.set_z(_currentHpr.get_z() - 1);

    _camera.set_hpr(_currentHpr);
  }
  if (_centeringCamera || _followingNodePath)
    RunFollow(elapsedTime);
}

void SceneCamera::RunScroll(float elapsedTime)
{
  MouseData     pointer      = _graphicWindow->get_pointer(0);
  unsigned char cameraMotion = MotionNone;
  LPoint3       cameraPos    = _camera.get_pos();

  float _mouseBorderMargin   = 40;
  float cameraSpeed          = _cameraMovementSpeed;
  float cameraMovement       = _cameraMovementSpeed * elapsedTime;

  if (pointer.get_y() <= _mouseBorderMargin)
  {
    cameraMotion |= MotionTop;
    cameraSpeed   = cameraSpeed * ((_mouseBorderMargin - pointer.get_y()) / _mouseBorderMargin);
  }
  else if (pointer.get_y() >= _graphicWindow->get_y_size() - _mouseBorderMargin)
  {
    cameraMotion |= MotionBottom;
    cameraSpeed   = cameraSpeed * ((pointer.get_y() - (_graphicWindow->get_y_size() - _mouseBorderMargin)) / _mouseBorderMargin);
  }
  if (pointer.get_x() <= _mouseBorderMargin)
  {
    cameraMotion |= MotionLeft;
    cameraSpeed   = cameraSpeed * ((_mouseBorderMargin - pointer.get_x()) / _mouseBorderMargin);
  }
  else if (pointer.get_x() >= _graphicWindow->get_x_size() - _mouseBorderMargin)
  {
    cameraMotion |= MotionRight;
    cameraSpeed   = cameraSpeed * ((pointer.get_x() - (_graphicWindow->get_x_size() - _mouseBorderMargin)) / _mouseBorderMargin);
  }
  
  cameraMovement = cameraSpeed * elapsedTime;

  unsigned char motionTop    = MotionTop;
  unsigned char motionBottom = MotionBottom;
  unsigned char motionLeft   = MotionLeft;
  unsigned char motionRight  = MotionRight;

  LPoint3f movement;
  LPoint3f cameraRot = _camera.get_hpr();
  float    rad2deg   = 3.1415926535897 / 180;

  cameraRot.set_x(cameraRot.get_x() * rad2deg);
  cameraRot.set_y(cameraRot.get_y() * rad2deg);
  cameraRot.set_z(cameraRot.get_z() * rad2deg);

  if (cameraMotion & MotionTop)
    movement.set_y(cameraMovement);
  else if (cameraMotion & MotionBottom)
    movement.set_y(-cameraMovement);
  if (cameraMotion & MotionLeft)
    movement.set_x(-cameraMovement);
  else if (cameraMotion & MotionRight)
    movement.set_x(cameraMovement);

  if (cameraMotion != MotionNone)
    _cameraPos += movement;

  float zoom = 1.f;

  _camera.set_pos(_cameraPos.get_x() - cos(cameraRot.get_x()) * zoom,
                  _cameraPos.get_y() - sin(cameraRot.get_y()) * zoom,
                  _cameraPos.get_z() + sin(cameraRot.get_z()) * zoom + CAMERA_HEIGHT);
  
  /*if (scrollSwapAxis[_currentCameraAngle])
  {
    motionTop    = MotionBottom;
    motionBottom = MotionTop;
    motionLeft   = MotionRight;
    motionRight  = MotionLeft;
  }

  if (scrollSwapAngles[_currentCameraAngle])
  {
    if (cameraMotion & motionTop && cameraMotion & motionRight)
    {
      cameraPos.set_x(cameraPos.get_x() + cameraMovement);
    }
    else if (cameraMotion & motionTop && cameraMotion & motionLeft)
    {
      cameraPos.set_y(cameraPos.get_y() + cameraMovement);
    }
    else if (cameraMotion & motionTop)
    {
      cameraPos.set_y(cameraPos.get_y() + cameraMovement);
      cameraPos.set_x(cameraPos.get_x() + cameraMovement);
    }
    else if (cameraMotion & motionBottom && cameraMotion & motionRight)
    {
      cameraPos.set_y(cameraPos.get_y() - cameraMovement);
    }
    else if (cameraMotion & motionRight)
    {
      cameraPos.set_y(cameraPos.get_y() - cameraMovement);
      cameraPos.set_x(cameraPos.get_x() + cameraMovement);
    }
    else if (cameraMotion & motionBottom && cameraMotion & motionLeft)
    {
      cameraPos.set_x(cameraPos.get_x() - cameraMovement);
    }
    else if (cameraMotion & motionBottom)
    {
      cameraPos.set_y(cameraPos.get_y() - cameraMovement);
      cameraPos.set_x(cameraPos.get_x() - cameraMovement);
    }
    else if (cameraMotion & motionLeft)
    {
      cameraPos.set_y(cameraPos.get_y() + cameraMovement);
      cameraPos.set_x(cameraPos.get_x() - cameraMovement);
    }
  }
  else
  {
    if      (cameraMotion & motionTop)
      cameraPos.set_y(cameraPos.get_y() + cameraMovement);
    else if (cameraMotion & motionBottom)
      cameraPos.set_y(cameraPos.get_y() - cameraMovement);
    if (cameraMotion & motionLeft)
      cameraPos.set_x(cameraPos.get_x() - cameraMovement);
    else if (cameraMotion & motionRight)
      cameraPos.set_x(cameraPos.get_x() + cameraMovement);
  }

  if      (cameraPos.get_x() < _minPosX && _minPosX != 0)
    cameraPos.set_x(_minPosX);
  else if (cameraPos.get_x() > _maxPosX && _minPosX != 0)
    cameraPos.set_x(_maxPosX);
  if      (cameraPos.get_y() < _minPosY && _minPosY != 0)
    cameraPos.set_y(_minPosY);
  else if (cameraPos.get_y() > _minPosY && _minPosY != 0)
    cameraPos.set_y(_maxPosY);*/

  //_camera.set_pos(cameraPos);
}

/*
 * Camera/NodePath centering and following stuff
 */
void SceneCamera::CenterCameraInstant(LPoint3f pos)
{
  _cameraPos = pos;
  /*if (_currentCameraAngle == 0)
  {
    _camera.set_y(pos.get_y() - 65);
    _camera.set_x(pos.get_x() - 60);
  }
  else if (_currentCameraAngle == 1)
  {
    _camera.set_y(pos.get_y() - 40);
    _camera.set_x(pos.get_x());
  }*/
}

void SceneCamera::FollowObject(InstanceDynamicObject* object)
{
  FollowNodePath(object->GetNodePath());
}

void SceneCamera::CenterOnObject(InstanceDynamicObject* object)
{
  CenterCameraOn(object->GetNodePath());
}

void SceneCamera::CenterCameraOn(NodePath np)
{
  _centeringCamera   = true;
  _objectivePos      = np.get_pos();
  _objectivePos.set_z(_objectivePos.get_z() + CAMERA_HEIGHT);
  
  // take in account camera decalage
  /*if (_currentCameraAngle == 0)
  {
    _objectivePos.set_y(_objectivePos.get_y() - 65);
    _objectivePos.set_x(_objectivePos.get_x() - 60);
  }
  else if (_currentCameraAngle == 1)
  {
    _objectivePos.set_y(_objectivePos.get_y() - 40);
    _objectivePos.set_x(_objectivePos.get_x());
  }*/
}

void SceneCamera::FollowNodePath(NodePath np)
{
  _followingNodePath = true;
  _toFollow          = np;
}

void SceneCamera::StopFollowingNodePath(void)
{
  _followingNodePath = false;
  _centeringCamera   = false;
}

void SceneCamera::RunFollow(float elapsedTime)
{
  if (_followingNodePath)
    CenterCameraOn(_toFollow);
  LPoint3f camVec   = _objectivePos - _camera.get_pos();
  LPoint3f camSpeed;

  camSpeed = camVec / 2;
  if (camSpeed.get_x() > 0 && camSpeed.get_x() < _cameraMovementSpeed)
    camSpeed.set_x(_cameraMovementSpeed);
  else if (camSpeed.get_x() < 0 && camSpeed.get_x() > -_cameraMovementSpeed)
    camSpeed.set_x(-_cameraMovementSpeed);
  
  if (camSpeed.get_y() > 0 && camSpeed.get_y() < _cameraMovementSpeed)
    camSpeed.set_y(_cameraMovementSpeed);
  else if (camSpeed.get_y() < 0 && camSpeed.get_y() > -_cameraMovementSpeed)
    camSpeed.set_y(-_cameraMovementSpeed);

  if (camSpeed.get_z() > 0 && camSpeed.get_z() < _cameraMovementSpeed)
    camSpeed.set_z(_cameraMovementSpeed);
  else if (camSpeed.get_z() < 0 && camSpeed.get_z() > -_cameraMovementSpeed)
    camSpeed.set_z(-_cameraMovementSpeed);
  
  camSpeed = camSpeed * elapsedTime;

  LPoint3f dest = _camera.get_pos() + (camSpeed);
  LPoint3f dist = _objectivePos - dest;
  
  if (ABS(dist.get_x()) < ABS(camSpeed.get_x()))
    dest.set_x(_objectivePos.get_x());
  if (ABS(dist.get_y()) < ABS(camSpeed.get_y()))
    dest.set_y(_objectivePos.get_y());
  if (ABS(dist.get_z()) < ABS(camSpeed.get_z()))
    dest.set_z(_objectivePos.get_z());
  _camera.set_pos(dest);
  if (dest == _objectivePos)
  {
    _centeringCamera = false;
    std::cout << "Centering camera Done" << std::endl;
  }
}


























