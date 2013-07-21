#include <string>

using namespace std;

string humanize(const string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if (i == 0 || str[i - 1] == '_')
      ret += str[i] - 'a' + 'A';
    else if (str[i] == '_')
      ret += ' ';
    else
      ret += str[i];
  }
  return (ret);
}

string underscore(const string& str)
{
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if      (str[i] >= 'A' && str[i] <= 'Z')
      ret += str[i] - 'A' + 'a';
    else if (str[i] == ' ')
      ret += '_';
    else
      ret += str[i];
  }
  return (ret);
}

