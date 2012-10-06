#include "dataengine.hpp"
#include "json.hpp"

DataEngine::~DataEngine()
{
  delete _dataTree;
  _dataTree = 0;
  _data     = 0;
}

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
