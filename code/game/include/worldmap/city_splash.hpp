#ifndef  CITY_SPLASH_HPP
# define CITY_SPLASH_HPP

# include "ui/rocket_extension.hpp"
# include "datatree.hpp"

class CitySplash : public UiBase
{
  typedef std::map<std::string, RocketListener*> ZoneListeners;
public:
  CitySplash(Data, WindowFramework*, Rocket::Core::Context*);
  ~CitySplash(void);

  Sync::Signal<void (std::string)> EntryZonePicked;
  Sync::Signal<void>               Canceled;
private:
  void                             ZonePicked(Rocket::Core::Event&);

  RocketListener                   CancelClicked;
  ZoneListeners                    zone_listeners;
};

#endif
