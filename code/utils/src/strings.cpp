#include <string>
#include <list>

using namespace std;

list<string> split(const std::string& str, char c = ' ')
{
  list<string> ret;
  short        last_sep = -1;
  short        i;

  for (i = 0 ; str[i] ; ++i)
  {
    if (str[i] == c)
    {
      if (i != 0 && str[i - 1] != c)
        ret.push_back(str.substr(last_sep + 1, i - (last_sep + 1)));
      last_sep = i;
    }
  }
  if (last_sep != i && i > 0 && str[i - 1] != c)
    ret.push_back(str.substr(last_sep + 1));
  return (ret);
}

string merge_strings(const list<string>& array, char c = ' ')
{
  string result;
  auto   it  = array.begin();
  auto   end = array.end();

  for (; it != end ; ++it)
  {
    if (result != "")
      result += c;
    result += *it;
  }
  return (result);
}

string str_strip(const string& str)
{
  unsigned short w_begin, w_end;
  bool           got_begin = false;

  for (unsigned short i = 0 ; i < str.size() ; ++i)
  {
    if (str[i] != ' ')
    {
      if (got_begin == false)
      {
        w_begin   = i;
        got_begin = true;
      }
      w_end = i;
    }
  }
  return (str.substr(w_begin, (w_end + 1) - w_begin));
}

string humanize(const string& tmp)
{
  string str = str_strip(tmp);
  string ret;
  
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if (i == 0 || str[i - 1] == '_' || str[i - 1] == ' ')
    {
      if (str[i] >= 'a' && str[i] <= 'z')
        ret += str[i] - 'a' + 'A';
      else
        ret += str[i];
    }
    else if (str[i] == '_')
    {
      if (i == 0 || str[i - 1] != '_')
        ret += ' ';
    }
    else
      ret += str[i];
  }
  return (ret);
}

string underscore(const string& tmp)
{
  string         str = str_strip(tmp);
  string         ret;
 
  for (unsigned short i = 0 ;  i < str.size() ; ++i)
  {
    if      (str[i] >= 'A' && str[i] <= 'Z')
      ret += str[i] - 'A' + 'a';
    else if (str[i] == ' ')
    {
      if (str[i - 1] != ' ')
        ret += '_';
    }
    else
      ret += str[i];
  }
  return (ret);
}

