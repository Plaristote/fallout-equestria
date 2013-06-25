#include "dataengine.hpp"
#include "json.hpp"
#include "level/diplomacy.hpp"

DataEngine::DataEngine()
{
  _dataTree  = 0;
  _diplomacy = new WorldDiplomacy(*this);
}

DataEngine::~DataEngine()
{
  delete _diplomacy;
  delete _dataTree;
  _dataTree = 0;
  _data     = 0;
}

void      DataEngine::Load(const std::string& filepath)
{
  if (_dataTree)
    delete _dataTree;
  _dataTree = DataTree::Factory::JSON(filepath);
  if (!_dataTree)
    throw 0;
  _data     = _dataTree;
  _diplomacy->Initialize();
}

void      DataEngine::Save(const std::string& filepath)
{
  DataTree::Writers::JSON(*this, filepath);
}

WorldDiplomacy& DataEngine::GetDiplomacy(void)
{
  return (*_diplomacy);
}
