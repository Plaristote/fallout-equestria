#include "level/level.hpp"
#include "level/objects/shelf.hpp"
#include "level/objects/locker.hpp"
#include <gametask.hpp>

void Level::BackupInventoriesToDynamicObjects(void)
{
  //
  // This is saving the inventories in the DynamicObject structure, thus they will be saved
  // in the World part of the save file. This information consequently needs to be processed before everything else.
  //
  for_each(characters.begin(), characters.end(), [this](ObjectCharacter* character)
  {
    Inventory& inventory = character->GetInventory();

    inventory.SaveInventory(character->GetDynamicObject());
  });
  for_each(objects.begin(), objects.end(), [this](InstanceDynamicObject* object)
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

void Level::SerializeParties(Utils::Packet& packet)
{
  unsigned int party_count = parties.size();
  
  packet << party_count;
  for_each(parties.begin(), parties.end(), [&packet](Party* party)
  {
    party->Serialize(packet);
  });
}

void Level::UnserializeParties(Utils::Packet& packet)
{
  unsigned int party_count;
  
  packet >> party_count;
  for (unsigned int i = 0 ; i < party_count ; ++i)
  {
    Party* party = new Party;
    
    party->Unserialize(packet);
    MatchPartyToExistingCharacters(*party);
  }
}

void Level::Serialize(Utils::Packet& packet)
{
  BackupInventoriesToDynamicObjects();
  UnprocessAllCollisions();
  {
    GetWorld()->Serialize(packet);
    packet << (char)(level_state);
    for_each(objects.begin(),    objects.end(),    [&packet](InstanceDynamicObject* object) { object->Serialize(packet);    });
    for_each(characters.begin(), characters.end(), [&packet](ObjectCharacter* character)    { character->Serialize(packet); });
    combat.Serialize(packet);
  }
  ProcessAllCollisions();
}

void Level::Unserialize(Utils::Packet& packet)
{
  char tmpState;

  packet >> tmpState;
  level_state = (State)tmpState;
  for_each(objects.begin(),    objects.end(),    [&packet](InstanceDynamicObject* object) { object->Unserialize(packet);    });
  for_each(characters.begin(), characters.end(), [&packet](ObjectCharacter* character)    { character->Unserialize(packet); });
  combat.Unserialize(packet);
}
