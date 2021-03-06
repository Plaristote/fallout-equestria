#ifndef  DIRECTORY_HPP
# define DIRECTORY_HPP

# include "globals.hpp"

# ifndef _WIN32
#  include <dirent.h>
#  include <sys/stat.h>
#  include <unistd.h>
#  include <string>
typedef struct dirent Dirent;
# else
#  define _WINSOCKAPI_
//#  include "semaphore.hpp"
#  include <Windows.h>
#  include <tchar.h>
#  include <stdio.h>
#  include <strsafe.h>
#  include <direct.h>
#  include <string>

#  define DT_DIR 1
#  define DT_REG 2

struct Dirent
{
  unsigned char d_type;
  std::string   d_name;
};
# endif

# include <fstream>
# include <list>

class Filesystem
{
public:
  static bool        FileContent(const std::string& filepath, std::string& out);
  static bool        FileCopy(const std::string& from, const std::string& dest);
  static bool        FileExists(const std::string& filepath);
  static std::size_t FileSize(const std::string& filepath);

private:
  Filesystem() {}
};

class Directory
{
public:
  typedef std::list<Dirent> Entries;  
  
  static bool        MakeDir(const std::string& str);
  static bool        RemoveDir(const std::string& str);
  static bool        Exists(const std::string& str);
  bool               OpenDir(const std::string& str);

  const std::string& Path(void)       const { return (_dirName); }
  Entries&           GetEntries(void)       { return (_dirEntries); }
  const Entries&     GetEntries(void) const { return (_dirEntries); }

private:
  Entries     _dirEntries;
  std::string _dirName;
};

#endif
