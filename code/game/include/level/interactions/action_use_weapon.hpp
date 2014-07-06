#ifndef  INTERACTIONS_ACTIONS_USE_WEAPON_HPP
# define INTERACTIONS_ACTIONS_USE_WEAPON_HPP

# include "action_runner.hpp"
# include "level/inventory.hpp"
# define MAX_EQUIPED_SLOTS 2
# define DEFAULT_XP_REWARD 100

namespace Interactions
{
  namespace Actions
  {
    class UseWeaponOn : public ActionRunner
    {
      Data             action;
      InventoryObject* weapon;
      unsigned char    action_it;
      unsigned int     equiped_it;
      bool             enemy_died, hit_success;
      LPoint3f         hit_position;
      
      UseWeaponOn(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item, unsigned char action_it);

      void         Run(void);
      virtual void PlayAnimation();
      virtual void RunAction();
      void         TargetDied(ObjectCharacter*);
      void         TargetAnimate();
      void         FireProjectile();
      bool         HasProjectile(void) const;
      void         ApplyEffect(void);
      void         FindEquipedIterator();

    public:
      static ActionRunner* Factory(ObjectCharacter* user, ObjectCharacter* target, InventoryObject* item, unsigned char action_it);
    };
  }
}

#endif
