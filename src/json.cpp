#include "json.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace Json;

void Parser::ParseString(DataBranch* value)
{
  unsigned int start = ++_it;

  for (; _it < _str.length() ; ++_it)
  {
    if ((_str[_it] == '"') && (_str[_it] != '\\'))
      break ;
  }
  if (_it < _str.length())
    value->value = _str.substr(start, _it - start);
}

void Parser::ParseOther(DataBranch* value)
{
  unsigned int start = _it;
  unsigned int endSpace;

  for (; _it < _str.length() && (_str[_it] != ',' && _str[_it] != '}' && _str[_it] != ']') ; ++_it);
  endSpace = _it - 1;
  if (_str[endSpace] == ' ' || _str[endSpace] == '\n' || _str[endSpace] == '\r')
    for (; endSpace > start && (_str[endSpace] == ' ' || _str[endSpace] == '\n' || _str[endSpace] == '\r') ; --endSpace);
  if (_it < _str.length())
    value->value = _str.substr(start, (endSpace + 1) - start);
}

void Parser::ParseValue(DataBranch* data)
{
  for (; _it < _str.length() && _str[_it] == ' ' ; ++_it);
  if (_it < _str.length())
  {
    if (_str[_it] == '{')
      ParseObject(data);
    else if (_str[_it] == '[')
      ParseArray(data);
    else if (_str[_it] == '"')
      ParseString(data);
    else
      ParseOther(data);
  }
}

void Parser::ParseObject(DataBranch* value)
{
  int         begKey;

  ++_it;
  while (_it < _str.length())
  {
    for (; _it < _str.length() && (_str[_it] == ' ' || _str[_it] == '\n' || _str[_it] == '\r') ; ++_it);
    if (_it < _str.length())
    {
      std::string key;

      // OBJECT KEY - OBJECT END
      if (_str[_it] == '"') // Quoted key
      {
        for (begKey = ++_it ; _it < _str.length() && (_str[_it] != '"' || _str[_it] == '\\') ; ++_it);
        if (_it >= _str.length())
          throw "String: Syntax Error";
        key = _str.substr(begKey, _it - begKey);
        for (_it = _it + 1 ; _it < _str.length() && _str[_it] == ' ' ; ++_it);
        if (_it >= _str.length() || _str[_it] != ':')
          throw "Object: Syntax Error";
        ++_it;
      }
      else if (_str[_it] == '}') // End of object
      {
        ++_it;
        break ;
      }
      else // Unquoted key
      {
        for (begKey = _it ; _it < _str.length() && _str[_it] != ':' ; ++_it);
        if (_it >= _str.length())
          throw "Object: Syntax Error";
        key = _str.substr(begKey, _it - begKey);
      }

      // MAKE THE BRANCH
      DataBranch* child = new DataBranch;

      child->father = value;
      child->nil    = false;
      child->key    = key;
      value->children.push_back(child);

      // OBJECT VALUE
      for (; _it < _str.length() && (_str[_it] == ' ' || _str[_it] == '\n' || _str[_it] == '\r') ; ++_it);
      ParseValue(child);

      for (; _it < _str.length() && _str[_it] != ',' && _str[_it] != '}' ; ++_it);
      if (_str[_it] == ',') ++_it;
    }
  }
}

void Parser::ParseArray(DataBranch* value)
{
  ++_it;
  while (_it < _str.length())
  {
    for (; _it < _str.length() && (_str[_it] == ' ' || _str[_it] == '\n' || _str[_it] == '\r') ; ++_it);
    if (_it >= _str.length())
      throw "Unclosed array";
    if (_str[_it] == ']')
      break ;
    DataBranch* child = new DataBranch;

    child->father = value;
    child->nil    = false;
    value->children.push_back(child);
    ParseValue(child);

    for (; _it < _str.length() && _str[_it] != ',' && _str[_it] != ']' ; ++_it);
    if (_str[_it] == ',')
      ++_it;
  }
}

Parser::Parser(const string& filename)
{
  std::ifstream file(filename.c_str());

  if ((_fileLoaded = file.is_open()))
  {
    long  begin, end;
    long  size;
    char* raw;

    begin     = file.tellg();
    file.seekg (0, ios::end);
    end       = file.tellg();
    file.seekg(0, ios::beg);
    size      = end - begin;
    raw       = new char[size + 1];
    file.read(raw, size);
    file.close();
    raw[size] = 0;
    _str      = raw;
    delete[] raw;

    for (int i = 0 ; i < size ; ++i)
    {
      if (_str[i] == '\t')
        _str[i] = ' ';
      if (_str[i] == '"')
        for (; i < size && _str[i] != '"' && (i == 0 || _str[i] - 1 != '\\') ; ++i);
    }
  }
  else
    std::cerr << filename << ": can't open file." << std::endl;
}

DataTree* Parser::Run()
{
  if (_fileLoaded)
  {
    DataTree* data = new DataTree;

    try
    {
      _it            = 0;
      ParseValue(data);
    }
    catch (const char* error)
    {
      std::cout << "/!\\ Parse Error at " << _it << " : " << error << std::endl;
      delete data;
      data           = 0;
    }
    return (data);
  }
  return (0);
}

DataTree* DataTree::Factory::JSON(const std::string& filename)
{
  Parser parser(filename);

  return (parser.Run());
}
