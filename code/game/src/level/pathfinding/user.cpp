#include "level/pathfinding/user.hpp"
#include "level/level.hpp"
#include "circular_value_set.hpp"

using namespace std;

namespace Pathfinding
{
  extern void* current_user;
}

LPoint3f NodePathSize(NodePath);

Pathfinding::User::User(Level* level, DynamicObject* object) : InstanceDynamicObject(level, object)
{
  path.SetRenderNode(level->GetWorld()->window->get_render());
  rotation_goal  = 0.f;
  path_visible   = false;
  movement_speed = 20.f;
}

void Pathfinding::User::Run(float elapsed_time)
{
  if (path.Size() > 0)
    RunMovement(elapsed_time);
  if (rotation_goal != _object->nodePath.get_hpr().get_x())
    RunRotate(elapsed_time);
  InstanceDynamicObject::Run(elapsed_time);
}

void Pathfinding::User::SetPathVisible(bool set)
{
  if (path_visible != set)
  {
    if (path_visible)
      path.ClearDisplay();
    path_visible = set;
  }
}

unsigned short      Pathfinding::User::GetPathDistance(Pathfinding::Collider* object)
{
  unsigned short ret;

  ret = GetPathDistance(object->GetOccupiedWaypoint());
  return (ret);
}

unsigned short      Pathfinding::User::GetPathDistance(Waypoint* waypoint)
{
  Pathfinding::Path path;

  UnprocessCollisions();
  current_user = this;
  path.FindPath(GetOccupiedWaypoint(), waypoint);
  current_user = 0;
  ProcessCollisions();
  path.StripFirstWaypointFromList();
  return (path.Size());
}

void                Pathfinding::User::GoToRandomDirection(void)
{
  Waypoint* from = GetOccupiedWaypoint();
  
  if (from)
  {
    Waypoint* to   = from->GetRandomWaypoint();

    if (to)
    {
      UnprocessCollisions();
      path.FindPath(from, to);
      ProcessCollisions();
    }
  }
}

void                Pathfinding::User::TeleportTo(Waypoint* waypoint)
{
  if (waypoint)
  {
    LPoint3  wp_size  = NodePathSize(waypoint->nodePath);
    LPoint3f position = waypoint->nodePath.get_pos();
    float    z        = (position.get_z() - wp_size.get_z()) + 1;

    position.set_z(z + (GetSize().get_z() / 2));
    GetNodePath().set_pos(position);
    SetOccupiedWaypoint(waypoint);
  }
}

void                Pathfinding::User::GoTo(Waypoint* waypoint)
{
  PStatCollector collector("Level:Characters:Pathfinding");
  Waypoint*      start_from = GetOccupiedWaypoint();

  current_target = Destination();
  collector.start();
  ReachedDestination.DisconnectAll();
  UnprocessCollisions();
  current_user = this;
  if (start_from && waypoint)
  {
    if (!(path.FindPath(start_from, waypoint)))
    {
      if (_level->GetPlayer() == this)
        _level->GetLevelUi().GetMainBar().AppendToConsole(i18n::T("No path."));
    }
    else if (path.Size() > 1)
      StartRunAnimation();
  }
  else
    cout << "Character " << GetName() << " doesn't have a waypointOccupied" << endl;
  current_user = 0;
  ProcessCollisions();
  collector.stop();
}

void                Pathfinding::User::GoTo(InstanceDynamicObject* object, int min_distance)
{
  current_target.object       = object;
  current_target.min_distance = min_distance;
  path                        = object->GetPathTowardsObject(this);
  if (path.ContainsValidPath())
  {
    if (path.Size() > 0)
      StartRunAnimation();
    else
      TriggerDestinationReached();
  }
  else
  {
    ReachedDestination.DisconnectAll();
    if (_level->GetPlayer() == this)
      _level->GetLevelUi().GetMainBar().AppendToConsole(i18n::T("Can't reach."));
  }
}

