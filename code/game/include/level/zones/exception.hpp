#ifndef  ZONES_EXCEPTION_HPP
# define ZONES_EXCEPTION_HPP

# include <exception>
# include <string>

namespace Zones
{
  class ZoneException : public std::exception
  {
  public:
    ZoneException(const std::string& zone_name, const std::string& message = "") : zone_name(zone_name), message(message)
    {
    }
    
    virtual const char* what() const throw();

    void Display();

  private:
    const std::string zone_name;
    const std::string message;
  };

  struct ZoneNotFound : public ZoneException
  {
    ZoneNotFound(const std::string& zone_name, const std::string& message = "") : ZoneException(zone_name, message) {}
  };

  struct ZoneIsFull : public ZoneException
  {
    ZoneIsFull(const std::string& zone_name, const std::string& message = "") : ZoneException(zone_name, message) {}
  };
}

#endif