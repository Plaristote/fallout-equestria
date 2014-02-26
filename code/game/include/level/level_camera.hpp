#ifndef  LEVEL_CAMERA_HPP
# define LEVEL_CAMERA_HPP

# include "globals.hpp"
# include "world/scene_camera.hpp"

class Level;
class InstanceDynamicObject;

class LevelCamera : public SceneCamera
{
public:
  LevelCamera(Level& level, WindowFramework* window, NodePath camera);

  void            SetConfigurationFromLevelState();
  void            UseCombatConfiguration(void);
  void            UseRegularConfiguration(void);

  void            CenterOnObject(InstanceDynamicObject*);
  void            FollowObject(InstanceDynamicObject*);
private:
  void            UseCombatConfigurationForPlayer(void);
  void            UseCombatConfigurationForNPCs(void);
  
  Level&          level;
};

#endif