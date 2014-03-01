#ifndef  PLAYERPARTY_HPP
# define PLAYERPARTY_HPP

# include "globals.hpp"
# include "party.hpp"
# include "level/objects/character.hpp"
# include "ui/stat_view_rocket.hpp"

class PlayerParty : public Party
{
public:
  struct PlayerNotFound : public std::exception { const char* what(void) const throw() { return ("The player's character was not found: party-player.blob file might be corrupted."); } };

  PlayerParty() : stat_view(0) {}
  PlayerParty(const std::string& savepath);
  ~PlayerParty(void);

  void                  SetView(StatViewRocket* view);

  StatController*       GetPlayerController(void);
  Inventory*            GetPlayerInventory(void);
  Party::Member*        GetPlayer(void) throw(PlayerNotFound);

private:
  
  void                  UpdateView(void);
  void                  SetViewOnPartyMember(const std::string&);
  
  Sync::ObserverHandler observers;
  StatViewRocket*       stat_view;
};

#endif
