#include "globals.hpp"
#include "my_zlib.hpp"

using namespace std;

Utils::zlib::zlib(char op, const char* _buffer, unsigned long length) : buffer(0), buffer_length(0)
{
  switch (op)
  {
    case 'c':
      Compress(_buffer, length);
      break ;
    case 'd':
      Decompress(_buffer, length);
      break ;
    default:
      break ;
  }
}

Utils::zlib::~zlib()
{
  if (buffer)
    delete[] buffer;
}

const char*   Utils::zlib::GetData(void) const
{
  return (reinterpret_cast<const char*>(buffer));
}

unsigned long Utils::zlib::GetSize(void) const
{
  return (buffer_length);
}

void          Utils::zlib::Compress(const char* _buffer, unsigned long length)
{
  buffer_length = compressBound(length);
  buffer        = new Byte[buffer_length];
  switch (compress(buffer, &buffer_length, reinterpret_cast<const unsigned char*>(_buffer), length))
  {
    case Z_MEM_ERROR:
      throw MemoryError();
    case Z_BUF_ERROR:
      throw BufferError();
    case Z_OK:
      break ;
  }
}

void          Utils::zlib::Decompress(const char* _buffer, unsigned long length)
{
  buffer_length = length * 10;
  buffer        = new Byte[buffer_length];
  switch (uncompress(buffer, &buffer_length, reinterpret_cast<const unsigned char*>(_buffer), length))
  {
    case Z_MEM_ERROR:
      throw MemoryError();
    case Z_BUF_ERROR:
      throw BufferError();
    case Z_DATA_ERROR:
      throw DataError();
    case Z_OK:
      break ;
  }
  buffer[buffer_length] = 0;
}

/*
 * Directory Compressor
 */
void Utils::DirectoryCompressor::Compress(const std::string& target, const std::string& path, std::function<bool (const std::string&)> selector)
{
  std::ofstream file_output;
  Directory     directory;
  Utils::Packet output;
  unsigned int  file_count = 0;

  file_output.open(target, std::ios::binary);
  directory.OpenDir(path);
  // Header
  std::for_each(directory.GetEntries().begin(), directory.GetEntries().end(), [&file_count](Dirent entry)
  {
    if (entry.d_type == DT_REG)
      file_count++;
  });
  output << file_count;
  // Files
  std::for_each(directory.GetEntries().begin(), directory.GetEntries().end(), [&output, path, selector](Dirent entry)
  {
    if (selector(entry.d_name) == false)
      return ;
    if (entry.d_type == DT_REG)
    {
      std::ifstream  input;
      std::streampos begin, end, size;
      char*          raw;

      input.open(path + '/' + entry.d_name, std::ios::binary);
      begin       = input.tellg();
      input.seekg(0, std::ios::end);
      end         = input.tellg();
      input.seekg(0, std::ios::beg);
      size        = end - begin;
      raw         = new char[(int)size + 1];
      input.read(raw, size);
      input.close();
      raw[size]   = 0;
      {
        const std::string data(raw, size);
        const std::string d_name = entry.d_name;

        output << d_name << data;
      }
    }
  });
  // Compressing with zlib
  {
    Utils::zlib compressor('c', output.raw(), output.size());

    file_output.write(compressor.GetData(), compressor.GetSize());
    file_output.close();
  }
}

void Utils::DirectoryCompressor::Uncompress(const std::string& path, const std::string& target)
{
  std::ifstream input;
  
  input.open(path, std::ios::binary);
  if (input.is_open())
  {
    std::streampos begin, end, size;
    char*          raw;

    begin       = input.tellg();
    input.seekg(0, std::ios::end);
    end         = input.tellg();
    input.seekg(0, std::ios::beg);
    size        = end - begin;
    raw         = new char[(int)size + 1];
    input.read(raw, size);
    input.close();
    {
      Utils::zlib   decompressor('d', raw, size);
      Utils::Packet packet(decompressor.GetData(), decompressor.GetSize());
      unsigned int  file_count;

      packet >> file_count;
      cout << "Amount of files" << endl;
      for (unsigned int i = 0 ; i < file_count ; ++i)
      {
        std::ofstream output;
        std::string   name, data;

        packet >> name >> data;
        cout << "Extracting file " << i << " named " << name << endl;
        output.open(target + '/' + name, std::ios::binary);
        output.write(data.data(), data.size());
        output.close();
      }
    }
  }
}
