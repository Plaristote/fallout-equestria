#include "dialogwaypointgenerate.h"
#include "ui_dialogwaypointgenerate.h"

#include "functorthread.h"
#include "qpandaapplication.h"

using namespace std;

DialogWaypointGenerate::DialogWaypointGenerate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWaypointGenerate)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(Generate()));
}

DialogWaypointGenerate::~DialogWaypointGenerate()
{
    delete ui;
}

void DialogWaypointGenerate::SetWorld(World *world)
{
    this->world = world;
}

void DialogWaypointGenerate::Generate()
{
  FunctorThread& thread = *FunctorThread::Create([this](void)
  {
    float step     = 0;
    float sizex    = ui->size_x->value();
    float sizey    = ui->size_y->value();
    float spacingx = ui->spacing_x->value();
    float spacingy = ui->spacing_y->value();
    float initPosX = ui->pos_x->value();
    float initPosY = ui->pos_y->value();
    float initPosZ = ui->pos_z->value();
    vector<vector<Waypoint*> > waypoints;

    to_select.clear();
    waypoints.resize(sizex);
    for (int i = 0 ; i < sizex ; ++i)
      waypoints[i].resize(sizey);

    for (int x = 0 ; x < sizex ; ++x)
    {
        for (int y = 0 ; y < sizey ; ++y)
        {
            float posx = initPosX + (spacingx * x);
            float posy = initPosY + (spacingy * y);
            float posz = initPosZ;

            waypoints[x][y] = world->AddWayPoint(posx, posy, posz);
            SigUpdateProgressbar("Waypoint Generation: %p%", ++step / (sizex * sizey * 2) * 100);
            to_select.push_back(waypoints[x][y]);
        }
    }

    for (int x = 0 ; x < sizex ; ++x)
    {
        for (int y = 0 ; y < sizey ; ++y)
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
            SigUpdateProgressbar("Waypoint Generation: %p%", ++step / (sizex * sizey * 2) * 100);
        }
    }
    EndedGeneration();
    QPandaApplication::SetPandaEnabled(true);
  });
  QPandaApplication::SetPandaEnabled(false);
  StartedGeneration();
  thread.start();
}
