#ifndef  INTERACTIONS_TARGET_HPP
# define INTERACTIONS_TARGET_HPP

# include "globals.hpp"
# include "i18n.hpp"
# include "level/skill_target.hpp"
# include "level/interactions/interaction.hpp"

class Level;
class InstanceDynamicObject;
class ObjectCharacter;
class DynamicObject;

namespace Interactions
{
  class Target
  {
  public:
    Target(Level* level, DynamicObject* object);

    virtual std::string GetName(void) const = 0;

    InteractionList&     GetInteractions(void) {return (interactions); }
    void                 AddInteraction(const std::string& name, Trigger& trigger);
    void                 DelInteraction(const std::string& name);
    void                 ClearInteractions(void);
    void                 SetInteractionsFromDynamicObject(DynamicObject*);
    
    virtual void         ActionUse(InstanceDynamicObject* user);
    virtual void         ActionLookAt(InstanceDynamicObject* user);
    virtual void         ActionUseSkill(ObjectCharacter* user, const std::string& skill);
    virtual void         ActionTalkTo(ObjectCharacter* user);

    void                 ThatDoesNothing(InstanceDynamicObject* user);

  protected:
    void                 SetupScript(AngelScript::Object* script);
    
    SkillTarget          skill_target;

  private:
    bool                 IsPlayer(InstanceDynamicObject* user) const;
    void                 DisplayMessage(const std::string& message);

    Level*               level;
    InteractionList      interactions;
    AngelScript::Object* script;
  };
}

#endif