#include "level/level.hpp"

/*
 * Level
 */
void Level::Load(Utils::Packet& packet)
{
  char tmpState;
  int  countObject;
  int  countCharacters;

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
  for_each(_characters.begin(), _characters.end(), [&packet](ObjectCharacter* character)    { character->Load(packet); });
}

void Level::SaveUpdateWorld(void)
{
  for_each(_characters.begin(), _characters.end(), [this](ObjectCharacter* character)
  {
    Inventory& inventory = character->GetInventory();
    
    inventory.SaveInventory(character->GetDynamicObject());
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
  for_each(_characters.begin(), _characters.end(), [&packet](ObjectCharacter* character)    { character->Save(packet); });
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
  asIScriptFunction* loadFunc = (_scriptContext && _scriptModule ? _scriptModule->GetFunctionByDecl("void Load(Serializer@)") : 0);

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
    if (!loadFunc)
    {
      _scriptContext->Prepare(loadFunc);
      _scriptContext->SetArgAddress(0, &packet);
      _scriptContext->Execute();
    }
    else
      std::cerr << "/!\\ FATAL ERROR: " << this->GetName() << "'s Script Load function couldn't be loaded, and the whole loading will probably screw up starting now.";
  }
  
  if (_hitPoints <= 0)
    RunDeath();
}

void ObjectCharacter::Save(Utils::Packet& packet)
{
  int                tmpPathSize = _path.size();
  asIScriptFunction* saveFunc    = (_scriptContext && _scriptModule ? _scriptModule->GetFunctionByDecl("void Save(Serializer@)") : 0);

  InstanceDynamicObject::Save(packet);
  packet << _actionPoints << _hitPoints << _armorClass << _tmpArmorClass;
  packet << _path.size();
  for_each(_path.begin(), _path.end(),   [this, &packet](Waypoint& wp)        { packet << wp.id;    });
  packet << _buffs.size();
  for_each(_buffs.begin(), _buffs.end(), [this, &packet](CharacterBuff* buff) { buff->Save(packet); });
  
  if (saveFunc)
  {
    packet << '1'; // Haz script save
    _scriptContext->Prepare(saveFunc);
    _scriptContext->SetArgAddress(0, &packet);
    _scriptContext->Execute();
  }
  else
    packet << '0'; // Haz not script save
}

void CharacterBuff::Load(Level* level, ObjectCharacter* character, Utils::Packet& packet)
{
  string             json;
  DataTree*          buff;

  _task = _timeManager.AddTask(TASK_LVL_CITY, false, 0);
  packet.operator>> <unsigned short>(_task->lastS);
  packet.operator>> <unsigned short>(_task->lastM);
  packet.operator>> <unsigned short>(_task->lastH);
  packet.operator>> <unsigned short>(_task->lastD);
  packet.operator>> <unsigned short>(_task->lastMo);
  packet.operator>> <unsigned short>(_task->lastY);
  packet.operator>> <unsigned short>(_task->timeS);
  packet.operator>> <unsigned short>(_task->timeM);
  packet.operator>> <unsigned short>(_task->timeH);
  packet.operator>> <unsigned short>(_task->timeD);
  packet.operator>> <unsigned short>(_task->timeMo);
  packet.operator>> <unsigned short>(_task->timeY);
  packet >> json;
  buff  = DataTree::Factory::StringJSON(json);
  Initialize(level, character, buff);
  Begin(0, _task);
}

void CharacterBuff::Save(Utils::Packet& packet)
{
  string json;
  Data   safeDataCopy;

  safeDataCopy.Duplicate(_buff);
  DataTree::Writers::StringJSON(safeDataCopy, json);
  packet << _task->lastS << _task->lastM << _task->lastH << _task->lastD << _task->lastMo << _task->lastY;
  packet << _task->timeS << _task->timeM << _task->timeH << _task->timeD << _task->timeMo << _task->timeY;
  packet << json;
}
