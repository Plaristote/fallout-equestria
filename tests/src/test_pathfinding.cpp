#include "test.hpp"
#include "timer.hpp"
#include <level/world.h>
#include <sstream>

using namespace std;

//
// Waypoint Observer Testing
//
struct TestArcObserver : public Waypoint::ArcObserver
{
  TestArcObserver() : can_go_through(true), went_through(false) {}

  bool CanGoThrough(unsigned char type)
  {
    went_through = true;
    return (can_go_through);
  }

  void GoingThrough(void*) {}

  bool can_go_through;
  bool went_through;
};

static void TestArcs(UnitTest& tester)
{
  tester.AddTest("Pathfinding", "Connecting arcs", []() -> string
  {
    Waypoint     first_waypoint;
    Waypoint     second_waypoint;
    Waypoint     successor_test;
    unsigned int successor_count;
    
    first_waypoint.id  = 1;
    second_waypoint.id = 2;
    successor_test.id = 3;
    first_waypoint.Connect(&second_waypoint);
    if (!(first_waypoint.GetArcTo(2)))
      return ("First waypoint wasn't properly connected to second waypoint");
    if (!(second_waypoint.GetArcTo(1)))
      return ("The arc connection doesn't go both ways");
    successor_count = first_waypoint.GetSuccessors(&successor_test).size();
    if (successor_count != 1)
    {
      std::stringstream errstream;
      
      errstream << "Expected successor_count to be 1, but was " << successor_count;
      return (errstream.str());
    }
    return ("");
  });
  tester.AddTest("Pathfinding", "Disconnecting arcs", []() -> string
  {
    Waypoint     first_waypoint;
    Waypoint     second_waypoint;

    first_waypoint.id  = 1;
    second_waypoint.id = 2;  
    first_waypoint.Connect(&second_waypoint);
    first_waypoint.Disconnect(&second_waypoint);
    if ((first_waypoint.GetArcTo(2)) != 0)
      return ("First waypoint wasn't properly disconnected");
    if ((second_waypoint.GetArcTo(1)) != 0)
      return ("The arc disconnected doesn't go both ways");
    return ("");
  });
  tester.AddTest("Pathfinding", "Arc observers", []() -> string
  {
    TestArcObserver arc_observer;
    Waypoint::Arc*  arc;
    Waypoint        successor_test;
    Waypoint        first_waypoint;
    Waypoint        second_waypoint;

    first_waypoint.id  = 1;
    second_waypoint.id = 2;  
    successor_test.id  = 3;
    first_waypoint.Connect(&second_waypoint);
    arc = first_waypoint.GetArcTo(second_waypoint.id);
    if (arc)
    {
      unsigned int successor_count;
      
      arc->observer = &arc_observer;
      first_waypoint.GetSuccessors(&successor_test);
      if (arc_observer.went_through == false)
        return ("Arc Observer wasn't called by Waypoint::GetSuccessors");
      arc_observer.can_go_through = false;
      successor_count = first_waypoint.GetSuccessors(&successor_test).size();
      if (successor_count != 0)
        return ("Arc Observer cannot block pathfinding");
    }
    else
      return ("Arc connection failed");
    return ("");
  });
  tester.AddTest("Pathfinding", "Arc withdrawal", []() -> string
  {
    Waypoint        successor_test;
    Waypoint        first_waypoint;
    Waypoint        second_waypoint;
    unsigned int    successor_count;

    first_waypoint.id  = 1;
    second_waypoint.id = 2;  
    successor_test.id  = 3;
    first_waypoint.Connect(&second_waypoint);
    first_waypoint.LoadArcs(); second_waypoint.LoadArcs();

    first_waypoint.WithdrawArc(&second_waypoint);

    successor_count = first_waypoint.GetSuccessors(&successor_test).size();
    if (successor_count != 0)
      return ("Arc wasn't withdrawed");
    successor_count = second_waypoint.GetSuccessors(&successor_test).size();
    if (successor_count != 0)
      return ("Arc withdrawal doesn't go both ways");
    
    first_waypoint.UnwithdrawArc(&second_waypoint, 0);
    
    successor_count = first_waypoint.GetSuccessors(&successor_test).size();
    if (successor_count != 1)
      return ("Arc wasn't unwithdrawed");
    successor_count = second_waypoint.GetSuccessors(&successor_test).size();
    if (successor_count != 1)
      return ("Arc unwithdrawal doesn't go both ways");
    return ("");
  });
}

#include <level/objectnode.hpp>

void TestWaypointModifiers(UnitTest& tester)
{
  tester.AddTest("Pathfinding", "Waypoint modifiers", [](void) -> string
  {
    Waypoint first_waypoint, second_waypoint, third_waypoint;

    first_waypoint.id = 1 ; second_waypoint.id = 2 ; third_waypoint.id = 3;
    first_waypoint.Connect(&second_waypoint);
    first_waypoint.Connect(&third_waypoint);
    second_waypoint.Connect(&third_waypoint);
    first_waypoint.LoadArcs() ; second_waypoint.LoadArcs() ; third_waypoint.LoadArcs();

    WaypointModifier wpm1, wpm2;
    unsigned int     successor_count;

    wpm1.SetOccupiedWaypoint(&first_waypoint);
    wpm2.SetOccupiedWaypoint(&second_waypoint);

    wpm1.ProcessCollisions();
    successor_count = first_waypoint.GetSuccessors(&third_waypoint).size();
    if (successor_count != 0)
      return ("Arc wasn't withdrawed");

    wpm1.UnprocessCollisions();
    successor_count = first_waypoint.GetSuccessors(&third_waypoint).size();
    if (successor_count != 1)
      return ("Arc wasn't unwithdrawed");

    wpm1.ProcessCollisions();
    wpm2.ProcessCollisions();
    successor_count = first_waypoint.GetSuccessors(&third_waypoint).size();
    if (successor_count != 0)
      return ("Second arc withdrawal failed");

    wpm2.UnprocessCollisions();
    successor_count = first_waypoint.GetSuccessors(&third_waypoint).size();
    if (successor_count != 0)
      return ("Unwithdrawed an arc before all the withdrawers unwithdrawed");

    wpm1.UnprocessCollisions();
    successor_count = first_waypoint.GetSuccessors(&third_waypoint).size();
    if (successor_count != 1)
      return ("Didn't unwithdraw the arc after all the withdrawers unwithdrawed");
    return ("");
  });
}

void TestsPathfinding(UnitTest& tester)
{
  TestArcs(tester);
  TestWaypointModifiers(tester);
}
