#include "npc.hpp"

using namespace std;

ObjectNode* Npc::Factory(WindowFramework* window, Tilemap& tilemap, Characters& chars, Data data)
{
  Npc* npc = new Npc(window, tilemap, data, chars);

  chars.push_back(npc);
  return (npc);
}

Npc::Npc(WindowFramework* w, Tilemap& t, Data data, Characters& c) : Character(w, t, data, c)
{
  _ia     = &Npc::DummyIa;
  _iaStep = 0;
}

void Npc::Run(float elapsedTime)
{
  if (_ia)
    (this->*_ia)(elapsedTime);
  Character::Run(elapsedTime);
}

void Npc::DummyIa(float)
{
  if (_path.empty())
  {
    if      (_iaStep == 0)
      GoTo(_mapPos.x + 5, _mapPos.y + 10);
    else if (_iaStep == 1)
      GoTo(_mapPos.x + 5, _mapPos.y);
    else if (_iaStep == 2)
      GoTo(_mapPos.x - 6, _mapPos.y - 6);
    else if (_iaStep == 3)
      GoTo(_mapPos.x - 4, _mapPos.y - 4);
    _iaStep++;
    if (_iaStep == 4)
      _iaStep = 0;
  }
}