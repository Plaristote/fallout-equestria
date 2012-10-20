#ifndef  DIRECTORY_HPP
# define DIRECTORY_HPP

# ifndef _WIN32
#  include <dirent.h>
#  include <sys/stat.h>
typedef struct dirent Dirent;
# else
#  define _WINSOCKAPI_
#  include <Windows.h>
#  include <tchar.h>
#  include <stdio.h>
#  include <strsafe.h>

#  define DT_DIR 1
#  define DT_REG 2

struct Dirent
{
	unsigned char d_type;
	std::string   d_name;
};
# endif

class Directory
{
public:
  typedef std::list<Dirent> Entries;  
  
  static bool        MakeDir(const std::string& str)
  {
# ifndef _WIN32
    return (mkdir(str.c_str() , S_IRWXU | S_IRWXG | S_IRWXO));
# else
    return (CreateDirectory(str.c_str(), NULL));
# endif
  }
  
  bool               OpenDir(const std::string& str)
  {
# ifndef _WIN32
    DIR*             dir;
    Dirent*   ent;

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
#else
	HANDLE          dir = FindFirstFile(str.c_str(), NULL);
	WIN32_FIND_DATA ffd;
	if (dir != INVALID_HANDLE_VALUE)
	{
      _dirEntries.clear();
	  _dirName = str;
	  do
	  {
		Dirent dirent;

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{ dirent.d_type = DT_DIR; }
		else
		{ dirent.d_type = DT_REG; }
		dirent.d_name = ffd.cFileName;
		_dirEntries.push_back(dirent);
	  } while (FindNextFile(dir, &ffd) != 0);
	  FindClose(dir);
	}
	return (dir != INVALID_HANDLE_VALUE);
#endif
  }

  const std::string& Path(void)       const { return (_dirName); }

  Entries&           GetEntries(void)       { return (_dirEntries); }
  const Entries&     GetEntries(void) const { return (_dirEntries); }

private:
  Entries     _dirEntries;
  std::string _dirName;
};

#endif