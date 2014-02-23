#include "serializer.hpp"

using namespace std;

namespace Utils
{
  // Sweet metaprog shit

  template Packet& Packet::operator<< <std::int32_t>(std::int32_t& i);
  template Packet& Packet::operator<< <float>(float& i);
  template Packet& Packet::operator<< <short>(short& i);
  template Packet& Packet::operator<< <char>(char& i);
  template Packet& Packet::operator<< <unsigned short>(unsigned short& i);
  template Packet& Packet::operator<< <unsigned char>(unsigned char& i);
  template Packet& Packet::operator<< <long>(long&);
  template Packet& Packet::operator<< <std::int32_t>(const std::int32_t& i);
  template Packet& Packet::operator<< <float>(const float& i);
  template Packet& Packet::operator<< <short>(const short& i);
  template Packet& Packet::operator<< <char>(const char& i);
  template Packet& Packet::operator<< <unsigned short>(const unsigned short& i);
  template Packet& Packet::operator<< <unsigned char>(const unsigned char& i);  
  // => There's also a String specialization.

  template Packet& Packet::operator>> <std::int32_t>(std::int32_t& i);
  template Packet& Packet::operator>> <short>(short& i);
  template Packet& Packet::operator>> <char>(char& i);
  template Packet& Packet::operator>> <float>(float& i);
  template Packet& Packet::operator>> <unsigned short>(unsigned short& i);
  template Packet& Packet::operator>> <unsigned char>(unsigned char& i);
  // => Same shit. Also a String specialization.

  template Packet& Packet::operator<< <std::int32_t>(list<int>& list);
  template Packet& Packet::operator<< <float>(list<float>& list);
  template Packet& Packet::operator<< <std::string>(list<std::string>& list);
  template Packet& Packet::operator<< <short>(list<short>& list);
  template Packet& Packet::operator<< <char>(list<char>& list);

  template Packet& Packet::operator<< <std::int32_t>(vector<int>& list);
  template Packet& Packet::operator<< <float>(vector<float>& list);
  template Packet& Packet::operator<< <std::string>(vector<std::string>& list);
  template Packet& Packet::operator<< <short>(vector<short>& list);
  template Packet& Packet::operator<< <char>(vector<char>& list);

  /*
  * Packet methods
  */
  Packet::Packet(void)
  {
    std::int32_t tmp_buffer = 0;

    buffer      = 0;
    sizeBuffer  = 0;
    reading     = 0;
    isDuplicate = true;
    *this << tmp_buffer;
  }
  
  Packet::Packet(std::ifstream& file)
  {
    int            begin, end;
    int            size;
    char*          raw;

    isDuplicate = true;
    begin       = file.tellg();
    file.seekg(0, std::ios::end);
    end         = file.tellg();
    file.seekg(0, std::ios::beg);
    size        = end - begin;
    raw         = new char[size + 1];
    file.read(raw, size);
    file.close();
    raw[size]   = 0;
    buffer      = raw;
    sizeBuffer  = size;
    realloc(size);
    updateHeader();
    std::cout << "Allocated: " << (unsigned int)buffer << ", " << (unsigned int)reading << std::endl;
  }

  Packet::Packet(char* raw, size_t size, bool duplicate) : isDuplicate(duplicate)
  {
    if (duplicate)
    {
      char*	dupRaw = new char[size + 1];

      copy(raw, &raw[size + 1], dupRaw);
      buffer = reinterpret_cast<void*>(dupRaw);
    }
    else
      buffer = raw;
    sizeBuffer = size;
    reading = buffer;
    realloc(size);
    updateHeader();
  }

  Packet::Packet(const char* raw, size_t size) : isDuplicate(true)
  {
      char* dupRaw = new char[size + 1];

      copy(raw, &raw[size + 1], dupRaw);
      buffer = reinterpret_cast<void*>(dupRaw);
      sizeBuffer = size;
      reading = buffer;
      realloc(size);
      updateHeader();
  }

  Packet::~Packet(void)
  {
    if (isDuplicate && buffer)
      delete[] reinterpret_cast<char*>(buffer);
  }

  const char*	Packet::raw(void) const
  {
    return (reinterpret_cast<char*>(buffer));
  }

  size_t		Packet::size(void) const
  {
    return (sizeBuffer);
  }

