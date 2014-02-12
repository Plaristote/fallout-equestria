#include "world/world.h"
#include "playerparty.hpp"
#include "level/objectnode.hpp"
#include "gametask.hpp"
#include <fstream>

using namespace std;

void Party::Join(InstanceDynamicObject* object)
{
  Join(object->GetDynamicObject());
}

void Party::Leave(InstanceDynamicObject* object)
{
  Leave(object->GetDynamicObject());
}

void Party::Join(DynamicObject* object)
{
  if (object)
  {
    _objects.push_back(object);
    Updated.Emit();
  }
}

void Party::Leave(DynamicObject* object)
{
  DynamicObjects::iterator it = find(_objects.begin(), _objects.end(), object);

  if (it != _objects.end())
  {
    _objects.erase(it);
    Updated.Emit();
  }
}

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
  cout << "PlayerParty::Save" << endl;
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

void Party::Serialize(Utils::Packet& packet) const
{
  unsigned short size     = _objects.size();

  packet << size;
  for_each(_objects.begin(), _objects.end(), [&packet](const DynamicObject* object)
  {
    //int  iType    = object->type;
    int  inv_size = object->inventory.size();
    auto it       = object->inventory.begin();
    auto end      = object->inventory.end();

    packet << object->nodePath.get_name();
    packet << (int)object->type << object->charsheet << object->dialog << object->interactions;
    packet << object->strModel << object->strTexture;
    packet << inv_size;
    for (; it != end ; ++it)
      packet << (*it).first << (*it).second;
  });
}

void Party::UnSerialize(Utils::Packet& packet)
{
  unsigned short size;

  packet >> size;
  for (unsigned int i = 0 ; i < size ; ++i)
  {
    DynamicObject* object = new DynamicObject;
    int            inv_size, type;
    string         name;

    packet >> name;
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
    object->nodePath.set_name(name);
    object->type = (DynamicObject::Type)type;
    _objects.push_back(object);
  }
}

void PlayerParty::Create(const string& savepath, const string& name, const string& model, const string& texture)
{
  PlayerParty   party;
  DynamicObject player;
  
  player.nodePath      = NodePath(name);
  player.strModel      = model;
  player.strTexture    = texture;
  player.type          = DynamicObject::Character;
  party._local_objects = false;
  party._objects.push_back(&player);
  party.Save(savepath);
}

Party::Statsheets Party::GetStatsheets(void) const
{
  Statsheets statsheets;
  
  for_each(_objects.begin(), _objects.end(), [&statsheets](DynamicObject* object)
  {
    //statsheets.emplace(object->nodePath.get_name(), object->charsheet);
    statsheets.insert(Party::Statsheets::value_type(object->nodePath.get_name(), object->charsheet));
  });
  return (statsheets);
}

void Party::Export(const std::string& name) const
{
  cout << "Party::Export" << endl;
  const string& savepath = GameTask::CurrentGameTask->GetSavePath();
  string        path     = savepath + "/party-" + name + ".blob";
  ofstream      file;
  Utils::Packet packet;

  file.open(path.c_str(), ios::binary);
  if (file.is_open())
  {
    Serialize(packet);
    file.write(packet.raw(), packet.size());
    file.close();
  }
  ForEach(_objects, [savepath](DynamicObject* object)
  {
    ObjectCharacter* character = Level::CurrentLevel->GetCharacter(object);

    if (character)
      DataTree::Writers::JSON(character->GetStatistics(),
                              savepath + "/stats-" + object->charsheet + ".json");
  });
}

Party* Party::Import(const std::string& name)
{
  cout << "Party::Import" << endl;
  Level*        level    = Level::CurrentLevel;

  if (level)
  {
    auto it  = level->_parties.begin();
    auto end = level->_parties.end();

    while (it != end)
    {
      if ((*it)->_name == name)
        return (0);
    }
    {
      Party*        party    = new Party;
      const string& savepath = GameTask::CurrentGameTask->GetSavePath();
      string        save_party = savepath + "/party-" + name + ".blob";
      ifstream      file(save_party.c_str());

      party->_name = name;
      if (file.is_open())
      {
        Utils::Packet packet(file);

        party->UnSerialize(packet);
      }
      ForEach(party->_objects, [level](DynamicObject* object)
      {
        ObjectCharacter* character;

        level->GetWorld()->InsertDynamicObject(*object);
        level->InsertDynamicObject(*object);
        character = level->GetCharacter(object);
        if (character)
          character->SetOccupiedWaypoint(0);
      });
      level->_parties.push_back(party);
      return (party);
    }
  }
  return (0);
}
