#include <panda3d/geoMipTerrain.h>
#include "datatree.hpp"
#include <string>

LPoint3 NodePathSize(NodePath);

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

