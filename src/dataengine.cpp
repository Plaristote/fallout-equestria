#include "dataengine.hpp"
#include "character.hpp"
#include "json.hpp"

void      DataEngine::Load(const std::string& filepath)
{
  if (_dataTree)
    delete _dataTree;
  _dataTree = DataTree::Factory::JSON(filepath);
  _data     = _dataTree;
}

void      DataEngine::Save(const std::string& filepath)
{
  DataTree::Writers::JSON(*this, filepath);
}
