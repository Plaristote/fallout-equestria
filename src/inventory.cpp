#include "inventory.hpp"
#include <algorithm>

InventoryObject::InventoryObject(Data data) : Data(&_dataTree)
{
  (*this)["texture"]  = data["texture"].Value();
  (*this)["model"]    = data["model"].Value();
  (*this)["scale"]    = data["scale"].Value();
  (*this)["pos"]["x"] = data["pos"]["x"].Value();
  (*this)["pos"]["y"] = data["pos"]["y"].Value();
  (*this)["interactions"]["use"] = "1";
}

void Inventory::AddObject(InventoryObject* toAdd)
{
  _content.push_back(toAdd);
}

void Inventory::DelObject(InventoryObject* toDel)
{
  Content::iterator it = std::find(_content.begin(), _content.end(), toDel);

  if (it != _content.end())
    _content.erase(it);
}
