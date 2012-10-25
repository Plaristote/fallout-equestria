#include "playerparty.hpp"
#include "level/world.h"
#include <fstream>

using namespace std;

PlayerParty::PlayerParty(const string& savepath)
{
  string        save_party = savepath + "/player-party.blob";
  ifstream      file(save_party.c_str());

  if (file.is_open())
  {
    Utils::Packet packet(file);

    UnSerialize(packet);
  }
  _local_objects = true;
}

PlayerParty::~PlayerParty()
{
  if (_local_objects)
    for_each(_objects.begin(), _objects.end(), [](DynamicObject* object) { delete object; });
}

bool PlayerParty::Save(const string& savepath) const
{
  string        path = savepath + "/player-party.blob";
  ofstream      file;
  Utils::Packet packet;

  file.open(path.c_str(), ios::binary);
  if (file.is_open())
  {
    Serialize(packet);
    file.write(packet.raw(), packet.size());
    file.close();
    return (true);
  }
  return (false);
}

void PlayerParty::Join(DynamicObject* object)
{
  if (object)
    _objects.push_back(object);
}

void PlayerParty::Leave(DynamicObject* object)
{
  DynamicObjects::iterator it = find(_objects.begin(), _objects.end(), object);

  _objects.erase(it);
}

void PlayerParty::Serialize(Utils::Packet& packet) const
{
  unsigned short size     = _objects.size();

  packet << size;
  for_each(_objects.begin(), _objects.end(), [&packet](const DynamicObject* object)
  {
    int  iType    = object->type;
    int  inv_size = object->inventory.size();
    auto it       = object->inventory.begin();
    auto end      = object->inventory.end();

    packet << (int)object->type << object->charsheet << object->dialog << object->interactions;
    packet << object->strModel << object->strTexture;
    packet << inv_size;
    for (; it != end ; ++it)
      packet << (*it).first << (*it).second;
  });
}

void PlayerParty::UnSerialize(Utils::Packet& packet)
{
  unsigned short size;

  packet >> size;
  for (unsigned int i = 0 ; i < size ; ++i)
  {
    DynamicObject* object = new DynamicObject;
    int            inv_size, type;

    packet >> type >> object->charsheet >> object->dialog >> object->interactions;
    packet >> object->strModel >> object->strTexture;
    packet >> inv_size;
    for (int i = 0 ; i < inv_size ; ++i)
    {
      string json_src;
      int    quantity;
      
      packet >> json_src >> quantity;
      object->inventory.push_back(pair<string, int>(json_src, quantity));
    }
    object->type = (DynamicObject::Type)type;
    _objects.push_back(object);
  }
}
