#include "level/waypoint_map.hpp"
#include <fstream>
#include <panda3d/geomTriangles.h>
using namespace std;

struct Triangle
{
  bool operator==(Triangle triangle) const
  {
    return (triangle.a == a && triangle.b == b);
  }

  LPoint3f a, b, c;
};

class Facets
{
public:
  typedef std::list<Triangle> Triangles;

  const Triangles& GetTriangles(void) const
  {
    return (triangles);
  }

  void AddFacet(Triangle triangle)
  {
    auto item = std::find(triangles.begin(), triangles.end(), triangle);

    if (item == triangles.end())
      triangles.push_back(triangle);
  }

private:
  Triangles triangles;
};

bool WaypointMap::Initialize(const std::vector<Waypoint>& waypoints, unsigned char floor)
{
  PT(GeomVertexData) vertex_data   = new GeomVertexData("wp-map", GeomVertexFormat::get_v3(), Geom::UH_static);
  PT(Geom)           geom          = new Geom(vertex_data);
  auto               it            = waypoints.begin();
  auto               end           = waypoints.end();
  bool               has_waypoints = false;
  Facets             facets;

  for (size_t i = 0 ; it != end ; ++it, ++i)
  {
    if (it->floor == floor)
    {
      const Waypoint& wp      = *it;
      auto            arc_it  = wp.arcs.begin();
      auto            arc_end = wp.arcs.end();

      for (; arc_it != arc_end ; ++arc_it)
      {
        const Waypoint::Arc&    arc            = *arc_it;
        auto                    remote_arc_it  = arc.to->arcs.begin();
        auto                    remote_arc_end = arc.to->arcs.end();

        for (; remote_arc_it != remote_arc_end ; ++remote_arc_it)
        {
          const Waypoint::Arc& remote_arc      = *remote_arc_it;
          bool                 add_facet       = (remote_arc.to->GetArcTo(wp.id) != 0);

          if (add_facet)
          {
            Triangle triangle;

            triangle.a = wp.nodePath.get_pos();
            triangle.b = arc.to->nodePath.get_pos();
            triangle.c = remote_arc.to->nodePath.get_pos();
            facets.AddFacet(triangle);
          }
        }
      }
      has_waypoints = true;
    }
  }
  if (has_waypoints)
  {
    GeomVertexWriter     geom_writer(vertex_data, "vertex");
    PT(GeomTriangles)    geom_triangles = new GeomTriangles(Geom::UH_static);
    {
      auto         triangle_it  = facets.GetTriangles().begin();
      auto         triangle_end = facets.GetTriangles().end();
      unsigned int writer_it    = 0;

      for (; triangle_it != triangle_end ; ++triangle_it)
      {
        const Triangle& triangle = *triangle_it;

        geom_writer.add_data3f(triangle.a.get_x(), triangle.a.get_y(), triangle.a.get_z());
        geom_writer.add_data3f(triangle.b.get_x(), triangle.b.get_y(), triangle.b.get_z());
        geom_writer.add_data3f(triangle.c.get_x(), triangle.c.get_y(), triangle.c.get_z());
        geom_triangles->add_consecutive_vertices(writer_it, writer_it + 2);
        geom_triangles->close_primitive();
        writer_it += 3;
      }
    }
    geom->add_primitive(geom_triangles);
    geom_node    = new GeomNode("wp-map");
    geom_node->add_geom(geom);
    waypoint_map = window->get_render().attach_new_node(geom_node);
    
    stringstream floor_model;

    floor_model << "waypoint_map#" << floor << ".bam";
    waypoint_map.write_bam_file(floor_model.str());
  }
  return (has_waypoints);
}
