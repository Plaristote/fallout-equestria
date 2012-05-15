#include "scene_camera.hpp"

using namespace std;

void SceneCamera::Run(float elapsedTime)
{
  if (_scrollEnabled)
    RunScroll(elapsedTime);
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

  if      (cameraMotion & MotionTop)
    cameraPos.set_y(cameraPos.get_y() + cameraMovement);
  else if (cameraMotion & MotionBottom)
    cameraPos.set_y(cameraPos.get_y() - cameraMovement);
  if (cameraMotion & MotionLeft)
    cameraPos.set_x(cameraPos.get_x() - cameraMovement);
  else if (cameraMotion & MotionRight)
    cameraPos.set_x(cameraPos.get_x() + cameraMovement);

  _camera.set_pos(cameraPos);
}
