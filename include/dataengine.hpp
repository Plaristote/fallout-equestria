#ifndef  DATAENGINE_HPP
# define DATAENGINE_HPP

# include "datatree.hpp"
# include "inventory.hpp"

class DataEngine : public Data
{
public:
  DataEngine(void)
  {
  }
  
  void      Load(const std::string& filepath);
  void      Save(const std::string& filepath);

private:
  DataTree* _dataTree;
};

#endif