#include "level/level.hpp"
#include "level/objects/shelf.hpp"
#include "level/objects/locker.hpp"
#include <gametask.hpp>

/*
 * Level
 */
void Level::Load(Utils::Packet& packet)
{
  char tmpState;

  packet >> tmpState;
  _state = (State)tmpState;
  if (_state == Fight)
  {
    string nameItCharacter;

    packet >> nameItCharacter;
    for (_itCharacter = _characters.begin() ; _itCharacter != _characters.end() && 
                                              (*_itCharacter)->GetName() != nameItCharacter ; ++_itCharacter);
    StartFight(*_itCharacter);
  }

  for_each(_objects.begin(),    _objects.end(),    [&packet](InstanceDynamicObject* object) { object->Unserialize(packet);    });
  for_each(_characters.begin(), _characters.end(), [&packet](ObjectCharacter* character)    { character->Unserialize(packet); });

  _task_metabolism->Unserialize(packet);
}

void Level::SaveUpdateWorld(void)
{
  //
  // This is saving the inventories in the DynamicObject structure, thus they will be saved
  // in the World part of the save file. This information consequently needs to be processed before everything else.
  //
  for_each(_characters.begin(), _characters.end(), [this](ObjectCharacter* character)
  {
    Inventory& inventory = character->GetInventory();

    inventory.SaveInventory(character->GetDynamicObject());
  });
  for_each(_objects.begin(), _objects.end(), [this](InstanceDynamicObject* object)
  {
    ObjectShelf* shelf = object->Get<ObjectShelf>();

    if (!shelf) { shelf = object->Get<ObjectLocker>(); }
    if (shelf)
    {
      Inventory& inventory = shelf->GetInventory();

      inventory.SaveInventory(shelf->GetDynamicObject());
    }
  });
}

void Level::Save(Utils::Packet& packet)
{
  SaveUpdateWorld();
  UnprocessAllCollisions();
  GetWorld()->Serialize(packet);
  {
    packet << (char)(_state);
    if (_state == Fight)
      packet << ((*_itCharacter)->GetName());

    for_each(_objects.begin(),    _objects.end(),    [&packet](InstanceDynamicObject* object) { object->Serialize(packet);    });
    for_each(_characters.begin(), _characters.end(), [&packet](ObjectCharacter* character)    { character->Serialize(packet); });

    _task_metabolism->Serialize(packet);
  }
  ProcessAllCollisions();
  cout << "End Level::Save" << endl;
}