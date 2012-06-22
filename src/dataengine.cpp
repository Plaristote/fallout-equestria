#include "dataengine.hpp"
#include "character.hpp"
#include "json.hpp"

void      DataEngine::Load(const std::string& filepath)
{
  _dataTree = DataTree::Factory::JSON(filepath);
  (Data)(*this) = Data(_dataTree);
}

void      DataEngine::Save(const std::string& filepath)
{
  DataTree::Writers::JSON(*this, filepath);
}
