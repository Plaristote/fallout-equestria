#ifndef  OBJECT_VISIBILITY_HPP
# define OBJECT_VISIBILITY_HPP

# include "animatedobject.hpp"
# include "serializer.hpp"

class ObjectVisibility : public AnimatedObject
{
public:
  ObjectVisibility(WindowFramework*);

  virtual void Run(float elapsed_time);
  void         RunFade(float elapsedTime);
  void         SetVisible(bool do_set);
  bool         IsHidden(void) const;

  virtual bool HasFlag(unsigned char) const = 0;

  virtual void Serialize(Utils::Packet&);
  virtual void Unserialize(Utils::Packet&);

protected:
  void         Initialize(void);
private:
  void         Fading(void);

  bool         _fading_off, _fading_in;
};

#endif
