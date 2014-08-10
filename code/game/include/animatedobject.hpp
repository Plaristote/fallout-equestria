#ifndef  ANIMATEDOBJECT_HPP
# define ANIMATEDOBJECT_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/animControlCollection.h>
# include "observatory.hpp"

# define ANIMATION_PATH(model, str) ("models/anims/" + model + "-" + str + ".egg") 
# define ANIMATION_DEFAULT "use"

/**
 * @brief Helper for managing an animated model
 **/
class AnimatedObject
{
public:
  AnimatedObject(WindowFramework*);

  virtual NodePath          GetNodePath(void) const = 0;

  /**
   * @brief ... Starts an animation
   *
   * @param name ... Animation name
   * @param loop ... If set to true, the animation will continue until another animation is played
   * @return void
   **/
  void                      PlayAnimation(const std::string& name, bool loop = false);
  void                      PlayIdleAnimation(void);
  void                      StopAnimationLoop(void) { _animLoop = false; }

  bool                      IsPlayingAnimation(void)   const { return (_anim != 0); }
  std::string               PlayingAnimationName(void) const { return (_anim == 0 ? "idle" : current_animation); }

  /**
   * @brief ... Triggered when a not looping animation ends.
   **/
  Sync::Signal<void>                   AnimationEnd;
  Sync::Signal<void (AnimatedObject*)> AnimationEndForObject;

protected:
  typedef std::map<std::string, AnimControl*> MapAnims;

  bool                      LoadAnimation(const std::string& name);
  void                      TaskAnimation(void);
  void                      SetModelName(const std::string& model_name) { _modelName = model_name; }
  void                      SetModelNameFromPath(const std::string& path);

  WindowFramework*          _window;
  std::string               _modelName;
  AnimControlCollection     _anims;
  MapAnims                  _mapAnims;
  AnimControl*              _anim;
  bool                      _animLoop;
private:
  std::string               current_animation;
};

#endif
