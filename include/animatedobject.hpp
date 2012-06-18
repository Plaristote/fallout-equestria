#ifndef  ANIMATEDOBJECT_HPP
# define ANIMATEDOBJECT_HPP

# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/animControlCollection.h>

# define ANIMATION_PATH(model, str) ("models/anims/" + model + "-" + str + ".egg") 
# define ANIMATION_DEFAULT "use"

class AnimatedObject
{
public:
  AnimatedObject(WindowFramework*);

  virtual NodePath          GetNodePath(void) const = 0;
  virtual void              ResetAnimation(void)    = 0;

  void                      PlayAnimation(const std::string& name, bool loop = false);
  Observatory::Signal<void> AnimationEnd;

  bool                      pendingAnimationDone;

protected:
  typedef std::map<std::string, AnimControl*> MapAnims;

  void                      LoadAnimation(const std::string& name);
  void                      TaskAnimation(void);
  void                      PlayIdleAnimation(void);

  WindowFramework*          _window;
  std::string               _modelName;
  AnimControlCollection     _anims;
  MapAnims                  _mapAnims;
  AnimControl*              _anim;
  bool                      _animLoop;  
  
  bool                      _idleSemaphore;
};

#endif