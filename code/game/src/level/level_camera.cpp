#include "level/level_camera.hpp"
#include "level/level.hpp"
#include "level/objectnode.hpp"
#include "options.hpp"

using namespace std;

LevelCamera::LevelCamera(Level& level, WindowFramework* window, NodePath camera) : SceneCamera(window, camera), level(level)
{
}

void LevelCamera::FollowObject(InstanceDynamicObject* object)
{
  FollowNodePath(object->GetNodePath());
}

void LevelCamera::CenterOnObject(InstanceDynamicObject* object)
{
  CenterCameraOn(object->GetNodePath());
}

void LevelCamera::SetConfigurationFromLevelState(void)
{
  if (level.GetState() == Level::Fight)
    UseCombatConfiguration();
  else
    UseRegularConfiguration();
}

void LevelCamera::UseRegularConfiguration(void)
{
  if (OptionsManager::Get()["camera"]["focus-self"].Value() == "1")
    FollowObject(level.GetPlayer());
  else
    StopFollowingNodePath();
}

void LevelCamera::UseCombatConfiguration(void)
{
  if (level.GetCurrentFightPlayer() == level.GetPlayer())
    UseCombatConfigurationForPlayer();
  else
    UseCombatConfigurationForNPCs();
}

void LevelCamera::UseCombatConfigurationForPlayer()
{
  if (OptionsManager::Get()["camera"]["fight"]["focus-enemies"].Value() == "1")
    FollowObject(level.GetCurrentFightPlayer());
  else
    StopFollowingNodePath();
}

void LevelCamera::UseCombatConfigurationForNPCs()
{
  if (OptionsManager::Get()["camera"]["fight"]["focus-self"].Value() == "1")
    FollowObject(level.GetCurrentFightPlayer());
  else
    StopFollowingNodePath();
}
