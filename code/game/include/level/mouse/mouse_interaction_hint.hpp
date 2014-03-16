#ifndef  MOUSE_INTERACTION_HINT_HPP
# define MOUSE_INTERACTION_HINT_HPP

# include "mouse_target_hint.hpp"

class Level;
class InstanceDynamicObject;
class ObjectCharacter;

class MouseInteractionHint : public MouseTargetHint
{
public:
  MouseInteractionHint(Level& level);

  void             Run(float elapsed_time);
  void             SetHoveredObject(NodePath object_path);
  void             SetPlayer(ObjectCharacter* player) { this->player = player; }
  void             SetInteractionCursor(const std::string& type);

protected:
  Level&           level;
private:
  float            GetFieldOfViewRadius(void) const;
  void             SetHoveredObject(InstanceDynamicObject* object);
  void             SetHoveredCharacter(ObjectCharacter* character);

  ObjectCharacter* player;
};

#endif