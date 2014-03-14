#include "world/world.h"
#include "world/waypoint.hpp"
#include "world/world.h"
#include "dices.hpp"

using namespace std;

extern unsigned int   blob_revision;

namespace Pathfinding
{
  void* current_user = 0;
}

Waypoint::Waypoint(NodePath root)
{
  selected = false;
  nodePath = root;
  //nodePath.set_collide_mask(CollideMask(ColMask::Waypoint));
  nodePath.set_scale(2.f);
  nodePath.show();

  nodePath.set_transparency(TransparencyAttrib::M_alpha);
  nodePath.set_color(0, 0, 0, 0.5);

  floor    = 0;
}

bool                Waypoint::Arc::CanGoThrough(void* object)
{
  if (observer)
    return (observer->CanGoThrough(from, to, object));
  return (true);
}

void                Waypoint::Arc::GoingThrough(void* object)
{
  if (observer)
    observer->GoingThrough(from, to, object);
}

void                Waypoint::SetSelected(bool selected)
{
  this->selected = selected;
  if (!nodePath.is_empty())
  {
    if (selected)
      nodePath.set_color(0, 1.0, 0, 0.5);
    else
      nodePath.set_color(0, 0, 0, 0.5);
    SetArcsVisible(selected);
  }
}

bool                Waypoint::operator==(const Waypoint& other) const
{
  return (nodePath == other.nodePath);
}

bool                Waypoint::operator==(const Waypoint* other) const
{
  return (*this == *other);
}

Waypoint::Arcs::iterator Waypoint::Connect(Waypoint* other, ArcObserver* observer)
{
  Arcs::iterator  it = find(arcs.begin(), arcs.end(), other);

  if (it == arcs.end())
  {
    Arc arc_from(this, other);
    Arc arc_to(other, this);

    arc_from.observer = arc_to.observer = observer;
    arcs.push_back(arc_from);
    other->arcs.push_back(arc_to);
    return (--(arcs.end()));
  }
  return (it);
}

Waypoint::Arcs::iterator Waypoint::Disconnect(Waypoint* other)
{
  Arcs::iterator itSelf  = find(arcs.begin(), arcs.end(), other);
  Arcs::iterator itOther = find(other->arcs.begin(), other->arcs.end(), this);

  if (itSelf  != arcs.end())
  {
    itSelf->Destroy();
    itSelf = arcs.erase(itSelf);
  }
  if (itOther != other->arcs.end())
  {
    itOther->Destroy();
    other->arcs.erase(itOther);
  }
  return (itSelf);
}

void                Waypoint::DisconnectAll(void)
{
  Arcs::iterator it = arcs.begin();

  while (it != arcs.end())
  {
    Disconnect(it->to);
    it = arcs.begin();
  }
}

Waypoint::Arc*      Waypoint::GetArcTo(unsigned int id)
{
  Arcs::iterator it = arcs.begin();

  while (it != arcs.end())
  {
    if ((*it).to->id == id)
      return (&(*it));
    ++it;
  }
  return (0);
}

float               Waypoint::GetDistanceEstimate(const Waypoint& other) const
{
  LPoint3 pos_1  = nodePath.get_pos();
  LPoint3 pos_2  = other.nodePath.get_pos();
  float   dist_x = pos_1.get_x() - pos_2.get_x();
  float   dist_y = pos_1.get_y() - pos_2.get_y();

  return (SQRT(dist_x * dist_x + dist_y * dist_y));
}

list<Waypoint*> Waypoint::GetSuccessors(Waypoint* parent)
{
  list<Waypoint*> successors;

  Arcs::iterator it  = arcs.begin();
  Arcs::iterator end = arcs.end();

  for (; it != end ; ++it)
  {
      Arc& arc = *it;

      if (parent == arc.to)
        continue ;
      if (arc.CanGoThrough(Pathfinding::current_user) == false)
        continue ;
      successors.push_back(arc.to);
  }
  return (successors);
}

void Waypoint::PositionChanged()
{
  Arcs::iterator it  = arcs.begin();
  Arcs::iterator end = arcs.end();

  for (; it != end ; ++it)
  {
      (*it).UpdateDirection();
      (*it).to->UpdateArcDirection(this);
  }
}

void Waypoint::UpdateArcDirection(Waypoint* to)
{
  Arcs::iterator it = find(arcs.begin(), arcs.end(), to);

  if (it != arcs.end())
    (*it).UpdateDirection();
}

#ifdef GAME_EDITOR
# define WAYPOINT_DEBUG
#endif

// WAYPOINTS ARCS
Waypoint::Arc::Arc(Waypoint* from, Waypoint* to) : from(from), to(to)
{
  observer = 0;
}

Waypoint::Arc::Arc(const Waypoint::Arc& arc) : from(arc.from), to(arc.to)
{
  csegment = arc.csegment;
  observer = arc.observer;
}

Waypoint::Arc::~Arc()
{
#ifdef WAYPOINT_DEBUG
  if (!(nodePath.is_empty()))
    nodePath.detach_node();
#endif
}

