#include "scene_camera.hpp"

using namespace std;

//HAIL MICROSOFT

std::vector<LPoint3f> cameraAngles;
/*std::vector<LPoint3f> cameraAngles = {
  LPoint3f(-40, -40, 0),
  LPoint3f(0, -60, 0),
  LPoint3f(40, -40, 0),
  LPoint3f(40 - 180, -40, 0),
  LPoint3f(-180, -60, 0),
  LPoint3f(-40 - 180, -40, 0),
};*/

std::vector<bool> scrollSwapAngles;
/*std::vector<bool> scrollSwapAngles = {
  true,
  false,
  false,
  true,
  false,
  false,
};*/

std::vector<bool> scrollSwapAxis;
/*std::vector<bool> scrollSwapAxis = {
  false,
  false,
  false,
  true,
  true,
  true,
};*/


SceneCamera::SceneCamera(WindowFramework* window, NodePath camera) : _window(window), _graphicWindow(window->get_graphics_window()), _camera(camera)
{
  _scrollEnabled = true;

  camera.set_pos(0, 0, 75);
  //camera.set_hpr(0, -60, 0);
  camera.set_hpr(-40, -40, 0);

  //HAIL MICROSOFT
  //(didn't add a function because I'm tired from all this compilation nonsense right now)
  cameraAngles.push_back( LPoint3f(-40, -40, 0) );
  scrollSwapAngles.push_back( true );
  scrollSwapAxis.push_back( false );
  cameraAngles.push_back( LPoint3f(-0, -60, 0) );
  scrollSwapAngles.push_back( false );
  scrollSwapAxis.push_back( false );
  cameraAngles.push_back( LPoint3f(40, -40, 0) );
  scrollSwapAngles.push_back( false );
  scrollSwapAxis.push_back( false );
  cameraAngles.push_back( LPoint3f(40 - 180, -40, 0) );
  scrollSwapAngles.push_back( true );
  scrollSwapAxis.push_back( true );
  cameraAngles.push_back( LPoint3f(-180, -60, 0) );
  scrollSwapAngles.push_back( false );
  scrollSwapAxis.push_back( true );
  cameraAngles.push_back( LPoint3f(-40 - 180, -40, 0) );
  scrollSwapAngles.push_back( false );
  scrollSwapAxis.push_back( true );
  _currentCameraAngle = 0;
  _currentHpr         = cameraAngles[_currentCameraAngle];
  _objectiveHpr       = _currentHpr;

  _minPosX = _minPosY = _maxPosX = _maxPosY = 0;
}

void SceneCamera::SwapCameraView(void)
{
  if (++_currentCameraAngle >= cameraAngles.size())
    _currentCameraAngle = 0;
  _objectiveHpr = cameraAngles[_currentCameraAngle];
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
    cout << "Need to rotate camera" << endl;
  }
}

void SceneCamera::RunScroll(float elapsedTime)
{
  MouseData     pointer      = _graphicWindow->get_pointer(0);
  unsigned char cameraMotion = MotionNone;
  LPoint3       cameraPos    = _camera.get_pos();

  int   _mouseBorderMargin   = 20;
  float _cameraMovementSpeed = 15.f;
  float cameraMovement       = _cameraMovementSpeed * elapsedTime;

  if (pointer.get_y() <= _mouseBorderMargin)
    cameraMotion |= MotionTop;
  else if (pointer.get_y() >= _graphicWindow->get_y_size() - _mouseBorderMargin)
    cameraMotion |= MotionBottom;
  if (pointer.get_x() <= _mouseBorderMargin)
    cameraMotion |= MotionLeft;
  else if (pointer.get_x() >= _graphicWindow->get_x_size() - _mouseBorderMargin)
    cameraMotion |= MotionRight;

  unsigned char motionTop    = MotionTop;
  unsigned char motionBottom = MotionBottom;
  unsigned char motionLeft   = MotionLeft;
  unsigned char motionRight  = MotionRight;

  if (scrollSwapAxis[_currentCameraAngle])
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
    cameraPos.set_y(_maxPosY);

  _camera.set_pos(cameraPos);
}
