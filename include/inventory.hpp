#ifndef  INVENTORY_HPP
# define INVENTORY_HPP

# include "datatree.hpp"

class InventoryObject : public Data
{
public:
  InventoryObject(Data);

private:
  DataTree _dataTree;
};

class Inventory
{
public:
  typedef std::list<InventoryObject*> Content;

  void             AddObject(InventoryObject*);
  void             DelObject(InventoryObject*);
  const Content&   GetContent(void) const { return (_content); }
  Content&         GetContent(void)       { return (_content); }

private:
  Content _content;
};

#endif