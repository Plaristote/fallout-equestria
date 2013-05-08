#include <panda3d/pandaFramework.h>
#include <panda3d/pandaSystem.h>

#include <panda3d/texturePool.h>
#include <panda3d/directionalLight.h>

PandaFramework       framework;
PT(AsyncTaskManager) taskMgr     = AsyncTaskManager::get_global_ptr();
PT(ClockObject)      globalClock = ClockObject::get_global_clock();

# include "mainmenu.hpp"
# include "options.hpp"

using namespace std;

#include <panda3d/load_prc_file.h>
#include <panda3d/graphicsStateGuardianBase.h>
#include <panda3d/pStatClient.h>
#include <panda3d/audioManager.h>

# include "scriptengine.hpp"

#include "serializer.hpp"
#include <i18n.hpp>
#include <level/objects/shelf.hpp>
#include <level/objects/locker.hpp>
#include <level/divide_and_conquer.hpp>

#include "musicmanager.hpp"
#include <soundmanager.hpp>
#include <dices.hpp>

template<typename TYPE>
struct RecursiveChecker
{
  typedef std::queue<RecursiveChecker>     Queue;
  typedef std::function<bool (TYPE, TYPE)> CompFunc;
  
  RecursiveChecker(TYPE first, TYPE second, CompFunc comp) : first(first), second(second), comp(comp)
  {
  }
  
  bool Execute(std::queue<RecursiveChecker> checkers)
  {
    if (first == second)
    {
      checkers.pop();
      if (!(checkers.empty()))
        return (checkers.front().Execute(checkers));
      return (true);
    }
    return (comp(first, second));
  }

  TYPE                             first, second;
  std::function<bool (TYPE, TYPE)> comp;
};

class DateTime
{
public:
  
  bool operator>(const DateTime& time)
  {
    RecursiveChecker<int>::CompFunc comp([](int a, int b) { return (a < b); });

    return (recursive_compare(comp, time));
  }
  
  bool operator<(const DateTime& time)
  {
    RecursiveChecker<int>::CompFunc comp([](int a, int b) { return (a > b); });

    return (recursive_compare(comp, time));
  }
  
private:
  bool recursive_compare(RecursiveChecker<int>::CompFunc comp, const DateTime& time)
  {
    RecursiveChecker<int>::Queue    checkers;

    checkers.push(RecursiveChecker<int>(year,   time.year,   comp));
    checkers.push(RecursiveChecker<int>(month,  time.month,  comp));
    checkers.push(RecursiveChecker<int>(day,    time.day,    comp));
    checkers.push(RecursiveChecker<int>(hour,   time.hour,   comp));
    checkers.push(RecursiveChecker<int>(minute, time.minute, comp));
    checkers.push(RecursiveChecker<int>(second, time.second, comp));    
    return (checkers.front().Execute(checkers));
  }
  
  unsigned short year;
  unsigned char  month, day, hour, minute, second;
};

string humanize(const std::string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if (i == 0 || str[i - 1] == '_')
      ret += str[i] - 'a' + 'A';
    else if (str[i] == '_')
      ret += ' ';
    else
      ret += str[i];
  }
  return (ret);
}

string underscore(const std::string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if      (str[i] >= 'A' && str[i] <= 'Z')
      ret += str[i] - 'A' + 'a';
    else if (str[i] == ' ')
      ret += '_';
    else
      ret += str[i];
  }
  return (ret);
}

void AngelScriptInitialize(void);
int  compile_statsheet(std::string);

#include <panda3d/eggData.h>
#include <panda3d/eggGroupNode.h>
#include <panda3d/eggGroup.h>
#include <panda3d/eggPolygon.h>
#include <boost/concept_check.hpp>

#define MAX_3(a, b, c) (a > b && a > c ? a : (b >= a && b > c ? b : c))

#define until(condition)  while (!(condition))
#define unless(condition) if (!(condition))

namespace DivideAndConquer
{
  struct Maps
  {
  private:
    struct Polygon
    {
      Polygon(EggPolygon* polygon) : polygon(polygon)
      {
        LVertexd poly_center(0, 0, 0);

        for (unsigned short i = 0 ; i < 3 ; ++i)
        {
          LVertexd pos3 = polygon->get_vertex(i)->get_pos3();

          poly_center += pos3;
        }
        poly_center /= 3.f;
        position.set_x(poly_center.get_x());
        position.set_y(poly_center.get_y());
        position.set_z(poly_center.get_z());
      }

