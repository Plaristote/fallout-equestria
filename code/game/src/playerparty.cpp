#include "world/world.h"
#include "playerparty.hpp"
#include "level/objects/instance_dynamic_object.hpp"
#include "gametask.hpp"
#include <fstream>

using namespace std;

/*
 * PlayerParty
 */
PlayerParty::PlayerParty(const string& savepath) : Party(savepath, "player")
{
  stat_view = 0;
  Updated.Connect(*this, &PlayerParty::UpdateView);
}

PlayerParty::~PlayerParty()
{
  if (stat_view)
    delete stat_view;
}

StatController* PlayerParty::GetPlayerController(void)
{
  return (GetPlayer()->GetStatController());
}

Inventory* PlayerParty::GetPlayerInventory(void)
{
  return (GetPlayer()->GetInventory());
}

Party::Member* PlayerParty::GetPlayer(void) throw(PlayerNotFound)
{
  auto player = GetMember("self");

  if (player == 0)
    throw PlayerNotFound();
  return (player);
}

void PlayerParty::SetView(StatViewRocket* view)
{
  if (stat_view)
    observers.DisconnectAllFrom(stat_view->SwapToPartyMember);
  stat_view = view;
  observers.Connect(stat_view->SwapToPartyMember, *this, &PlayerParty::SetViewOnPartyMember);
}

void PlayerParty::SetViewOnPartyMember(const string& name)
{
  if (stat_view)
  {
    Party::Member*  party_member = GetMember(name);

    if (party_member)
    {
      StatController* controller   = party_member->GetStatController();

      controller->SetView(stat_view);
    }
  }
}

void PlayerParty::UpdateView(void)
{
  if (stat_view)
  {
    PartyMembers&  party_members = GetPartyMembers();
    vector<string> names;
    
    for_each(party_members.begin(), party_members.end(), [&names](Party::Member* member)
    {
      names.push_back(member->GetDynamicObject().name);
    });
    stat_view->SetPartyMembers(names);
  }
}

/*void Party::Export(const std::string& name) const
{
  cout << "Party::Export" << endl;
  const string& savepath = GameTask::CurrentGameTask->GetSavePath();
  string        path     = savepath + "/party-" + name + ".blob";
  ofstream      file;
  Utils::Packet packet;

  file.open(path.c_str(), ios::binary);
  if (file.is_open())
  {
    Serialize(packet);
    file.write(packet.raw(), packet.size());
    file.close();
  }
  ForEach(_objects, [savepath](DynamicObject* object)
  {
    ObjectCharacter* character = Level::CurrentLevel->GetCharacter(object);

    if (character)
      DataTree::Writers::JSON(character->GetStatistics(),
                              savepath + "/stats-" + object->charsheet + ".json");
  });
}

Party* Party::Import(const std::string& name)
{
  cout << "Party::Import" << endl;
  Level*        level    = Level::CurrentLevel;

  if (level)
  {
    auto it  = level->_parties.begin();
    auto end = level->_parties.end();

    while (it != end)
    {
      if ((*it)->_name == name)
        return (0);
    }
    {
      Party*        party    = new Party;
      const string& savepath = GameTask::CurrentGameTask->GetSavePath();
      string        save_party = savepath + "/party-" + name + ".blob";
      ifstream      file(save_party.c_str());

      party->_name = name;
      if (file.is_open())
      {
        Utils::Packet packet(file);

        party->UnSerialize(packet);
      }
      ForEach(party->_objects, [level](DynamicObject* object)
      {
        ObjectCharacter* character;

        level->GetWorld()->InsertDynamicObject(*object);
        level->InsertDynamicObject(*object);
        character = level->GetCharacter(object);
        if (character)
          character->SetOccupiedWaypoint(0);
      });
      level->_parties.push_back(party);
      return (party);
    }
  }
  return (0);
}*/
