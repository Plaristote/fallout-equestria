#ifndef  FLOORS_HPP
# define FLOORS_HPP

# include "globals.hpp"
# include "world/world.h"

class Level;
class InstanceDynamicObject;

class Floors
{
  class HidingFloor
  {
    NodePath    floor;
    bool        fadingIn, done;
    float       alpha;
  public:
    HidingFloor() : done(false) {}

    bool        operator==(NodePath np) const { return (floor == np); }
    bool        Done(void)              const { return (done);        }
    float       Alpha(void)             const { return (alpha);       }
    void        ForceAlpha(float _alpha)      { alpha = _alpha;       }
    void        SetNodePath(NodePath np);
    void        SetFadingIn(bool set);
    void        Run(float elapsedTime);
  };

public:
  Floors(Level& level) : level(level), current_floor(0), last_waypoint(0) {}

  void          RunFadingEffect(float elapsed_time);
  void          SetCurrentFloorFromObject(InstanceDynamicObject*);
  void          SetCurrentFloor(unsigned char floor);
  void          ShowOnlyFloor(unsigned char floor);
  bool          IsInsideBuilding(unsigned char& floor);
  unsigned char GetCurrentFloor(void) const { return (current_floor); }
  unsigned char GetFloorFromObject(InstanceDynamicObject*);

  void          EnableShowLowerFloors(bool enable) { show_lower_floors = enable; }

private:
  void          FadeInFloor(NodePath floor)  { FadeFloor(floor, true);  }
  void          FadeOutFloor(NodePath floor) { FadeFloor(floor, false); }
  void          FadeFloor(NodePath floor, bool fade_in);

  Level&                 level;
  unsigned char          current_floor;
  std::list<HidingFloor> hiding_floors;
  unsigned int           last_waypoint;
  bool                   show_lower_floors;
};

#endif
