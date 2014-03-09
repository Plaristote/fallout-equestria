#ifndef  MY_ZLIB_HPP
# define MY_ZLIB_HPP

# include <zlib.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <serializer.hpp>
#include <directory.hpp>

namespace Utils
{
  class zlib
  {
  public:
    struct Exception : public std::exception
    {
    public:
      Exception(const std::string& message, int error_code) : message("zlib call failed because " + message), error_code(error_code)
      {
      }

      const char* what(void) const throw() { return (message.c_str()); }
      int         error_number(void) const throw() { return (error_code);      }
      
    private:
      std::string message;
      int         error_code;
    };
    
    struct BufferError : public Exception { BufferError() : Exception("there was not enough memory",                   Z_BUF_ERROR)  {} };
    struct MemoryError : public Exception { MemoryError() : Exception("there wasn't enough room in the output buffer", Z_MEM_ERROR)  {} };
    struct DataError   : public Exception { DataError()   : Exception("the input data was corrupted or incomplete",    Z_DATA_ERROR) {} };
    
    
    zlib(char op, const char* _buffer, unsigned long length);
    ~zlib();

    const char*   GetData(void) const;
    unsigned long GetSize(void) const;

  private:
    void          Compress(const char* _buffer, unsigned long length);
    void          Decompress(const char* _buffer, unsigned long length);
    
    Byte*         buffer;
    unsigned long buffer_length;
  };
  
  class DirectoryCompressor
  {
    DirectoryCompressor() {}
  public:
    static void Compress(const std::string& target, const std::string& path, std::function<bool (const std::string&)> selector = [](const std::string&) { return (true); });
    static void Uncompress(const std::string& path, const std::string& target);
  };
}

#endif
