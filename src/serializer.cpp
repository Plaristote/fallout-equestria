#include "serializer.hpp"

using namespace std;

namespace Utils
{
  // Sweet metaprog shit
  template<> struct Packet::TypeToCode<string>         { enum { TypeCode = Packet::String }; };
  template<> struct Packet::TypeToCode<int>            { enum { TypeCode = Packet::Int    }; };
  template<> struct Packet::TypeToCode<float>          { enum { TypeCode = Packet::Float  }; };
  template<> struct Packet::TypeToCode<short>          { enum { TypeCode = Packet::Short  }; };
  template<> struct Packet::TypeToCode<char>           { enum { TypeCode = Packet::Char   }; };
  template<> struct Packet::TypeToCode<unsigned int>   { enum { TypeCode = Packet::UInt }; };
  template<> struct Packet::TypeToCode<unsigned short> { enum { TypeCode = Packet::UShort }; };
  template<> struct Packet::TypeToCode<unsigned char>  { enum { TypeCode = Packet::UChar  }; };

  template Packet& Packet::operator<< <my_int32>(my_int32& i);
  template Packet& Packet::operator<< <float>(float& i);
  template Packet& Packet::operator<< <short>(short& i);
  template Packet& Packet::operator<< <char>(char& i);
  template Packet& Packet::operator<< <unsigned int>(unsigned int& i);
  template Packet& Packet::operator<< <unsigned short>(unsigned short& i);
  template Packet& Packet::operator<< <unsigned char>(unsigned char& i);
  template Packet& Packet::operator<< <my_int32>(const my_int32& i);
  template Packet& Packet::operator<< <float>(const float& i);
  template Packet& Packet::operator<< <short>(const short& i);
  template Packet& Packet::operator<< <char>(const char& i);
  template Packet& Packet::operator<< <unsigned int>(const unsigned int& i);
  template Packet& Packet::operator<< <unsigned short>(const unsigned short& i);
  template Packet& Packet::operator<< <unsigned char>(const unsigned char& i);  
  // => There's also a String specialization.

  template Packet& Packet::operator>> <int>(int& i);
  template Packet& Packet::operator>> <short>(short& i);
  template Packet& Packet::operator>> <char>(char& i);
  template Packet& Packet::operator>> <float>(float& i);
  template Packet& Packet::operator>> <unsigned int>(unsigned int& i);
  template Packet& Packet::operator>> <unsigned short>(unsigned short& i);
  template Packet& Packet::operator>> <unsigned char>(unsigned char& i);
  // => Same shit. Also a String specialization.

  template Packet& Packet::operator<< <int>(list<int>& list);
  template Packet& Packet::operator<< <float>(list<float>& list);
  template Packet& Packet::operator<< <std::string>(list<std::string>& list);
  template Packet& Packet::operator<< <short>(list<short>& list);
  template Packet& Packet::operator<< <char>(list<char>& list);

  template Packet& Packet::operator<< <int>(vector<int>& list);
  template Packet& Packet::operator<< <float>(vector<float>& list);
  template Packet& Packet::operator<< <std::string>(vector<std::string>& list);
  template Packet& Packet::operator<< <short>(vector<short>& list);
  template Packet& Packet::operator<< <char>(vector<char>& list);

  template Packet& Packet::operator>> <int>(list<int>& list);
  template Packet& Packet::operator>> <float>(list<float>& list);
  template Packet& Packet::operator>> <short>(list<short>& list);
  template Packet& Packet::operator>> <char>(list<char>& list);
  template Packet& Packet::operator>> <std::string>(list<std::string>& list);

  template Packet& Packet::operator>> <int>(vector<int>& list);
  template Packet& Packet::operator>> <float>(vector<float>& list);
  template Packet& Packet::operator>> <short>(vector<short>& list);
  template Packet& Packet::operator>> <char>(vector<char>& list);
  template Packet& Packet::operator>> <std::string>(vector<std::string>& list);

  /*
  * Packet methods
  */
  Packet::Packet(void)
  {
    buffer      = 0;
    sizeBuffer  = 0;
    reading     = 0;
    isDuplicate = true;
    *this << (my_int32&)sizeBuffer;
  }
  
