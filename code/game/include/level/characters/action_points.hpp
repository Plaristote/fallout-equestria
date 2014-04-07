#ifndef  CHARACTER_ACTION_POINTS_HPP
# define CHARACTER_ACTION_POINTS_HPP

# include "level/characters/statistics.hpp"

class CharacterActionPoints : public CharacterStatistics
{
public:
  CharacterActionPoints(Level* level, DynamicObject* dynamic_object) : CharacterStatistics(level, dynamic_object)
  {
  }

  unsigned short     GetActionPoints(void)    const;
  unsigned short     GetMaxActionPoints(void) const;
  void               SetActionPoints(unsigned short ap);
  bool               UseActionPoints(unsigned short ap);
  void               RefreshActionPoints(void);
  void               ConvertRemainingActionPointsToArmorClass(void);
  
private:
  void               RemoveArmorClassBonusFromActionPoints(void);
};

#endif
