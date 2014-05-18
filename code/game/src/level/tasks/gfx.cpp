#include "level/tasks/scripted_task.hpp"
#include "level/objects/instance_dynamic_object.hpp"
#include "level/objects/character.hpp"
#include "level/level.hpp"

using namespace std;

void ScriptedTask::Gfx::Initialize(const std::string& name, const std::string& joint)
{
  this->name  = name;
  this->joint = joint;
  particle_effect.LoadConfiguration("data/particle-effects/" + name + ".json");
}

void ScriptedTask::Gfx::SetTarget(InstanceDynamicObject* object)
{
  if (object)
  {
    if (joint == "")
      particle_effect.ReparentTo(object->GetNodePath());
    else
    {
      ObjectCharacter* character = object->Get<ObjectCharacter>();

      if (character)
        particle_effect.ReparentTo(character->GetJoint(joint));
    }
  }
}

void ScriptedTask::Gfx::Serialize(Utils::Packet& packet) const
{
  packet << name << joint;
}

void ScriptedTask::Gfx::Unserialize(Utils::Packet& packet)
{
  packet >> name >> joint;
  Initialize(name, joint);
}
