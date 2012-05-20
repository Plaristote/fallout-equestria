#ifndef  NPC_HPP
# define NPC_HPP

# include "character.hpp"

class Npc : public Character
{
public:
  static ObjectNode* Factory(WindowFramework*, Tilemap&, Characters&, Data);

  Npc(WindowFramework* w, Tilemap& t, Data data, Characters&);
  
  void        Run(float elapsedTime);

private:
  typedef void (Npc::*IARunner)(float elapsedTime);

  void        DummyIa(float);

  unsigned int _iaStep;
  IARunner     _ia;
};

#endif