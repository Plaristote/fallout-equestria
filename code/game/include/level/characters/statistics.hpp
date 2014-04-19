#ifndef  CHARACTER_STATISTICS_HPP
# define CHARACTER_STATISTICS_HPP

# include "globals.hpp"
# include "level/pathfinding/user.hpp"
#include <level/tasks/metabolism.hpp>
# include "cmap/statcontroller.hpp"

class CharacterStatistics : public Pathfinding::User
{
public:
  CharacterStatistics(Level*, DynamicObject*);
  ~CharacterStatistics(void);

  void               ForceStatController(StatController* controller);
  void               ForceStatDatatree(DataTree* tree) { data_tree = tree; }
  StatController*    GetStatController(void) const { return (controller); }
  Data               GetStatistics(void)           { return (controller->GetData()); }
  bool               IsAlive(void) const           { return (GetHitPoints() > 0);    }

  short              GetMaxHitPoints(void) const;
  short              GetHitPoints(void)    const;
  void               SetHitPoints(short hp);
  
  Metabolism*        GetMetabolism(void)   const { return (metabolism); }
  void               SetMetabolism(Metabolism*);
  
  virtual void       Serialize(Utils::Packet&);
  virtual void       Unserialize(Utils::Packet&);
  
protected:
  virtual void       RefreshStatistics(void);

  StatController*    controller;
private:
  bool               TryToLoadCharacterSheet(void);
  bool               TryToLoadFromFile(const std::string& path);
  bool               TryToLoadFromString(const std::string& json);
  void               GenerateCharacterSheet(void);
  void               Cleanup(void);

  Metabolism*        metabolism;
  DataTree*          data_tree;
  string             character_sheet_name;
};

#endif
