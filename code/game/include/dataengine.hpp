#ifndef  DATAENGINE_HPP
# define DATAENGINE_HPP

# include "globals.hpp"
# include "datatree.hpp"

class WorldDiplomacy;

class DataEngine : public Data
{
public:
  DataEngine(void);
  ~DataEngine(void);
  
  void      Load(const std::string& filepath);
  void      Save(const std::string& filepath);

  WorldDiplomacy& GetDiplomacy(void);

private:
  DataTree*       _dataTree;
  WorldDiplomacy* _diplomacy;
};

#endif
