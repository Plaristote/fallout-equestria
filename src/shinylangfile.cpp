#include "shinylang.hpp"
#include <fstream>
#include <iostream>

using namespace ShinyLang;
using namespace std;

Parser::Parser(const string& filename)
{
  std::ifstream file(filename.c_str());

  if (file.is_open())
  {
    long  begin, end;
    long  size;
    char* raw;
    std::string text;

    begin     = file.tellg();
    file.seekg (0, ios::end);
    end       = file.tellg();
    file.seekg(0, ios::beg);
    size      = end - begin;
    raw       = new char[size + 1];
    file.read(raw, size);
    file.close();
    raw[size] = 0;
    text      = raw;
    delete[] raw;

    int begLine = 0;
    for (int i = 0 ; i < size ; ++i)
    {
      if (text[i] == '\n')
      {
        std::string line  = text.substr(begLine, i - begLine);
        bool        empty = true;

        for (unsigned int ii = 0 ; ii < line.size() ; ++ii)
        {
          if (line[ii] == '#')
          {
            for (unsigned int iii = ii ; iii < line.size() ; ++iii)
              line[iii] = ' ';
          }
          if (line[ii] == '\t')
            line[ii] = ' ';
          else if (line[ii] != ' ')
            empty = false;
          if (line[ii] == ' ' && empty)
          {
            line = line.substr(ii + 1);
            --ii;
          }
        }
        if (!empty)
          _lines.push_back(line);

        begLine = i + 1;
        if (text[i + 1] == '\r')
        {
          ++begLine;
          i      += 1;
        }
        continue ;
      }
    }
    _fileExist = true;
  }
  else
  {
    _fileExist = false;
    std::cerr << filename << ": can't open file." << std::endl;
  }
}

Parser::~Parser()
{
}

DataTree* Parser::Run()
{
  if (!_fileExist)
    return (0);
  DataTree*              file    = new DataTree();
  list<string>::iterator line    = _lines.begin();
  list<string>::iterator end     = _lines.end();
  DataBranch*            current = file;

  for (; line != end ; ++line)
  {
    std::string& str = *line;

    if (str.substr(0, 4) == "def ")
    {
      DataBranch* newBlock = new DataBranch;
      newBlock->nil    = false;
      newBlock->key    = str.substr(4);
      newBlock->father = current;
      current->children.push_back(newBlock);
      current = newBlock;
    }
    else if (str.substr(0, 3) == "end" && str[3] != ' ')
    {
      if (current->father == 0)
      {
        std::cout << "Syntax error: unexpected end of block" << std::endl;
        return (file);
      }
      current = current->father;
    }
    else
    {
      DataBranch*  branch = new DataBranch;
      unsigned int endKey, begValue;
      string       key, value;

      for (endKey   = 0          ; endKey < str.size() && str[endKey] != ':'     ; ++endKey);
      for (begValue = endKey + 1 ; begValue < str.size() && str[begValue] == ' ' ; ++begValue);

      key   = str.substr(0, endKey);
      if (begValue < str.size())
        value = str.substr(begValue);

      branch->nil    = false;
      branch->key    = key;
      branch->value  = value;
      branch->father = current;
      current->children.push_back(branch);
    }
  }
  return (file);
}

DataTree* DataTree::Factory::ShinyLang(const std::string& filename)
{
  Parser parser(filename);

  return (parser.Run());
}