      LPoint3f GetPosition(void) const
      {
        return (position);
      }

      PT(EggNode) polygon;
      LPoint3f    position;
    };

  public:
    static void Build(const std::string& source_path, const std::string& dest)
    {
      PT(EggData)              egg_source = new EggData();
      PT(EggData)              egg_target = new EggData();
      PT(EggVertexPool)        vertex_pool;
      Graph<Polygon, LPoint3f> graph;
      std::vector<Polygon*>    polygons;
      LPoint3f                 minBB, maxBB;

      egg_source->read(source_path);
      egg_source->get_first_child();

      cout << ":: Triangulating polygons" << endl;
      egg_source->triangulate_polygons(EggGroupNode::T_polygon | EggGroupNode::T_convex);
      cout << ">> Done" << endl;

      egg_target->set_coordinate_system(egg_source->get_coordinate_system());      

      std::function<void (EggGroup*)> recursive_fetch_node = [&recursive_fetch_node, &polygons](EggGroup* group)
      {
        PT(EggNode) node = group->get_first_child();

        until (node.is_null())
        {
          if (node->get_type().get_name() == "EggGroup")
            recursive_fetch_node(reinterpret_cast<EggGroup*>(node.p()));
          else if (node->get_type().get_name() == "EggPolygon")
            polygons.push_back(new Polygon(reinterpret_cast<EggPolygon*>(node.p())));
          node = group->get_next_child();
        }
      };

      PT(EggNode) node = egg_source->get_next_child();

      cout << ":: Preparing the pool of polygons" << endl;
      until (node.is_null())
      {
        cout << "-> Node of type " << node->get_type().get_name() << endl;
        if (node->get_type().get_name() == "EggGroup")
          recursive_fetch_node(reinterpret_cast<EggGroup*>(node.p()));
        else
          egg_target->add_child(node.p());
        node = egg_source->get_next_child();
      }
      cout << ">> Done (" << polygons.size() << " polygons)" << endl;

      graph.SetHeuristic([](LPoint3f position1, LPoint3f position2) -> float
      {
        float   dist_x = position1.get_x() - position2.get_x();
        float   dist_y = position1.get_y() - position2.get_y();

        return (ABS(SQRT(dist_x * dist_x + dist_y * dist_y)));
      });
      graph.Initialize(polygons, [](const std::vector<Polygon*>& entries) -> std::vector<LPoint3f>
      {
        auto it       = entries.begin();
        auto end      = entries.end();
        bool min_init = false;
        bool max_init = false;
        LPoint3f min_pos, max_pos;

        for (; it != end ; ++it)
        {
          const Polygon& polygon = *(*it);
          const LPoint3f pos     = polygon.position;

          if (pos.get_x() < min_pos.get_x() || !min_init) { min_pos.set_x(pos.get_x()); }
          if (pos.get_y() < min_pos.get_y() || !min_init) { min_pos.set_y(pos.get_y()); }
          if (pos.get_z() < min_pos.get_z() || !min_init) { min_pos.set_z(pos.get_z()); }
          if (pos.get_x() > max_pos.get_x() || !max_init) { max_pos.set_x(pos.get_x()); }
          if (pos.get_y() > max_pos.get_y() || !max_init) { max_pos.set_y(pos.get_y()); }
          if (pos.get_z() > max_pos.get_z() || !max_init) { max_pos.set_z(pos.get_z()); }
          min_init = max_init = true;
        }
        {
          std::vector<LPoint3f> positions;
          LPoint3f              block_size;

          function<float (float, float)> distance = [](float min_pos, float max_pos) -> float
          {
            if (min_pos < 0 && max_pos > 0)
              return (ABS(min_pos - max_pos));
            return (ABS(max_pos) - ABS(min_pos));
          };

          block_size.set_x(distance(min_pos.get_x(), max_pos.get_x()));
          block_size.set_y(distance(min_pos.get_y(), max_pos.get_y()));
          block_size.set_z(distance(min_pos.get_z(), max_pos.get_z()));

          unsigned short block_count = 5;
          for (unsigned short i = 0 ; i < block_count ; ++i)
          {
            LPoint3f block_position;

            block_position.set_x(min_pos.get_x() + block_size.get_x() / block_count * i);
            block_position.set_y(min_pos.get_y() + block_size.get_y() / block_count * i);
            block_position.set_z(min_pos.get_z() + block_size.get_z() / block_count * i);
            positions.push_back(block_position);
          }
          return (positions);
        }
      });

      std::function<void (Graph<Polygon, LPoint3f>::Node*, EggGroup*)> initialize_egg = [&initialize_egg](Graph<Polygon, LPoint3f>::Node* node, EggGroup* group)
      {
        for (unsigned int i = 0 ; i < node->entries.size() ; ++i)
          group->add_child(node->entries[i]->polygon);
        for (unsigned int i = 0 ; i < node->nodes.size() ; ++i)
        {
          PT(EggGroup) sub_group = new EggGroup("Divide&Conquer Container");

          initialize_egg(node->nodes[i], sub_group);
          group->add_child(sub_group);
        }
      };

      auto node_tree = graph.node_tree;
      PT(EggGroup) root_group = new EggGroup("Divide&Conquer Root Node");
      initialize_egg(&node_tree, root_group);
      
      egg_target->add_child(root_group);
      egg_target->write_egg(dest);

      cout << "DONE" << endl;

    }
  };
}