  Packet::Packet(std::ifstream& file)
  {
    long           begin, end;
    long           size;
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

  template<typename T>
  Packet&		Packet::operator<<(T& i)
  {
    int	    newSize = sizeBuffer;
    char*   typeCode;
    T*	    copy;

    newSize += sizeof(T) + sizeof(char);
    realloc(newSize);
    typeCode = reinterpret_cast<char*>((long)buffer + sizeBuffer);
    copy = reinterpret_cast<T*>((long)typeCode + sizeof(char));
    *typeCode = TypeToCode<T>::TypeCode;
    *copy = i;
    sizeBuffer = newSize;
    updateHeader();
    return (*this);
  }
  
  template<typename T>
  Packet&		Packet::operator<<(const T& i)
  {
    int	    newSize = sizeBuffer;
    char*   typeCode;
    T*	    copy;

    newSize += sizeof(T) + sizeof(char);
    realloc(newSize);
    typeCode = reinterpret_cast<char*>((long)buffer + sizeBuffer);
    copy = reinterpret_cast<T*>((long)typeCode + sizeof(char));
    *typeCode = TypeToCode<T>::TypeCode;
    *copy = i;
    sizeBuffer = newSize;
    updateHeader();
    return (*this);
  }

  template<>
  Packet& Packet::operator<< <string>(const string& str)
  {
    int		newSize = sizeBuffer;
    char*         typeCode;
    my_int32*	sizeString;
    char*		dupStr;
    const char*	tmp = str.c_str();

    newSize += (sizeof(my_int32));
    newSize += sizeof(char) * str.size() + 1;
    realloc(newSize);
    typeCode = reinterpret_cast<char*>((long)buffer + sizeBuffer);
    sizeString = reinterpret_cast<my_int32*>((long)typeCode + sizeof(char));
    dupStr = reinterpret_cast<char*>((long)sizeString + sizeof(my_int32));
    *typeCode = Packet::String;
    *sizeString = str.size();
    copy(tmp, &tmp[str.size()], dupStr);
    sizeBuffer = newSize;
    updateHeader();
    return (*this);
  }

  template<>
  Packet& Packet::operator<< <string>(string& str)
  {
    const string& ref = str;
    
    this->operator<< <string>(ref);
    return (*this);
  }

  template<typename T>
  void				Packet::SerializeArray(T& tehList)
  {
    typename T::iterator		current = tehList.begin();
    typename T::iterator		end = tehList.end();
    int				newSize = sizeBuffer;
    char*			        typeCode;
    my_int32*			sizeArray;

    newSize += sizeof(char) + sizeof(my_int32);
    realloc(newSize);
    typeCode = reinterpret_cast<char*>((long)buffer + sizeBuffer);
    sizeArray = reinterpret_cast<my_int32*>((long)typeCode + sizeof(char));
    *typeCode = Packet::Array;
    *sizeArray = tehList.size();
    sizeBuffer = newSize;
    while (current != end)
    {
      *this << *current;
      current++;
    }
    updateHeader();
  }


  template<typename T>
  Packet&		Packet::operator<<(list<T>& tehList)
  {
    SerializeArray(tehList);
    return (*this);
  }

  template<typename T>
  Packet&		Packet::operator<<(vector<T>& tehList)
  {
    SerializeArray(tehList);
    return (*this);
  }

  /*
  * Unserializer
  */
  template<typename T>
  Packet&		Packet::operator>>(T& v)
  {
    checkType(TypeToCode<T>::TypeCode);
    v = 0;
    read<T>(v);
    return (*this);
  }

  template<>
  Packet&		Packet::operator>> <string>(string& str)
  {
    my_int32	size;

    str.clear();
    checkType(Packet::String);
    if (!(canIHaz(sizeof(my_int32), 1)))
      return (*this);
    size = *(reinterpret_cast<my_int32*>(reading));
    reading = reinterpret_cast<void*>((long)reading + sizeof(my_int32));
    if (!(canIHaz(sizeof(char), size)))
      return (*this);
    str.append(static_cast<char*>(reading), size);
    reading = reinterpret_cast<void*>((long)reading + sizeof(char) * size);
    return (*this);
  }

  // TODO: find a way to template this bullshit (list & vector unserializer)
  template<typename T>
  Packet&		Packet::operator>>(list<T>& list)
  {
    unsigned int	size, it;
    my_int32	tmp = 0;

    list.clear();
    checkType(Packet::Array);
    read<my_int32>(tmp);
    size = tmp;
    for (it = 0 ; it < size ; ++it)
    {
      T		reading;

      *this >> reading;
      list.push_back(reading);
    }
    return (*this);
  }

  template<typename T>
  Packet&		Packet::operator>>(vector<T>& list)
  {
    unsigned int	size, it;
    my_int32	tmp = 0;

    list.clear();
    checkType(Packet::Array);
    read<my_int32>(tmp);
    size = tmp;
    for (it = 0 ; it < size ; ++it)
    {
      T		reading;

      *this >> reading;
      list.push_back(reading);
    }
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

    read<char>(type);
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
    reading = reinterpret_cast<void*>((long)buffer + sizeof(my_int32) + sizeof(char));
  }

  void		Packet::updateHeader(void)
  {
    if (sizeBuffer >= sizeof(int))
      *(static_cast<int*>(buffer)) = sizeBuffer;
  }

  /*
  * Less important stuff
  */
  void		Packet::PrintContent(void)
  {
    std::cout << "[Packet::PrintContent] isn't implemented yet" << std::endl;
  }
}
