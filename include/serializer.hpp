#ifndef PACKET_HPP
# define PACKET_HPP

# include <string>
# include <list>
# include <vector>
# include <algorithm>
# include <iostream>
# include <fstream>

# ifdef _WIN32
#  include <cstdint>
typedef std::int32_t my_int32;
# else
typedef int32_t	my_int32;
# endif

/*! \namespace Utils
 * \brief Various essentials utilities for all-purpose programming
 */
namespace Utils
{
/*! \class Packet
 * \brief Serialization utility.
 */
class		Packet
{
public:
  enum Types
  {
    String = 1,
    Int    = 2,
    Float  = 3,
    Short  = 4,
    Char   = 5,
    Array  = 6,
    UInt   = 7,
    UShort = 8,
    UChar  = 9
  };

  enum Errors
  {
    TypeMismatch,
    InvalidSize
  };

  /*! \brief Use to create an empty Packet. */
  Packet(void);
  /*! \brief Use to create a packet from a file */
  Packet(std::ifstream&);
  /*! \brief Use to create a packet from some raw data.
   * \param raw : The raw data in the state of a char string.
   * \param size : The size of that same char string.
   * \param duplicate : Define if the Packet must use the raw pointer or a copy of the char string it's pointing to. */
  Packet(char* raw, size_t size, bool duplicate = true);
  Packet(const char* raw, size_t size);
  /*! \brief Use to create a packet from another packet */
  Packet(const Packet& cpy);
  virtual ~Packet(void);

  // Utils
  /*! \brief Returns raw data as a char tab */
  const char*	raw(void) const;
  /*! \brief Returns size of the char tab */
  size_t	size(void) const;
  /*! \brief Prints the content of the packet in an human-readable form */
  void		PrintContent(void);
  
  Packet&	operator=(const Packet& cpy);

  // Serialization
  template<typename T> Packet&	operator<<(T& v);
  template<typename T> Packet&	operator<<(const T& v);
  template<typename T> Packet&	operator<<(std::list<T>& list);
  template<typename T> Packet&	operator<<(std::vector<T>& list);
  // Unserialization
  template<typename T> Packet&	operator>>(T& v);
  template<typename T> Packet&	operator>>(std::list<T>& list);
  template<typename T> Packet&	operator>>(std::vector<T>& list);

private:
  template<typename T> void	SerializeArray(T& list);

  bool		canIHaz(size_t sizeType, int howMany); // Checks if the buffer is big enough for Packet to read size_t
  void		checkType(int assumedType);            // Check if the next type in buffer match the assumed type
  void		realloc(int newsize);                  // Realloc the buffer (used in serializing)
  void		updateHeader(void);                    // Update the size of the packet at the very front of the packet.

  template<typename T>
  void		read(T& tmp)
  {
    if ((canIHaz(sizeof(T), 1)))
      tmp = *(reinterpret_cast<T*>(reading));
    reading = reinterpret_cast<void*>((long)reading + sizeof(T));
  }

  // Association Type/TypeCode
  template<typename type> struct TypeToCode { enum { TypeCode = 0 }; };

  bool          isDuplicate;
  void*		buffer;
  size_t	sizeBuffer;
  void*		reading;
};
}

#endif
