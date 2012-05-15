#ifndef  SHINYLANG_HPP
# define SHINYLANG_HPP

# include <string>
# include <vector>
# include "datatree.hpp"

namespace ShinyLang
{
  class Parser
  {
  public:
    Parser(const std::string&);
    ~Parser();
    
    DataTree*                   Run(void);

  private:
    bool                       _fileExist;
    std::list<std::string>     _lines;
  };
}
#endif