  /*
  * Serializer
  */
  template<>
  Packet& Packet::operator<< <std::string>(const std::string& str)
  {
    int		newSize = sizeBuffer;
    char*         typeCode;
    std::int32_t*	sizeString;
    char*		dupStr;
    const char*	tmp = str.c_str();

    newSize += (sizeof(std::int32_t));
    newSize += sizeof(char) * str.size() + 1;
    realloc(newSize);
    typeCode = reinterpret_cast<char*>((long)buffer + sizeBuffer);
    sizeString = reinterpret_cast<std::int32_t*>((long)typeCode + sizeof(char));
    dupStr = reinterpret_cast<char*>((long)sizeString + sizeof(std::int32_t));
    *typeCode = Packet::String;
    *sizeString = str.size();
    std::copy(tmp, &tmp[str.size()], dupStr);
    sizeBuffer = newSize;
    updateHeader();
    return (*this);
  }

  template<>
  Packet& Packet::operator<< <std::string>(std::string& str)
  {
    const std::string& ref = str;
    
    this->operator<< <std::string>(ref);
    return (*this);
  }

  template<>
  Packet& Packet::operator>> <std::string>(std::string& str)
  {
    std::int32_t	size;

    str.clear();
    checkType(Packet::String);
    if (!(canIHaz(sizeof(std::int32_t), 1)))
      return (*this);
    size = *(reinterpret_cast<std::int32_t*>(reading));
    reading = reinterpret_cast<void*>((long)reading + sizeof(std::int32_t));
    if (!(canIHaz(sizeof(char), size)))
      return (*this);
    str.append(static_cast<char*>(reading), size);
    reading = reinterpret_cast<void*>((long)reading + sizeof(char) * size);
    return (*this);
  }

  /*
  * Utility Methods for Packet
  */
  bool		Packet::canIHaz(size_t size, int n)
  {
    int		endBuffer, request;

    endBuffer = (long)buffer + sizeBuffer;
    request = (long)reading + (size * n);
    if (endBuffer < request)
    {
      cerr << "[Serializer] Invalid Size: can't read on this Packet anymore." << endl;
      return (false);
    }
    return (true);
  }

  void		Packet::checkType(int assumedType)
  {
    char	        type = -1;

    unsigned int ptr_buff = (unsigned int)buffer;
    unsigned int ptr_read = (unsigned int)reading;

    cout << "Size: " << sizeBuffer << ", buffer: " << ptr_buff << ", reading: " << ptr_read << endl;

    if ((canIHaz(sizeof(char), 1)))
      type = *(reinterpret_cast<char*>(reading));
    reading = reinterpret_cast<void*>((long)reading + sizeof(char));

    //read<char>(type);
    if (type != assumedType)
    {
      int offset = (long)reading - (long)buffer;

      // Hex debug
      int   it  = 0;
      char* hex = (char*)buffer;

      while (it < offset)
      {
          cerr << (int)(hex[it]) << " ";
          ++it;
      }
      cerr << endl;

      cerr << "[Serializer] Type Mismatch: waiting for " << assumedType << ", getting " << (int)type << "..." << endl;
      cerr << "[Serializer] Type Mismatch at offset " << offset << endl;
      throw static_cast<unsigned int>(TypeMismatch);
    }
  }
  
  void          Packet::PrintRawContent(void)
  {
    char*        hex = (char*)buffer;
    unsigned int it  = 0;

    while (it < sizeBuffer)
      cout << (int)(hex[it++]) << ' ';
    cout << endl;
  }

  void		Packet::realloc(int newSize)
  {
    char*		alloc = new char[newSize];

    if (buffer)
    {
      char*	toCopy = static_cast<char*>(buffer);

      copy(toCopy, &toCopy[sizeBuffer], alloc);
      // WARNING may need to be commented:
      delete[] (char*)buffer;
    }
    buffer  = reinterpret_cast<void*>(alloc);
    reading = reinterpret_cast<void*>((long)buffer + sizeof(std::int32_t) + sizeof(char));
  }

  void		Packet::updateHeader(void)
  {
    if (sizeBuffer >= sizeof(std::int32_t))
      *(static_cast<std::int32_t*>(buffer)) = sizeBuffer;
  }

  /*
  * Less important stuff
  */
  void		Packet::PrintContent(void)
  {
    std::cout << "[Packet::PrintContent] isn't implemented yet" << std::endl;
  }
}
