#ifndef  CHARACTER_VISIBILITY_HPP
# define CHARACTER_VISIBILITY_HPP

# include "level/characters/action_points.hpp"

class CharacterVisibility : public CharacterActionPoints
{
public:
  CharacterVisibility(Level* level, DynamicObject* dynamic_object);

  virtual void Run(float elapsed_time);
  void         RunFade(float elapsedTime);
  void         SetVisible(bool do_set);

  virtual bool HasFlag(unsigned char) const = 0;

  virtual void Serialize(Utils::Packet&);
  virtual void Unserialize(Utils::Packet&);

private:
  void         Fading(void);

  bool         _fading_off, _fading_in;
};

#endif