struct Egg2EggOctree
{
  struct Cell
  {
    LPoint3f         min, max;
    PT(EggGroupNode) group;
  };

  typedef std::vector<std::vector<std::vector<Cell> > > Cells;
public:
  static void Build(const std::string& source_path, const std::string& dest, LPoint3f cell_size, bool collide = false)
  {
    Egg2EggOctree(source_path, dest, cell_size, collide);
  }

private:
  Egg2EggOctree(const std::string& source_path, const std::string& dest, LPoint3f cell_size, bool collide)
  {
    std::cout << "[EggOctree] Processing '" << source_path << "'. Output: '" << dest << '\'' << std::endl;
    PT(EggData)       egg        = new EggData();
    PT(EggData)       egg_target = new EggData();
    PT(EggGroupNode)  source;
    PT(EggVertexPool) vertex_pool;
    LPoint3f          minBB(0, 0, 0), maxBB(0, 0, 0);
    int               n_vertexes;

    std::cout << ":: Loading egg file" << std::endl;
    egg->read(source_path);
    std::cout << "> Done" << std::endl;
    egg->get_first_child();
    {
      PT(EggNode) node = egg->get_next_child();

      if (node->get_type().get_name() == "EggVertexPool")
      {
        std::cout << "Node is an EggVertexPool" << std::endl;
        egg->triangulate_polygons(EggGroupNode::T_polygon | EggGroupNode::T_convex);
        vertex_pool = reinterpret_cast<EggVertexPool*>(node.p());
        source      = egg.p();
      }
      else if (node->get_type().get_name() == "EggGroupNode")
      {
        std::cout << "Node is an EggGroupNode" << std::endl;
        source = reinterpret_cast<EggGroupNode*>(node.p());
      }
      else
      {
        std::cout << "Expected EggGroupNode or EggVertexPool. Found " << node->get_type().get_name() << std::endl;
        return ;
      }
    }
    egg_target->set_coordinate_system(egg->get_coordinate_system());
    if (vertex_pool.is_null())
    {
      std::cout << ":: Triangulating polygons" << std::endl;
      source->triangulate_polygons(EggGroupNode::T_polygon | EggGroupNode::T_convex);
      std::cout << "> Done" << std::endl;
      vertex_pool = reinterpret_cast<EggVertexPool*>(source->get_first_child());
    }
    n_vertexes  = vertex_pool->get_highest_index();
    bool min_init = false;
    bool max_init = false;
    for (int i = 0 ; i < n_vertexes ; ++i)
    {
      PT(EggVertex) vertex     = vertex_pool->get_vertex(i);
      LVertexd      vertex_pos = vertex->get_pos3();

      // Set the X, Y or Z components
      #define SET_XYZ(coord) \
        if (vertex_pos.get_##coord() < minBB.get_##coord() || min_init == false) \
        { minBB.set_##coord(vertex_pos.get_##coord()); min_init = true; } \
        if (vertex_pos.get_##coord() > maxBB.get_##coord() || max_init == false) \
        { maxBB.set_##coord(vertex_pos.get_##coord()); max_init = true; }
      SET_XYZ(x)
      SET_XYZ(y)
      SET_XYZ(z)
      #undef  SET_XYZ
    }
    minBB -= LPoint3f(0.001, 0.001, 0.001);
    maxBB += LPoint3f(0.001, 0.001, 0.001);
    {
      LPoint3f bbox_size = maxBB - minBB;

      ncx       = ceil(bbox_size.get_x());
      ncy       = ceil(bbox_size.get_y());
      ncz       = ceil(bbox_size.get_z());
      depth     = MAX_3(ceil(log(ncx)) / log(2), ceil(log(ncy)) / log(2), ceil(log(ncz)) / log(2));

      cout << "Depth is " << depth << endl;
      cells.resize(ncx);
      for (int x = 0 ; x < ncx ; ++x)
      {
        cells[x].resize(ncy);
        for (int y = 0 ; y < ncy ; ++y)
        {
          cells[x][y].resize(ncz);
          for (int z = 0 ; z < ncz ; ++z)
          {
            /*Cell              cell;
            std::stringstream group_name;

            cell.min = LPoint3f(minBB.get_x() + x       * cell_size.get_x(),
                                minBB.get_y() + y       * cell_size.get_y(),
                                minBB.get_z() + z       * cell_size.get_z());
            cell.max = LPoint3f(minBB.get_x() + (x + 1) * cell_size.get_x(),
                                minBB.get_y() + (y + 1) * cell_size.get_y(),
                                minBB.get_z() + (z + 1) * cell_size.get_z());
            //group_name << "leaf_" << x << '_' << y << '_' << z;
            cell.group = new EggGroupNode(group_name.str());
            cells[x][y][z] = cell;
            cout << "\r";*/
            //cout << "Generated node group " << group_name.str() << " (depth is " << ncx << ',' << ncy << ',' << ncz << ')' << endl;
          }
        }
        //cout << "Whatever -> " << x << " over " << ncx;
      }
      //cout << "Done generating whatever that was I just generated" << endl;

      if (collide)
      {
        for (int x = 0 ; x < ncx ; ++x)
        {
          for (int y = 0 ; y < ncy ; ++y)
          {
            for (int z = 0 ; z < ncz ; ++z)
            {
              // TODO Find out what should be happening here. What is this addObjectType method ??
              // cells[x][y][z].group->add_object_type("barrier");
            }
          }
        }

        // Iterate on the <Polygon>s (should be triangles)
        PT(EggNode) node = source->get_next_child();
        until (node.is_null())
        {
          if (node->is_of_type(EggPolygon::get_class_type()))
          {
            cout << "FOUND A POLYGON ! BULLSHIT !" << endl;
            PT(EggPolygon) poly = reinterpret_cast<EggPolygon*>(node.p());

            //
            // Get the triangle center
            //
            LVertexd poly_center(0, 0, 0);

            for (unsigned short i = 0 ; i < 3 ; ++i)
            {
              LVertexd pos3 = poly->get_vertex(i)->get_pos3();

              poly_center += pos3;
            }
            poly_center /= 3.f;

            //
            // Add the triangle to the corresponding cell group
            //
            #define INIT_CXYZ(coord) floor(poly_center.get_##coord() - minBB.get_##coord()) / cell_size.get_##coord()
            int cx = INIT_CXYZ(x);
            int cy = INIT_CXYZ(y);
            int cz = INIT_CXYZ(z);
            #undef  INIT_CXYZ
            if (cells[cx][cy][cz].group.is_null())
              cells[cx][cy][cz].group = new EggGroupNode("");
            cells[cx][cy][cz].group->add_child(poly);
          }
          node = source->get_next_child();
        }
        egg_target->add_child(vertex_pool);
        n_leaves = Recur(egg_target.p(), 0, 0, 0, 0);
        std::cout << n_leaves << " leaves added" << std::endl;
      }
    }
    cout << ":: Triangulating polygons" << endl;
    std::cout << ":: Generating output file" << std::endl;
    egg_target->write_egg(dest);
    std::cout << "> Done" << std::endl;
  }

  template<typename T>
  int Recur(T* node, int depth, int x, int y, int z)
  {
    if (depth < this->depth)
    {
      PT(EggGroup) nnode    = new EggGroup("");
      int          delt     = pow(2, this->depth - depth - 1);
      int          n_childs = 0;

      n_childs += Recur(nnode.p(), depth + 1, x, y, z);
      n_childs += Recur(nnode.p(), depth + 1, x + delt, y, z);
      n_childs += Recur(nnode.p(), depth + 1, x, y + delt, z);
      n_childs += Recur(nnode.p(), depth + 1, x + delt, y + delt, z);
      n_childs += Recur(nnode.p(), depth + 1, x, y, z + delt);
      n_childs += Recur(nnode.p(), depth + 1, x + delt, y, z + delt);
      n_childs += Recur(nnode.p(), depth + 1, x, y + delt, z + delt);
      n_childs += Recur(nnode.p(), depth + 1, x + delt, y + delt, z + delt);
      if (n_childs > 0)
        node->add_child(nnode);
      return (n_childs);
    }
    // We are in a leaf
    if (x < ncx && y < ncy && z < ncz)
    {
      if (cells[x][y][z].group.is_null())
        cells[x][y][z].group = new EggGroupNode("");
      node->add_child(cells[x][y][z].group);
      return (1);
    }
    return (0);
  }
  
  Cells cells;
  int   n_leaves;
  int   depth;
  int   ncx, ncy, ncz;
};

#include <panda3d/geoMipTerrain.h>
void Earthscultor2Bam(Data heightmap, const std::string& out)
{
  NodePath terrain;

  GeoMipTerrain* mip_terrain = new GeoMipTerrain("Terrain");

  mip_terrain->set_heightfield(heightmap["heightmap"].Value());
  mip_terrain->set_block_size(heightmap["block_size"]);
  mip_terrain->set_factor(heightmap["factor"]);
  mip_terrain->set_min_level(heightmap["min_level"]);
  
  mip_terrain->get_root().set_sz(heightmap["max_height"]);
  
  mip_terrain->generate();
  mip_terrain->get_root().write_bam_file(out);

  delete mip_terrain;
}

int compile_heightmap(const std::string& sourcefile, const std::string& out)
{
  DataTree* datatree = DataTree::Factory::JSON(sourcefile);

  if (datatree)
  {
    Earthscultor2Bam(datatree, out);
    delete datatree;
    return (0);
  }
  return (-1);
}

#ifndef UNIT_TESTER
int main(int argc, char *argv[])
{
  Dices::Initialize();          // Randomness initialization
  Script::Engine::Initialize(); // Script Engine initialization (obviously)
  AngelScriptInitialize();      // Registering script API (see script_api.cpp)

  // With some options, game binary can also be used to compile statsheet or heightmaps.
  // If used as compiler of some sort
  if (argc == 3 && std::string(argv[1]) == "--compile-statsheet")
    return (compile_statsheet(argv[2]));
  if (argc == 4 && std::string(argv[1]) == "--compile-heightmap")
    return (compile_heightmap(argv[2], argv[3]));
  if (argc == 4 && std::string(argv[1]) == "--egg-octree")
  {
    DivideAndConquer::Maps::Build(argv[2], argv[3]);
    //Egg2EggOctree::Build(argv[2], argv[3], LPoint3f(50, 50, 50), true);
    return (0);
  }
  // Otherwise run the game
  {
    WindowFramework* window;
    ConfigPage*      config = load_prc_file("config.prc");

    if (!(PStatClient::connect("localhost", 5185))) // Initialize client for profiling collector
      cout << "Can't connect to PStat client" << endl;
    framework.open_framework(argc, argv);
    framework.set_window_title("Fallout Equestria");
    window = framework.open_window();
    window->enable_keyboard();
    window->get_render().set_shader_auto();

    // Set Windows Properties
    {
      WindowProperties props = window->get_graphics_window()->get_properties();

      props.set_cursor_hidden(true);
      props.set_icon_filename("data/icon.ico");
      window->get_graphics_window()->request_properties(props);
    }

    OptionsManager::Initialize(); // Loads and handle conf.json file
    {
      MainMenu       mainMenu(window); // MainMenu will take over as a Panda3D async_task.

      framework.main_loop();
      framework.close_framework();

      unload_prc_file(config);
    }
    OptionsManager::Finalize();
    Script::Engine::Finalize();
  }
  return (0);
}
#endif

