#ifndef  DATAENGINE_HPP
# define DATAENGINE_HPP

# include "datatree.hpp"
# include "inventory.hpp"

class DataEngine : public Data
{
public:
  DataEngine(void) : Data(&_dataTree)
  {
  }

private:
  DataTree _dataTree;
};

#endif