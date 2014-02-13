#include "datatree.hpp"
#include <fstream>
#include <iostream>

using namespace std;

void      Data::Output(unsigned char indent)
{
  Data::iterator it  = this->begin();
  Data::iterator end = this->end();

  for (unsigned char toIndent = indent ; toIndent ; --toIndent)
    std::cout << ' ';
  std::cout << "[" << _data->key << "] -> '" << _data->value << "'" << std::endl;
  for (; it != end ; ++it)
    (*it).Output(indent + 2);
}

static void WriteFile(ofstream& stream, const Data data, unsigned char indent)
{
  Data::const_iterator it  = data.const_begin();
  Data::const_iterator end = data.const_end();

  for (unsigned char toIndent = indent ; toIndent ; --toIndent)
    stream << ' ';
  stream << data.Key() << ": " << data.Value() << endl;
  for (; it != end ; ++it)
    (*it).Output(indent + 2);
}

void Data::MoveUp()
{
  if (_data->father)
  {
    list<DataBranch*>& entries  = _data->father->children;
    auto               it       = entries.begin();
    auto               previous = it;

    for (; it != entries.end() ; ++it)
    {
      if (*it == _data)
        break ;
      previous = it;
    }
    entries.insert(previous, _data);
    entries.erase(it);
    Parent().Output();
  }
}

void Data::MoveDown()
{
  if (_data->father)
  {
    list<DataBranch*>& entries  = _data->father->children;
    auto               it       = entries.begin();
    auto               next     = it;

    while (it != entries.end())
    {
      ++next;
      if (*it == _data)
        break ;
      ++it;
    }
    entries.insert(++next, _data);
    entries.erase(it);
    Parent().Output();
  }
}

bool DataTree::Save(const string& path)
{
  ofstream file;
  
  file.open(path.c_str());
  if (file.is_open())
  {
    WriteFile(file, this, 0);
    file.close();
    return (true);
  }
  return (false);
}

Data::Data(DataTree* d) : _data(d)
{}

Data::Data(const std::string& key, DataBranch* father)
{
  _data = new DataBranch();
  _data->father   = father;
  _data->key      = key;
  _data->pointers = 1;
  _data->nil      = true;
  father->children.push_back(_data);
}

Data::~Data()
{
  if (_data)
  {
    _data->pointers--;
    if ((_data->nil || !_data->father) && _data->root == false && _data->pointers == 0)
      delete _data;
  }
}

Data Data::operator[](unsigned int n)
{
  DataBranch::Children::iterator it  = _data->children.begin();
  DataBranch::Children::iterator end = _data->children.end();
  
  for (unsigned int nIt = 0 ; it != end && nIt < n ; ++it, ++nIt);
  if (it != end)
    return (Data((*it)));
  return (Data());
}

Data Data::operator[](const std::string& key)
{
  if (_data)
  {
    DataBranch::Children::iterator it  = _data->children.begin();
    DataBranch::Children::iterator end = _data->children.end();

    _data->nil = false;
    for (; it != end ; ++it)
    {
      if ((*it)->key == key)
	return (Data((*it)));
    }
  }
  return (Data(key, _data));
}

const Data Data::operator[](const std::string& key) const
{
  if (_data)
  {
    DataBranch::Children::const_iterator it  = _data->children.begin();
    DataBranch::Children::const_iterator end = _data->children.end();

    _data->nil = false;
    for (; it != end ; ++it)
    {
      if ((*it)->key == key)
	return (Data((*it)));
    }
  }
  return (Data(key, _data));
}

void        Data::Duplicate(Data var)
{
  Data::iterator it  = var.begin();
  Data::iterator end = var.end();
  
  _data->key   = var.Key();
  _data->value = var.Value();
  _data->nil   = false;

  for (; it != end ; ++it)
  {
    Data        children = *it;
    DataBranch* tmp      = new DataBranch();
    
    tmp->father = _data;
    _data->children.push_back(tmp);
    Data(tmp).Duplicate(children);
  }
}

const Data& Data::operator=(const Data& var)
{
  if (var.Nil() && !Nil())
  {
    _data->pointers--;
    delete _data;
    _data = 0;
  }
  else if (Nil())
  {
    if (_data && _data->pointers == 1)
      delete _data;
    _data = var._data;
    if (_data)
      _data->pointers++;
  }
  else
    _data->value = var.Value();
  return (*this);
}

DataBranch::~DataBranch()
{
  Children::iterator it;
  Children::iterator end;

  if (father)
  {
    it  = father->children.begin();
    end = father->children.end();
    
    for (; it != end ; ++it)
    {
      if ((*it) == this)
      {
	father->children.erase(it);
	break ;
      }
    }
  }
  while (children.begin() != children.end())
  {
    DataBranch* child = *children.begin();

    if (child->nil == false)
      delete child;
    else
    {
      children.erase(children.begin());
      child->father = 0;
    }
  }
}
