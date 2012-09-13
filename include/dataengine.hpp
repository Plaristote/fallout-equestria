#ifndef  DATAENGINE_HPP
# define DATAENGINE_HPP

# include "datatree.hpp"

class DataEngine : public Data
{
public:
  DataEngine(void)
  {
    _dataTree = 0;
  }
  
  ~DataEngine(void)
  {
//     Remove();
//     _dataTree = 0;
  }
  
  void      Load(const std::string& filepath);
  void      Save(const std::string& filepath);

private:
  DataTree* _dataTree;
};

#endif