#include "level/objectnode.hpp"
#include <panda3d/nodePathCollection.h>
#include <panda3d/auto_bind.h>
#include "level/level.hpp"

using namespace std;
using namespace Sync;
using namespace ObjectTypes;

void InstanceDynamicObject::ThatDoesNothing()
{
  if (_level->GetPlayer() == this)
    _level->ConsoleWrite(i18n::T("That does nothing"));
}

InstanceDynamicObject::InstanceDynamicObject(Level* level, DynamicObject* object) : AnimatedObject(level->GetWorld()->window), _object(object), _skill_target(this)
{
  _type                 = Other;
  _anim                 = 0;
  _animLoop             = true;
  _level                = level;
  _waypointDisconnected = object->lockedArcs;
  _waypointOccupied     = object->waypoint;
  _idle_size            = NodePathSize(object->nodePath);

  // Get model name from path
  _modelName            = object->strModel;
  for (unsigned short i = 0 ; i < _modelName.size() ; ++i)
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
  if (object->interactions & Interactions::UseSpell)
    _interactions.push_back(Interaction("use_magic",  this, &ActionUseSpellOn));
  
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

void InstanceDynamicObject::CallbackActionUseSkill(ObjectCharacter* character, const std::string& name)
{
  _skill_target.UseSkill(character, name);
}

void InstanceDynamicObject::AddTextBox(const std::string& text, unsigned short r, unsigned short g, unsigned short b, float timeout)
{
  _level->GetChatterManager().PushTextBox(this, text, r, g, b, timeout);
}