void Pathfinding::User::StartRunAnimation()
{
  ReachedDestination.Connect([this]() { StopAnimationLoop(); });
  PlayAnimation(movement_animation, true);
}

void Pathfinding::User::TruncatePath(unsigned short max_size)
{
  if ((path.Size() > 0) && path.Front().id == GetOccupiedWaypointAsInt())
    max_size++;
  path.Truncate(max_size);
  if (ReachedDestination.ObserverCount() > 0 && !IsMoving())
    PlayIdleAnimation();
}

bool                Pathfinding::User::HasReachedTarget(void)
{
  return (current_target.object &&
          path.Size() <= current_target.min_distance && HasLineOfSight(current_target.object));
}

void Pathfinding::User::FindNewWayOrAbort(void)
{
  if (current_target.object)
    GoTo(current_target.object, current_target.min_distance);
  else
  {
    Waypoint* dest = _level->GetWorld()->GetWaypointFromId((path.Last()).id);

    GoTo(dest);
  }
  if (path.Size() == 0)
    ReachedDestination.DisconnectAll();
}

void Pathfinding::User::MovePathForward(void)
{
  if (HasReachedTarget())
    path.Clear();
}

bool Pathfinding::User::GoThroughNextWaypoint(void)
{
  Waypoint::Arc* arc;

  UnprocessCollisions();
  {
    arc = GetOccupiedWaypoint()->GetArcTo(path.Front().id);
    if (arc && arc->CanGoThrough(this))
      arc->GoingThrough(this);
    else
      arc = 0;
  }
  ProcessCollisions();
  if (!arc)
    FindNewWayOrAbort();
  return (arc != 0);
}

void Pathfinding::User::SetNextWaypoint(void)
{
  Waypoint* wp            = _level->GetWorld()->GetWaypointFromId(path.Front().id);
  Waypoint* last_occupied = GetOccupiedWaypoint();

  while (wp == last_occupied)
  {
    path.StripFirstWaypointFromList();
    MovePathForward();
    if (path.Size() == 0)
      return ;
    wp = _level->GetWorld()->GetWaypointFromId(path.Front().id);
  }
  if (GoThroughNextWaypoint())
  {
    // Check if the character has been teleported by ZoneManager
    if (last_occupied == GetOccupiedWaypoint())
    {
      MovePathForward();
      SetOccupiedWaypoint(wp);
      MovedFor1ActionPoint.Emit();
    }
    // If teleported, re-compute the path from the newly occupied waypoint.
    else if (path.Size() > 0)
    {
      cout << "Penis ?" << endl;
      GoTo(_level->GetWorld()->GetWaypointFromId(path.Last().id));
    }
    else
      cout << "Hello ?" << endl;
  }
}

void Pathfinding::User::RunNextMovement(float elapsedTime)
{
  SetNextWaypoint();
  if (path.Size() > 0)
  {
    if (path_visible)
      path.CreateDisplay();
  }
  else
  {
    cout << "Destination reached" << endl;
    TriggerDestinationReached();
  }
}

void Pathfinding::User::TriggerDestinationReached(void)
{
  cout << "Trigger destination reached" << endl;
  path.Clear();
  ReachedDestination.Emit();
  ReachedDestination.DisconnectAll();
  PlayIdleAnimation();
}

LPoint3             Pathfinding::User::GetDistanceToNextWaypoint(void) const
{
  const Waypoint& next_waypoint    = path.Front();
  LPoint3         current_position = _object->nodePath.get_pos();
  LPoint3         objective        = next_waypoint.nodePath.get_pos();
  LPoint3         waypoint_size    = NodePathSize(next_waypoint.nodePath);
  float           height_objective = ((objective.get_z() - waypoint_size.get_z()) + 1) + (GetSize().get_z() / 2);

  objective.set_z(height_objective);
  return (current_position - objective);
}

bool                Pathfinding::User::IsMoving(void) const
{
  return (path.Size() > 0);
}

