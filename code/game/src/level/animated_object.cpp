#include "level/objects/instance_dynamic_object.hpp"
#include <panda3d/nodePathCollection.h>
#include <panda3d/auto_bind.h>

using namespace std;

AnimatedObject::AnimatedObject(WindowFramework* window) : _window(window), _anim(0), _animLoop(true)
{
  AnimationEnd.Connect(*this, &AnimatedObject::PlayIdleAnimation);
}

bool                     AnimatedObject::LoadAnimation(const std::string& name)
{
  NodePath root = GetNodePath();
  NodePath np   = _window->load_model(root, ANIMATION_PATH(_modelName, name));
  string   controlName;

  if (np.get_error_type() != NodePath::ET_ok)
  {
    std::cout << "Can't load anim " << name << " for " << _modelName << std::endl;
    return (false);
  }
  auto_bind(root.node(), _anims, 0xf);
  np.detach_node();
  controlName = _anims.get_anim_name(_anims.get_num_anims() - 1);
  _mapAnims.insert(_mapAnims.end(), pair<string, AnimControl*>(name, _anims.find_anim(controlName)));
  return (true);
}

void                     AnimatedObject::PlayAnimation(const std::string& name, bool loop)
{
  cout << this << " - PlayAnimation('" << name << "')" << endl;
  MapAnims::iterator     it   = _mapAnims.find(name);
  AnimControl*           anim = (it != _mapAnims.end() ? it->second : 0);

  if (anim)
  {
    if (anim->is_playing() && loop)
      return ;
    anim->loop(true);
    anim->play();
    _anim     = anim;
    _animLoop = loop;
  }
  else if (LoadAnimation(name))
    PlayAnimation(name, loop);
  else if (!loop && name != ANIMATION_DEFAULT)
    PlayAnimation(ANIMATION_DEFAULT, loop);
  else
  {
    AnimationEnd.Emit();
    AnimationEndForObject.Emit(this);
  }
}

void                      AnimatedObject::TaskAnimation(void)
{
  if (_anim && _anim->is_playing() == false)
  {
    if (_animLoop)
      _anim->play();
    else
    {
      //cout << this << " - Animation ended naturally" << endl;
      AnimationEnd.Emit();
      AnimationEndForObject.Emit(this);
      AnimationEndForObject.DisconnectAll();
      _anim = 0;
    }
  }
}

void                      AnimatedObject::PlayIdleAnimation(void)
{
  //cout << this << " - PlayIdleAnimation" << endl;
  MapAnims::iterator     it   = _mapAnims.find("idle");
  AnimControl*           anim = (it != _mapAnims.end() ? it->second : 0);

  if (anim)
    anim->play();
}

void                      AnimatedObject::SetModelNameFromPath(const string& path)
{
  _modelName = path;
  for (short i = _modelName.size() - 1 ; i >= 0 ; --i)
  {
    if (_modelName[i] == '.')
    {
      _modelName.erase(i);
      break ;
    }
  }
}
