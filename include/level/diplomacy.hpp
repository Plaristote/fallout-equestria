#ifndef  DIPLOMACY_HPP
# define DIPLOMACY_HPP

# include <string>
# include "dataengine.hpp"

class WorldDiplomacy
{
public:
  struct Faction
  {
    std::string  name;
    unsigned int flag;
    unsigned int enemyMask;

    bool operator==(const std::string& name)
    { return (this->name == name); }

    bool operator==(unsigned int flag)
    { return (this->flag == flag); }
  };

  typedef std::list<Faction> Factions;

  WorldDiplomacy(DataEngine&);
  ~WorldDiplomacy(void);

  void     AddFaction(const std::string& name);
  Faction* GetFaction(const std::string& name);
  Faction* GetFaction(unsigned int flag);

  void     SetAsEnemy(bool set, const std::string& name1, const std::string& name2);
  void     SetAsEnemy(bool set, unsigned int flag1, unsigned int flag2);

  void     Initialize(void);

private:
  void     SetAsEnemy(bool set, Faction& first, Faction& second);

  DataEngine&  _data_engine;
  Factions     _factions;
  unsigned int _next_flag;
};

#endif


