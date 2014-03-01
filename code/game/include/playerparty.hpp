#ifndef  PLAYERPARTY_HPP
# define PLAYERPARTY_HPP

# include "globals.hpp"
# include "party.hpp"
# include "level/objects/character.hpp"
# include "ui/stat_view_rocket.hpp"

class PlayerParty : public Party
{
public:
  PlayerParty() : stat_view(0) {}
  PlayerParty(const std::string& savepath);
  ~PlayerParty(void);

  void                  SetView(StatViewRocket* view);

  StatController*       GetPlayerController(void);
  Inventory*            GetPlayerInventory(void);
  Party::Member*        GetPlayer(void);
  
private:
  
  void                  UpdateView(void);
  void                  SetViewOnPartyMember(const std::string&);
  
  Sync::ObserverHandler observers;
  StatViewRocket*       stat_view;
};

#endif