#include "dialogwaypointgenerate.h"
#include "ui_dialogwaypointgenerate.h"

#include "functorthread.h"
#include "qpandaapplication.h"
#include "world/world.h"

using namespace std;

WaypointGenerator::WaypointGenerator(World* world, MapObject* object, LPoint4 margin, LPoint2 spacing, bool corner_origin) : world(world), object(object)
{
  LPoint3f size = NodePathSize(object->render);

  spacingx = spacing.get_x();
  spacingy = spacing.get_y();
  sizex    = (size.get_x() - (margin.get_x() + margin.get_z())) / spacingx + 1;
  sizey    = (size.get_y() - (margin.get_y() + margin.get_w())) / spacingy + 1;
  if (corner_origin)
  {
    initPosX = -(margin.get_x() - size.get_x());
    initPosY = -(margin.get_y() - size.get_y());
    spacingx = -spacingx;
    spacingy = -spacingy;
  }
  else
  {
    initPosX = margin.get_x() - (size.get_x() / 2);
    initPosY = margin.get_y() - (size.get_y() / 2);
  }
  initPosZ = size.get_z() + 50.f;
}

void WaypointGenerator::ClearObject(void)
{
  int i = 0;

  std::for_each(object->waypoints.begin(), object->waypoints.end(), [this, &i](Waypoint* wp)
  {
    world->DeleteWayPoint(wp);
    UpdateProgress("Waypoint Clearing (1/4): %p%", i / object->waypoints.size() * 100);
    i++;
  });
  object->waypoints.clear();
}

void WaypointGenerator::Generating(WaypointGrid& waypoints)
{
  float step = 0;

  for (unsigned int x = 0 ; x < sizex ; ++x)
  {
    for (unsigned int y = 0 ; y < sizey ; ++y)
    {
      NodePath render = world->window->get_render();
      float    posx   = initPosX + (spacingx * x);
      float    posy   = initPosY + (spacingy * y);
      float    posz   = initPosZ;
      Waypoint* wp    = world->AddWayPoint(posx, posy, posz);

      waypoints[x][y] = wp;
      wp->nodePath.reparent_to(object->waypoints_root);
      object->waypoints.push_back(wp);
      wp->nodePath.set_pos  (render, object->nodePath.get_pos(render) + wp->nodePath.get_pos());
      wp->nodePath.set_hpr  (render, wp->nodePath.get_hpr());
      wp->nodePath.set_scale(render, wp->nodePath.get_scale());
      UpdateProgress("Waypoint Generation (2/4): %p%", ++step / (sizex * sizey) * 100);
    }
  }
}

void WaypointGenerator::Linking(WaypointGrid& waypoints)
{
  float step = 0;

  for (unsigned int x = 0 ; x < sizex ; ++x)
  {
    for (unsigned int y = 0 ; y < sizey ; ++y)
    {
      if (x > 0)
        waypoints[x][y]->Connect(waypoints[x - 1][y]);
      if (x + 1 < sizex)
        waypoints[x][y]->Connect(waypoints[x + 1][y]);
      if (y > 0)
        waypoints[x][y]->Connect(waypoints[x][y - 1]);
      if (y + 1 < sizey)
        waypoints[x][y]->Connect(waypoints[x][y + 1]);
      if (x > 0 && y > 0)
        waypoints[x][y]->Connect(waypoints[x - 1][y - 1]);
      if (x + 1 < sizex && y > 0)
        waypoints[x][y]->Connect(waypoints[x + 1][y - 1]);
      if (x > 0 && y + 1 < sizey)
        waypoints[x][y]->Connect(waypoints[x - 1][y + 1]);
      if (x + 1 < sizex && y + 1 < sizey)
        waypoints[x][y]->Connect(waypoints[x + 1][y + 1]);
      UpdateProgress("Waypoint Linking (3/4): %p%", ++step / (sizex * sizey) * 100);
    }
  }
}

bool WaypointGenerator::LevelWaypoint(Waypoint* waypoint)
{
  NodePath np;
  NodePath wp = waypoint->nodePath;
  LPoint3  min_pos;
  float    new_height = wp.get_z();

  {
    CollisionTraverser        col_traverser;
    PT(CollisionHandlerQueue) col_queue = new CollisionHandlerQueue;
    PT(CollisionNode)         cnode     = new CollisionNode("waypoint");
    PT(CollisionSegment)      segment   = new CollisionSegment;

    cnode->set_from_collide_mask(CollideMask(ColMask::Object));
    cnode->add_solid(segment);
    np = object->nodePath.attach_new_node(cnode);
    segment->set_point_a(wp.get_x(), wp.get_y(), wp.get_z());
    segment->set_point_b(wp.get_x(), wp.get_y(), wp.get_z() - (100000.f));

    col_traverser.add_collider(np, col_queue);
    col_traverser.traverse(object->render);
    if (col_queue->get_num_entries())
    {
      LPoint3 np_size = NodePathSize(wp);

      col_queue->sort_entries();
      min_pos    = col_queue->get_entry(0)->get_surface_point(object->nodePath);
      new_height = min_pos.get_z() + (np_size.get_y() / 2);
    }
    np.remove_node();
    if (new_height != wp.get_z(object->nodePath))
    {
      wp.set_z(object->nodePath, new_height);
      return (true);
    }
    return (false);
  }
}

void WaypointGenerator::Leveling(void)
{
  float          step           = 0;
  float          steps          = object->waypoints.size();

  for (unsigned int it = 0 ; it < object->waypoints.size() ; ++it)
  {
    unsigned int attempts = 0;
    Waypoint*    wp       = object->waypoints[it];
    NodePath     np       = wp->nodePath;

    while (LevelWaypoint(wp) == false && attempts < 3)
    {
      if (attempts % 2)
        np.set_x(np.get_x() + 0.01);
      else
        np.set_y(np.get_y() + 0.01);
      ++attempts;
    }
    if (attempts == 3)
    {
      it--;
      world->DeleteWayPoint(wp);
    }
    UpdateProgress("Waypoint Leveling (4/4): %p%", ++step / steps * 100);
  }
}

void WaypointGenerator::Run(void)
{
  thread = FunctorThread::Create([this](void)
  {
    vector<vector<Waypoint*> > waypoints;

    waypoints.resize(sizex);
    for (unsigned int i = 0 ; i < sizex ; ++i)
      waypoints[i].resize(sizey);

    ClearObject();
    Generating(waypoints);
    Linking(waypoints);
    Leveling();
    Done();
    QPandaApplication::SetPandaEnabled(true);
  });
  QPandaApplication::SetPandaEnabled(false);
  Started();
  thread->start();
}

DialogWaypointGenerate::DialogWaypointGenerate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWaypointGenerate)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SIGNAL(Generate()));
}

DialogWaypointGenerate::~DialogWaypointGenerate()
{
    delete ui;
}

LPoint4 DialogWaypointGenerate::GetMargin(void) const
{
  return (LPoint4(ui->margin_left->value(),
                  ui->margin_bottom->value(),
                  ui->margin_right->value(),
                  ui->margin_top->value()));
}

LPoint2 DialogWaypointGenerate::GetSpacing(void) const
{
  return (LPoint2(ui->spacing_x->value(),
                  ui->spacing_y->value()));
}

bool DialogWaypointGenerate::IsOriginCorner(void) const
{
  return (ui->originCorner->isChecked());
}
