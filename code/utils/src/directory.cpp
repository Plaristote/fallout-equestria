#include "directory.hpp"

using namespace std;

bool Filesystem::FileContent(const string& filepath, string& out)
{
  ifstream file(filepath.c_str());

  if (file.is_open())
  {
    long           size;
    std::streamoff begin, end;
    char*          raw;

    begin       = file.tellg();
    file.seekg(0, std::ios::end);
    end         = file.tellg();
    file.seekg(0, std::ios::beg);
    size        = end - begin;
    raw         = new char[size + 1];
    file.read(raw, size);
    file.close();
    out = raw;
    delete[] raw;
    return (true);
  }
  return (false);
}

bool Filesystem::FileCopy(const string& from, const string& dest)
{
  ifstream stream_src;
  ofstream stream_dest;

  stream_src.open (from.c_str(), ios::binary);
  stream_dest.open(dest.c_str(), ios::binary);
  if (stream_src.is_open() && stream_dest.is_open())
  {
    std::streamoff begin, end;
    long           size;
    char*          raw;

    begin       = stream_src.tellg();
    stream_src.seekg(0, std::ios::end);
    end         = stream_src.tellg();
    stream_src.seekg(0, std::ios::beg);
    size        = end - begin;
    raw         = new char[size + 1];
    stream_src.read(raw, size);
    stream_src.close();
    raw[size]   = 0;
    stream_dest.write(raw, size);
    delete[] raw;
    return (true);
  }
  return (false);
}

std::size_t Filesystem::FileSize(const string& file)
{
  ifstream in(file.c_str(), ifstream::in | ifstream::binary);
  
  if (in.is_open())
  {
    in.seekg(0, ifstream::end);
    return (in.tellg());
  }
  return (0);
}

#ifndef _WIN32 // UNIX

bool Filesystem::FileExists(const string& file)
{
  return (!(::access(file.c_str(), F_OK)));
}

bool Directory::MakeDir(const string& str)
{
  return (mkdir(str.c_str() , S_IRWXU | S_IRWXG | S_IRWXO) == 0);
}

bool Directory::RemoveDir(const string& str)
{
  return (rmdir(str.c_str()) == 0);
}

bool Directory::OpenDir(const string& str)
{
  DIR*      dir;

  dir = opendir(str.c_str());
  if (dir != NULL)
  {
    Dirent* ent;

    _dirEntries.clear();
    _dirName    = str;
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL)
      _dirEntries.push_back(*ent);
    closedir (dir);
  }
  return (dir != 0);
}

#else // WINDOWS
# include <io.h>

bool Filesystem::FileExists(const string& file)
{
  return (!(_access(file.c_str(), 0)));
}

bool Directory::MakeDir(const string& str)
{
  return ((CreateDirectory(str.c_str(), NULL)) != 0);
}

bool Directory::RemoveDir(const string& str)
{
  return (_rmdir(str.c_str()) == 0);
}

string       merge_strings(const list<string>& array, char c);
list<string> split(const std::string& str, char c);

bool Directory::OpenDir(const string& str)
{
  string          szDir = merge_strings(split(str, '/'), '\\') + "\\*";
  WIN32_FIND_DATA ffd;
  HANDLE          dir = FindFirstFile(szDir.c_str(), &ffd);

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
    return (true);
  }
  return (false);
}
#endif
