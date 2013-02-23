#include "globals.hpp"
#include "level/scene_camera.hpp"
#include "level/objectnode.hpp"

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


void SceneCamera::SetEnabledScroll(bool set)
{
  _scrollEnabled = set;
}

void SceneCamera::SlideToHeight(float height)
{
  _destHeight = height + CAMERA_HEIGHT;
}

void SceneCamera::RunSlideHeight(float elapsedTime)
{
  LPoint3 pos    = _camera.get_pos();
  float   posZ   = pos.get_z();
  float   toMove = (posZ - _destHeight) * elapsedTime;

  pos.set_z(posZ + -toMove);
  _camera.set_pos(pos);
}

SceneCamera::SceneCamera(WindowFramework* window, NodePath camera) : _window(window), _graphicWindow(window->get_graphics_window()), _camera(camera)
{
  _scrollEnabled = true;

  _destHeight = CAMERA_HEIGHT;
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
  
  _objectivePos.set_z(_camera.get_z());

//   if (_currentCameraAngle == 0)
//   {
//     _objectivePos.set_y(_camera.get_y() + 40 - 65);
//     _objectivePos.set_x(_camera.get_x() + 0  - 60);
//     _objectivePos.set_z(_camera.get_z());
//   }
//   else if (_currentCameraAngle == 1)
//   {
//     _objectivePos.set_y(_camera.get_y() + 65 - 40);
//     _objectivePos.set_x(_camera.get_x() + 60 - 0);
//     _objectivePos.set_z(_camera.get_z());
//   }
}

void SceneCamera::Run(float elapsedTime)
{
  PStatCollector collector("Level:SceneCamera");

  collector.set_level(2);
  collector.start();  
  if (_scrollEnabled)
    RunScroll(elapsedTime);
  if (_destHeight != _camera.get_z())
    RunSlideHeight(elapsedTime);
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
  collector.stop();
}

void SceneCamera::RunScroll(float elapsedTime)
{
  MouseData     pointer      = _graphicWindow->get_pointer(0);
  unsigned char cameraMotion = MotionNone;
  LPoint3       cameraPos    = _camera.get_pos();

  float    _mouseBorderMargin   = 40;
  float    cameraSpeedX         = _cameraMovementSpeed;
  float    cameraSpeedY         = _cameraMovementSpeed;
  float    cameraMovementX;
  float    cameraMovementY;
  LPoint3f movement;
  LPoint3f cameraRot = _camera.get_hpr();
  float    rad2deg   = 3.1415926535897 / 180;

  if (pointer.get_y() <= _mouseBorderMargin)
  {
    cameraMotion |= MotionTop;
    cameraSpeedY  = cameraSpeedY * ((_mouseBorderMargin - pointer.get_y()) / _mouseBorderMargin);
  }
  else if (pointer.get_y() >= _graphicWindow->get_y_size() - _mouseBorderMargin)
  {
    cameraMotion |= MotionBottom;
    cameraSpeedY  = cameraSpeedY * ((pointer.get_y() - (_graphicWindow->get_y_size() - _mouseBorderMargin)) / _mouseBorderMargin);
  }
  if (pointer.get_x() <= _mouseBorderMargin)
  {
    cameraMotion |= MotionLeft;
    cameraSpeedX   = cameraSpeedX * ((_mouseBorderMargin - pointer.get_x()) / _mouseBorderMargin);
  }
  else if (pointer.get_x() >= _graphicWindow->get_x_size() - _mouseBorderMargin)
  {
    cameraMotion |= MotionRight;
    cameraSpeedX   = cameraSpeedX * ((pointer.get_x() - (_graphicWindow->get_x_size() - _mouseBorderMargin)) / _mouseBorderMargin);
  }

  cameraMovementX = cameraSpeedX * elapsedTime;
  cameraMovementY = cameraSpeedY * elapsedTime;
  cameraRot.set_x(cameraRot.get_x() * rad2deg);
  cameraRot.set_y(cameraRot.get_y() * rad2deg);
  cameraRot.set_z(cameraRot.get_z() * rad2deg);

  // Horizontal movement
  {
    movement.set_y(sin(cameraRot.get_x()));
    movement.set_x(cos(cameraRot.get_x()));

    if (cameraMotion & MotionLeft)
      movement *= -cameraMovementX;
    else if (cameraMotion & MotionRight)
      movement *= cameraMovementX;
    else
      movement *= 0;

    movement.set_z(0);
    _camera.set_pos(_camera.get_pos() + movement);
  }

  // Vertical movement
  {
    movement.set_x(tan(cameraRot.get_x()));
    movement.set_y(tan(cameraRot.get_y()));
    
    if (cameraMotion & MotionTop)
      movement *= -cameraMovementY;
    else if (cameraMotion & MotionBottom)
      movement *= cameraMovementY;
    else
      movement *= 0;

    movement.set_z(0);
    _camera.set_pos(_camera.get_pos() + movement);
  }
  
  if (_maxPosX != 0 || _minPosX != 0 || _maxPosY != 0 || _minPosY != 0)
  {
    if      (_camera.get_x() > _maxPosX) { _camera.set_x(_maxPosX); }
    else if (_camera.get_x() < _minPosX) { _camera.set_x(_minPosX); }
    if      (_camera.get_y() > _maxPosY) { _camera.set_y(_maxPosY); }
    else if (_camera.get_y() < _minPosY) { _camera.set_y(_minPosY); }
  }
}

/*
 * Camera/NodePath centering and following stuff
 */
void SceneCamera::CenterCameraInstant(LPoint3f pos)
{
  _cameraPos = pos;

  LPoint3f cameraRot = _camera.get_hpr();
  float    rad2deg   = 3.1415926535897 / 180;
  
  cameraRot.set_x(cameraRot.get_x() * rad2deg);
  cameraRot.set_y(cameraRot.get_y() * rad2deg);
  cameraRot.set_z(cameraRot.get_z() * rad2deg);
  
  _camera.set_x(pos.get_x() + sin(cameraRot.get_x()) * 100);
  _camera.set_y(pos.get_y() + sin(cameraRot.get_y()) * 100);
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

  LPoint3f cameraRot = _camera.get_hpr();  
  float    rad2deg   = 3.1415926535897 / 180;
  
  cameraRot.set_x(cameraRot.get_x() * rad2deg);
  cameraRot.set_y(cameraRot.get_y() * rad2deg);
  cameraRot.set_z(cameraRot.get_z() * rad2deg);  

  _objectivePos.set_x(np.get_x() + sin(cameraRot.get_x()) * 100);
  _objectivePos.set_y(np.get_y() + tan(cameraRot.get_y()) * 100);  
  
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
    _centeringCamera = false;
}
