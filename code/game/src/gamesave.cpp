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

  for_each(_objects.begin(),    _objects.end(),    [&packet](InstanceDynamicObject* object) { object->Load(packet);    });
  cout << ">>>> Objects: " << _world->dynamicObjects.size() << endl;
  cout << ">>>> Loading " << _characters.size() << " characters" << endl;
  for_each(_characters.begin(), _characters.end(), [&packet](ObjectCharacter* character)    { character->Load(packet); });

  GameTask::CurrentGameTask->LoadLevelBuffs(packet);

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
  packet << (char)(_state);
  if (_state == Fight)
    packet << ((*_itCharacter)->GetName());

  for_each(_objects.begin(),    _objects.end(),    [&packet](InstanceDynamicObject* object)
  {
    if (object)
      object->Save(packet);
  });
  cout << ">>>> Objects: " << _world->dynamicObjects.size() << endl;
  cout << ">>>> Saving " << _characters.size() << " characters" << endl;
  for_each(_characters.begin(), _characters.end(), [&packet](ObjectCharacter* character)    { character->Save(packet); });
  std::cout << "Done saving characters" << std::endl;

  if (GameTask::CurrentGameTask)
    GameTask::CurrentGameTask->SaveLevelBuffs(packet);
  std::cout << "Done saving level buffs" << std::endl;

  _task_metabolism->Serialize(packet);
  cout << "End Level::Save" << endl;
}

/*
 * InstanceDynamicObject
 */
void InstanceDynamicObject::Load(Utils::Packet& packet)
{
  char hasWaypoint;
  
  packet >> hasWaypoint;
  if (hasWaypoint == '1')
  {
    unsigned int waypointId;

    packet.operator>> <unsigned int>(waypointId);
    _waypointOccupied = _level->GetWorld()->GetWaypointFromId(waypointId);
  }
}

void InstanceDynamicObject::Save(Utils::Packet& packet)
{
  if (_waypointOccupied)
  {
    packet << '1'; // has a waypointOccupied
    packet << _waypointOccupied->id;
  }
  else
    packet << '0';
}

/*
 * Character
 */
void ObjectCharacter::Load(Utils::Packet& packet)
{
  unsigned int       pathSize, buffs_size;

  InstanceDynamicObject::Load(packet);
  packet >> _actionPoints >> _hitPoints >> _armorClass >> _tmpArmorClass;
  packet.operator>> <unsigned int>(pathSize);
  for (unsigned int i = 0 ; i < pathSize ; ++i)
  {
    unsigned int id;

    packet.operator>> <unsigned int>(id);
    _path.push_back(*(_level->GetWorld()->GetWaypointFromId(id)));
  }
  
  packet.operator>> <unsigned int>(buffs_size);
  for (unsigned int i = 0 ; i < buffs_size ; ++i)
  {
    CharacterBuff* buff = new CharacterBuff(_level);
    
    buff->Load(_level, this, packet);
    _buffs.push_back(buff);
  }

  char hazScriptSave;
  packet >> hazScriptSave;
  if (hazScriptSave == '1')
  {
    if (_script->IsDefined("Load"))
    {
      AngelScript::Type<Utils::Packet*> param_packet(&packet);
      
      _script->Call("Load", 1, &param_packet);
    }
    else
      std::cerr << "/!\\ FATAL ERROR: " << this->GetName() << "'s Script Load function couldn't be loaded, and the whole loading will probably screw up starting now.";
  }
  
  if (_hitPoints <= 0)
    RunDeath();
}

void ObjectCharacter::Save(Utils::Packet& packet)
{
  InstanceDynamicObject::Save(packet);
  packet << _actionPoints << _hitPoints << _armorClass << _tmpArmorClass;
  packet << (unsigned int)_path.size();
  for_each(_path.begin(), _path.end(),   [this, &packet](Waypoint& wp)        { packet << wp.id;    });
  packet << (unsigned int)_buffs.size();
  for_each(_buffs.begin(), _buffs.end(), [this, &packet](CharacterBuff* buff) { buff->Save(packet); });
  
  if (_script->IsDefined("Save"))
  {
    AngelScript::Type<Utils::Packet*> param_packet(&packet);

    packet << '1'; // Haz script save
    _script->Call("Save", 1, &param_packet);
  }
  else
    packet << '0'; // Haz not script save
}

void CharacterBuff::Load(Level* level, ObjectCharacter* character, Utils::Packet& packet)
{
  string             json;
  DataTree*          buff;

  _task = _timeManager.AddTask(TASK_LVL_CITY, 0);
  _task->next_run.Unserialize(packet);
  _task->length.Unserialize(packet);
  packet >> json;
  buff  = DataTree::Factory::StringJSON(json);
  Initialize(level, character, buff);
  Begin(0, _task);
}

void CharacterBuff::Save(Utils::Packet& packet)
{
  string json;

  DataTree::Writers::StringJSON(_buff, json);
  _task->next_run.Serialize(packet);
  _task->length.Serialize(packet);
  packet << json;
}