void                Pathfinding::User::RunMovement(float elapsedTime)
{
  LPoint3           distance  = GetDistanceToNextWaypoint();
  float             max_speed;
  float             max_distance;    
  LPoint3           speed, axis_speed, dest;
  int               dirX, dirY, dirZ;

  max_speed   = movement_speed * elapsedTime;
  if (distance.get_x() == 0 && distance.get_y() == 0 && distance.get_z() == 0)
    RunNextMovement(elapsedTime);
  else
  {
    dirX = dirY = dirZ = 1;
    if (distance.get_x() < 0)
    {
      distance.set_x(-(distance.get_x()));
      dirX = -1;
    }
    if (distance.get_y() < 0)
    {
      distance.set_y(-(distance.get_y()));
      dirY = -1;
    }
    if (distance.get_z() < 0)
    {
      distance.set_z(-(distance.get_z()));
      dirZ = -1;
    }
    
    max_distance = (distance.get_x() > distance.get_y() ? distance.get_x() : distance.get_y());
    max_distance = (distance.get_z() > max_distance     ? distance.get_z() : max_distance);

    axis_speed.set_x(distance.get_x() / max_distance);
    axis_speed.set_y(distance.get_y() / max_distance);
    axis_speed.set_z(distance.get_z() / max_distance);
    if (max_speed > max_distance)
      max_speed = max_distance;
    speed.set_x(max_speed * axis_speed.get_x() * dirX);
    speed.set_y(max_speed * axis_speed.get_y() * dirY);
    speed.set_z(max_speed * axis_speed.get_z() * dirZ);

    dest = _object->nodePath.get_pos() - speed;

    LookAt(dest);
    _object->nodePath.set_pos(dest);
  }
}

void                Pathfinding::User::RunRotate(float elapsedTime)
{
  LVecBase3 rot    = _object->nodePath.get_hpr();
  float     factor = elapsedTime * 500;

  if ((ABS(rotation_goal - rot.get_x())) < factor)
    rot.set_x(rotation_goal);
  else
  {
    CircularValueSet value_set(360);
    float            positive_distance = value_set.AdditionDistance    (ABS(rotation_goal), ABS(rot.get_x()));
    float            negative_distance = value_set.SubstractionDistance(ABS(rotation_goal), ABS(rot.get_x()));

    if (negative_distance > positive_distance)
      rot.set_x(rot.get_x() - factor);
    else
      rot.set_x(rot.get_x() + factor);

    if (rot.get_x() < -360)
      rot.set_x(rot.get_x() + 360);
    else if (rot.get_x() > 0)
      rot.set_x(-360 + rot.get_x());
  }
  _object->nodePath.set_hpr(rot);
}

void                Pathfinding::User::LookAt(LVecBase3 pos)
{
   LVecBase3 rot, backup;

   backup = _object->nodePath.get_hpr();
   _object->nodePath.look_at(pos);
   rot = _object->nodePath.get_hpr();
   rot.set_x(rot.get_x() - 180);
   rot.set_y(-rot.get_y());
   rotation_goal = rot.get_x();
   rot.set_x(backup.get_x());
   if (pos.get_y() == _object->nodePath.get_y())
     rot.set_y(backup.get_y());
   _object->nodePath.set_hpr(rot);
}

void                Pathfinding::User::LookAt(InstanceDynamicObject* object)
{
  LVecBase3 pos = object->GetNodePath().get_pos();

  LookAt(pos);
}

void Pathfinding::User::Serialize(Utils::Packet& packet)
{
  packet << movement_animation << movement_speed << rotation_goal;
  path.Serialize(packet);
  InstanceDynamicObject::Serialize(packet);
}

void Pathfinding::User::Unserialize(Utils::Packet& packet)
{
  packet >> movement_animation >> movement_speed >> rotation_goal;
  path.Unserialize(_level->GetWorld(), packet);
  InstanceDynamicObject::Unserialize(packet);
}