void Waypoint::Arc::SetVisible(bool set)
{
#ifdef WAYPOINT_DEBUG
  if (set && nodePath.is_empty())
  {
    csegment = new CollisionSegment();
    node     = new CollisionNode("waypointArc");
    node->set_into_collide_mask(CollideMask(0));
    node->set_from_collide_mask(CollideMask(0));
    node->add_solid(csegment);
    csegment->set_point_a(0, 0, 0);
    nodePath = from->nodePath.attach_new_node(node);
    nodePath.set_pos(0, 0, 0);
    UpdateDirection();
    nodePath.show();
  }
  else
  {
    nodePath.remove_node();
  }
#endif
}

void Waypoint::Arc::UpdateDirection(void)
{
  if (!(nodePath.is_empty()))
  {
    NodePath  other  = to->nodePath;
    NodePath  parent = nodePath.get_parent();
    LVecBase3 rot    = parent.get_hpr();
    LVector3  dir    = parent.get_relative_vector(other, other.get_pos() - parent.get_pos(other.get_parent()));

    nodePath.set_scale(1 / parent.get_scale().get_x());
    nodePath.set_hpr(-rot.get_x(), -rot.get_y(), -rot.get_z());
    csegment->set_point_b(dir);
  }
}

void Waypoint::Arc::Destroy(void)
{
#ifdef WAYPOINT_DEBUG
  if (!(nodePath.is_empty()))
    nodePath.detach_node();
#endif
}

void Waypoint::SetArcsVisible(bool set)
{
  for_each(arcs.begin(), arcs.end(), [set](Waypoint::Arc& arc)
  {
    arc.SetVisible(set);
  });
}

void Waypoint::WithdrawArc(Waypoint* other)
{
  ArcsWithdrawed::iterator it, end;

  for (it = arcs_withdrawed.begin(), end = arcs_withdrawed.end() ; it != end ; ++it)
  {
    const Arc& arc = (*it).first;

    if (arc.to->id == other->id)
    {
      if (GetArcTo(arc.to->id))
        Disconnect(arc.to);
      (*it).second++;
      break ;
    }
  }
}

void Waypoint::UnwithdrawArc(Waypoint* other)
{
  ArcsWithdrawed::iterator it, end;

  for (it = arcs_withdrawed.begin(), end = arcs_withdrawed.end() ; it != end ; ++it)
  {
    const Arc& arc = (*it).first;

    if (arc.to == other)
    {
      if ((*it).second != 0)
        (*it).second--;
      if ((*it).second == 0)
      {
        pair<Waypoint::Arc, unsigned short>* withdrawable = arc.to->GetWithdrawable(this);

        if (!withdrawable)
          break ;
        if (withdrawable && withdrawable->second == 0)
          Connect(arc.to, withdrawable->first.observer);
      }
      break ;
    }
  }
}

pair<Waypoint::Arc, unsigned short>* Waypoint::GetWithdrawable(Waypoint* other)
{
  ArcsWithdrawed::iterator it, end;

  for (it = arcs_withdrawed.begin(), end = arcs_withdrawed.end() ; it != end ; ++it)
  {
    const Arc& arc = (*it).first;

    if (arc.to->id == other->id)
      return (&(*it));
  }
  return (0);
}

Waypoint* Waypoint::GetRandomWaypoint(void) const
{
#ifndef GAME_EDITOR
  if (arcs_withdrawed.size() > 0)
  {
    ArcsWithdrawed::const_iterator it  = arcs_withdrawed.begin();
    unsigned int                   i   = Dices::Throw(arcs_withdrawed.size() - 1);

    std::advance(it, i);
    return ((*it).first.to);
  }
#endif
  return (0);
}

/*
 * Serialization
 */
void Waypoint::Unserialize(Utils::Packet &packet)
{
  float            posx, posy, posz;

  packet >> (int&)(id);
  packet >> posx >> posy >> posz;
  packet >> floor;
  packet >> tmpArcs;

  nodePath.set_pos(posx, posy, posz);
}

void Waypoint::LoadArcs(void)
{
  std::for_each(arcs.begin(), arcs.end(), [this](Arc& arc)
  {
    arcs_withdrawed.push_back(std::pair<Arc, unsigned short>(arc, 0));
  });
}

void Waypoint::UnserializeLoadArcs(World* world)
{
  vector<int>::iterator it  = tmpArcs.begin();
  vector<int>::iterator end = tmpArcs.end();

  for (; it != end ; ++it)
  {
    Waypoint* waypoint = world->GetWaypointFromId((*it));

    if (waypoint)
      Connect(waypoint);
  }
  LoadArcs();
  tmpArcs.clear();
}

void Waypoint::Serialize(World* world, Utils::Packet &packet)
{
  LPoint3f         pos = nodePath.get_pos(world->window->get_render());
  int              id  = this->id;
  float            posx, posy, posz;
  vector<int>      arcs;

  posx = pos.get_x();
  posy = pos.get_y();
  posz = pos.get_z();

  Arcs::iterator it  = this->arcs.begin();
  Arcs::iterator end = this->arcs.end();

  for (; it != end ; ++it)
    arcs.push_back((*it).to->id);

  packet << id;
  packet << posx << posy << posz;
  packet << floor;
  packet << arcs;
}
