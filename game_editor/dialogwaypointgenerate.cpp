#include "dialogwaypointgenerate.h"
#include "ui_dialogwaypointgenerate.h"

#include "functorthread.h"
#include "qpandaapplication.h"
#include "world.h"

using namespace std;

WaypointGenerator::WaypointGenerator(World* world, MapObject* object, LPoint4 margin, LPoint2 spacing) : world(world), object(object)
{
  LPoint3f size = NodePathSize(object->nodePath);

  spacingx = spacing.get_x();
  spacingy = spacing.get_y();
  initPosX = margin.get_x();
  initPosY = margin.get_y();
  initPosZ = size.get_z() + 50.f;
  sizex    = (size.get_x() - (margin.get_x() + margin.get_z())) / spacingx + 1;
  sizey    = (size.get_y() - (margin.get_y() + margin.get_w())) / spacingy + 1;
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
      float posx = initPosX + (spacingx * x);
      float posy = initPosY + (spacingy * y);
      float posz = initPosZ;
      waypoints[x][y] = world->AddWayPoint(posx, posy, posz);
      waypoints[x][y]->nodePath.reparent_to(object->waypoints_root);
      object->waypoints.push_back(waypoints[x][y]);
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

    cout << "Looking for collisions" << endl;
    col_traverser.add_collider(np, col_queue);
    col_traverser.traverse(object->nodePath);
    if (col_queue->get_num_entries())
    {
      LPoint3 np_size = NodePathSize(wp);

      col_queue->sort_entries();
      min_pos    = col_queue->get_entry(0)->get_surface_point(object->nodePath);
      new_height = min_pos.get_z() + (np_size.get_y() / 2);
      cout << "found a collision" << endl;
      cout << min_pos.get_x() << ", " << min_pos.get_y() << ", " << min_pos.get_z() << endl;
    }
    np.remove_node();
    if (new_height != wp.get_z())
    {
      wp.set_z(new_height);
      return (true);
    }
    return (false);
  }
}

void WaypointGenerator::Leveling(void)
{
  float step  = 0;
  float steps = object->waypoints.size();
  auto  it    = object->waypoints.begin();
  auto  end  = object->waypoints.end();

  while (it != end)
  {
    unsigned int attempts = 0;
    Waypoint*    wp       = *it;
    NodePath     np       = wp->nodePath;

    while (LevelWaypoint(wp) == false && attempts < 20)
    {
      if (attempts % 2)
        np.set_x(np.get_x() + 0.01);
      else
        np.set_y(np.get_y() + 0.01);
      ++attempts;
    }
    if (attempts == 20)
    {
      world->DeleteWayPoint(wp);
      it = object->waypoints.erase(it);
    }
    else
      ++it;
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
