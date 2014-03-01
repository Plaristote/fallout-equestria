#ifndef  GRAPHICAL_EFFECT_HPP
# define GRAPHICAL_EFFECT_HPP

# include "globals.hpp"
# include "animatedobject.hpp"
# include "datatree.hpp"

class InstanceDynamicObject;

class GraphicalEffect : public AnimatedObject
{
public:
  GraphicalEffect(WindowFramework* window, Data data);
  ~GraphicalEffect();

  void SetOnObject(InstanceDynamicObject* object);

private:
  void SetModel(Data model);
  void SetColor(Data color);
  void SetScale(Data scale);
  void SetPosition(Data position);
  void SetRotation(Data rotation);
  void SetTexture(Data texture);
  void SetAnimation(Data animation);
  
  NodePath root;
};

#endif