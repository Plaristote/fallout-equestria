#include "level/level.hpp"

/*
 * Level
 */
void Level::Load(Utils::Packet& packet)
{
  cout << "===> Load level now" << endl;
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

  cout << "Loading Objects" << endl;
  for_each(_objects.begin(),    _objects.end(),    [&packet](InstanceDynamicObject* object) { object->Load(packet);    });
  cout << "Loading Characters" << endl;
  for_each(_characters.begin(), _characters.end(), [&packet](ObjectCharacter* character)    { character->Load(packet); });
  cout << "===> Done" << endl;
}

void Level::SaveUpdateWorld(void)
{
  std::cout << "Using SaveUpdateWorld" << std::endl;
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
  cout << "--> Loading InstanceDynamicObject" << endl;
  char hasWaypoint;
  
  packet >> hasWaypoint;
  if (hasWaypoint == '1')
  {
    cout << _object->strModel << endl;
    unsigned int waypointId;

    packet.operator>> <unsigned int>(waypointId);
    cout << "----> Has waypoint (" << waypointId << ")" << endl;
    _waypointOccupied = _level->GetWorld()->GetWaypointFromId(waypointId);
  }
  cout << "--> Done" << endl;
}

void InstanceDynamicObject::Save(Utils::Packet& packet)
{
  if (_waypointOccupied)
  {
    packet << '1'; // has a waypointOccupied
    packet << _waypointOccupied->id;
    cout << "----> WaypointId = " << _waypointOccupied->id << endl;
  }
  else
    packet << '0';
}

/*
 * Character
 */
void ObjectCharacter::Load(Utils::Packet& packet)
{
  cout << "-> Loading ObjectCharacter" << endl;
  unsigned int       pathSize;
  asIScriptFunction* loadFunc = (_scriptContext && _scriptModule ? _scriptModule->GetFunctionByDecl("void Load(Serializer@)") : 0);

  InstanceDynamicObject::Load(packet);
  cout << "Trololo" << endl;
  packet >> _actionPoints >> _hitPoints >> _armorClass >> _tmpArmorClass;
  cout << "Tralala" << endl;
  packet.operator>> <unsigned int>(pathSize);
  cout << "Trilili" << endl;
  for (unsigned int i = 0 ; i < pathSize ; ++i)
  {
    unsigned int id;

    packet.operator>> <unsigned int>(id);
    cout << "Trululu" << endl;
    _path.push_back(*(_level->GetWorld()->GetWaypointFromId(id)));
  }

  cout << "ScriptSave" << endl;
  char hazScriptSave;
  packet >> hazScriptSave;
  if (hazScriptSave == '1')
  {
    cout << "HazScriptSave" << endl;
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
  cout << "-> End" << endl;
}

void ObjectCharacter::Save(Utils::Packet& packet)
{
  int                tmpPathSize = _path.size();
  asIScriptFunction* saveFunc    = (_scriptContext && _scriptModule ? _scriptModule->GetFunctionByDecl("void Save(Serializer@)") : 0);

  cout << "--> Saving character '" << _object->strModel << "'" << endl;
  InstanceDynamicObject::Save(packet);
  packet << _actionPoints << _hitPoints << _armorClass << _tmpArmorClass;
  packet << _path.size();
  for_each(_path.begin(), _path.end(), [this, &packet](Waypoint& wp) { packet << wp.id; });

  
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
