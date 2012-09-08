#ifndef  DIRECTORY_HPP
# define DIRECTORY_HPP

# include <dirent.h>
# ifndef WIN32
#  include <sys/stat.h>
# endif

class Directory
{
public:
  typedef std::list<struct dirent> Entries;  
  
  static bool        MakeDir(const std::string& str)
  {
# ifndef WIN32
    return mkdir(str.c_str() , S_IRWXU | S_IRWXG | S_IRWXO);
# else
    return mkdir(str.c_str());
# endif
  }
  
  bool               OpenDir(const std::string& str)
  {
    DIR*             dir;
    struct dirent*   ent;

    dir = opendir(str.c_str());
    if (dir != NULL)
    {
      _dirEntries.clear();
      _dirName    = str;
      /* print all the files and directories within directory */
      while ((ent = readdir(dir)) != NULL)
	_dirEntries.push_back(*ent);
      closedir (dir);
    }
    return (dir != 0);
  }

  const std::string& Path(void)       const { return (_dirName); }

  Entries&           GetEntries(void)       { return (_dirEntries); }
  const Entries&     GetEntries(void) const { return (_dirEntries); }

private:
  Entries     _dirEntries;
  std::string _dirName;
};

#endif