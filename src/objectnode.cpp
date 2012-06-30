#include "globals.hpp"
#include "objectnode.hpp"
#include <panda3d/nodePathCollection.h>
#include <panda3d/auto_bind.h>
#include "level.hpp"

using namespace std;
using namespace Observatory;
using namespace ObjectTypes;

void InstanceDynamicObject::ThatDoesNothing() { _level->ConsoleWrite("That does nothing"); }

InstanceDynamicObject::InstanceDynamicObject(Level* level, DynamicObject* object) : AnimatedObject(level->GetWorld()->window), _object(object)
{
  _type                 = Other;
  _anim                 = 0;
  _animLoop             = true;
  _level                = level;
  _waypointDisconnected = object->lockedArcs;
  _waypointOccupied     = object->waypoint;

  // Get model name from path
  _modelName            = object->strModel;
  for (short i = 0 ; i < _modelName.size() ; ++i)
  {
    if (_modelName[i] == '/' || _modelName[i] == '\\')
    {
      _modelName.erase(0, i);
      i = 0;
    }
  }
  for (short i = _modelName.size() - 1 ; i >= 0 ; --i)
  {
    if (_modelName[i] == '.')
    {
      _modelName.erase(i);
      break ;
    }
  }

  if (object->interactions & Interactions::TalkTo)
    _interactions.push_back(Interaction("talk_to",    this, &ActionTalkTo));
  if (object->interactions & Interactions::Use)
    _interactions.push_back(Interaction("use",        this, &ActionUse));
  if (object->interactions & Interactions::UseObject)
    _interactions.push_back(Interaction("use_object", this, &ActionUseObjectOn));
  if (object->interactions & Interactions::UseSkill)
    _interactions.push_back(Interaction("use_skill",  this, &ActionUseSkillOn));
  
  AnimationEnd.Connect(*this, &InstanceDynamicObject::CallbackAnimationEnded);
}

InstanceDynamicObject::GoToData   InstanceDynamicObject::GetGoToData(InstanceDynamicObject* character)
{
  GoToData         ret;

  ret.nearest      = _waypointOccupied;
  ret.objective    = this;
  ret.max_distance = 0;
  ret.min_distance = 0;
  return (ret);
}

// Animations
AnimatedObject::AnimatedObject(WindowFramework* window) : _window(window)
{
  pendingAnimationDone = false;
  AnimationEnd.Connect(*this, &AnimatedObject::PlayIdleAnimation);
}

void                     AnimatedObject::LoadAnimation(const std::string& name)
{
  NodePath root = GetNodePath();
  NodePath np   = _window->load_model(root, ANIMATION_PATH(_modelName, name));
  string   controlName;

  if (np.get_error_type() != NodePath::ET_ok)
  {
    std::cout << "Can't load anim " << name << " for " << _modelName << std::endl;
    return ;
  }
  auto_bind(root.node(), _anims, 0xf);
  np.detach_node();
  controlName = _anims.get_anim_name(_anims.get_num_anims() - 1);
  _mapAnims.insert(_mapAnims.end(), pair<string, AnimControl*>(name, _anims.find_anim(controlName)));
}

void                     AnimatedObject::PlayAnimation(const std::string& name, bool loop)
{
  MapAnims::iterator     it   = _mapAnims.find(name);
  AnimControl*           anim = (it != _mapAnims.end() ? it->second : 0);

  if (anim)
  {
    std::cout << "Playing animation " << name << " now." << std::endl;
    if (anim->is_playing() && loop)
      return ;
    anim->loop(true);
    anim->play();
    _anim     = anim;
    _animLoop = loop;
  }
  else if (!loop && name != ANIMATION_DEFAULT)
    PlayAnimation(ANIMATION_DEFAULT, loop);
  else
  {
    pendingAnimationDone = true;
    AnimationEnd.Emit();
    pendingAnimationDone = false;
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
      std::cout << "Animation is over" << std::endl;
      pendingAnimationDone = true;
      AnimationEnd.Emit();
      ResetAnimation();
      pendingAnimationDone = false;
      _anim = 0;
    }
  }
}

void                      AnimatedObject::PlayIdleAnimation(void)
{
  MapAnims::iterator     it   = _mapAnims.find("idle");
  AnimControl*           anim = (it != _mapAnims.end() ? it->second : 0);

  if (anim)
    anim->play();
}
