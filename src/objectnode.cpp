#include "globals.hpp"
#include "objectnode.hpp"
#include <panda3d/nodePathCollection.h>
#include <panda3d/auto_bind.h>
#define ANIMATION_PATH(model, str) ("models/anims/" + model + "-" + str + ".egg") 
#define ANIMATION_DEFAULT "use"
#include "level.hpp"

using namespace std;
using namespace Observatory;

void InstanceDynamicObject::ThatDoesNothing() { _level->ConsoleWrite("That does nothing"); }

InstanceDynamicObject::InstanceDynamicObject(Level* level, DynamicObject* object) : _object(object)
{
  _type                 = Other;
  _anim                 = 0;
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
void                     InstanceDynamicObject::LoadAnimation(const std::string& name)
{
  NodePath np = _level->GetWorld()->window->load_model(_object->nodePath, ANIMATION_PATH(_modelName, name));

  if (np.get_error_type() != NodePath::ET_ok)
  {
    std::cout << "Can't load anim " << name << " for " << _modelName << std::endl;
    return ;
  }
  auto_bind(_object->nodePath.node(), _anims, 0);
  np.detach_node();
  int nanim = _anims.get_num_anims();
  std::cout << "nanim is " << nanim << std::endl;
  string animname = _anims.get_anim_name(nanim - 1);
  std::cout << "Anim name is " << animname << std::endl;
}

void                     InstanceDynamicObject::PlayAnimation(const std::string& name, bool loop)
{
  const std::string key  = ANIMATION_PATH(_modelName, name);
  AnimControl*      anim = _anims.find_anim(key);
  
  std::cout << "Animation not playing" << std::endl;
  if (anim)
  {
    std::cout << "Animation playing correctly" << std::endl;
    AnimationEnded.DisconnectAll();
    if (anim->is_playing() && loop)
      return ;
    anim->loop(loop);
    anim->play();
    _anim = anim;
    pendingAnimationDone = false;
  }
  else if (!loop && name != ANIMATION_DEFAULT)
    PlayAnimation(ANIMATION_DEFAULT, loop);
  else
  {
    pendingAnimationDone = true;
    AnimationEnded.Emit(this);
  }
}

void                      InstanceDynamicObject::TaskAnimation(void)
{
  if (_anim && _anim->is_playing() == false)
  {
    pendingAnimationDone = true;
    AnimationEnded.Emit(this);
    _anim = 0;
  }
}