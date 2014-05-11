#include "party.hpp"
#include "level/objects/character.hpp"
#include "level/inventory.hpp"
#include "datatree.hpp"
#include <fstream>

using namespace std;

/*
 * Party Member
 */
Party::Member::Member(void) : statistics_datatree(0), statistics(0), metabolism(0)
{
  inventory = new Inventory;
  TaskSet(0).Serialize(task_set);
  object.type          = DynamicObject::Character;
  object.collider.type = Collider::MODEL;
}

Party::Member::Member(const std::string& name, const std::string& object_name)
{
  const std::string path = "data/charsheets/" + name + ".json";
  Data              data;

  cout << ">> character joining: " << endl;
  statistics_datatree = DataTree::Factory::JSON(path);
  if (statistics_datatree)
  {
    statistics        = new StatController(statistics_datatree);
    data              = Data(statistics_datatree);
  }
  metabolism          = new Metabolism(statistics);
  object.type         = DynamicObject::Character;
  object.collider.type= Collider::MODEL;
  object.name         = object_name == "" ? name : object_name;
  object.charsheet    = name;
  object.script       = data["Behaviour"]["script"].Value();
  object.dialog       = data["Behaviour"]["dialog"].Value();
  object.interactions = (int)(data["Behaviour"]["interactions"].Or(Interactions::UseObject |
                                                                   Interactions::UseSkill  |
                                                                   Interactions::UseSpell  |
                                                                   Interactions::LookAt    |
                                                                   Interactions::TalkTo));
  object.strModel     = data["Appearance"]["model"].Value();
  object.strTexture   = data["Appearance"]["texture"].Value();
  object.waypoint     = 0;
  inventory           = new Inventory;
  if (data["Inventory"].NotNil())
    inventory->LoadInventory(data["Inventory"]);
  TaskSet(0).Serialize(task_set);
}

Party::Member::Member(ObjectCharacter* character)
{
  object              = *(character->GetDynamicObject());
  statistics_datatree = new DataTree;
  Data(statistics_datatree).Duplicate(character->GetStatistics());
  statistics          = new StatController(statistics_datatree);
  metabolism          = character->GetMetabolism();
  character->SetMetabolism(0);
  character->ForceStatController(statistics);
  inventory           = new Inventory;
  TaskSet(0).Serialize(task_set);
}

Party::Member::~Member()
{
  if (statistics_datatree)
  {
    delete metabolism;
    delete statistics;
    delete statistics_datatree;
  }
  if (inventory)
    delete inventory;
}

void Party::Member::SaveCharacter(ObjectCharacter* character)
{
  DynamicObject& character_object = *(character->GetDynamicObject());
  
  task_set.Clear();
  object.name         = character_object.name;
  object.dialog       = character_object.dialog;
  object.script       = character_object.script;
  object.interactions = character_object.interactions;
  object.strModel     = character_object.strModel;
  object.strTexture   = character_object.strTexture;
  character->GetTaskSet().Serialize(task_set);
}

void Party::Member::LinkCharacter(ObjectCharacter* character)
{
  character->ForceStatController(statistics);
  character->SetMetabolism(0);
  character->SetInventory(inventory);
  character->GetTaskSet().Unserialize(task_set);
}

void Party::Member::UnlinkCharacter(ObjectCharacter* character)
{
  character->ForceStatDatatree(statistics_datatree);
  character->SetMetabolism(metabolism);
  statistics_datatree = 0;
  statistics          = 0;
  inventory           = 0;
  metabolism          = 0;
}

void Party::Member::Serialize(Utils::Packet& packet)
{
  std::string statistics_json;
  
  DataTree::Writers::StringJSON(statistics->GetData(), statistics_json);
  task_set.Serialize(packet);
  packet << statistics_json;
  inventory->SaveInventory(&object);
  packet << object;
  metabolism->Serialize(packet);
}

void Party::Member::Unserialize(Utils::Packet& packet)
{
  std::string statistics_json;
  bool workaround_bool = false;
  
  task_set.Clear();
  task_set.Unserialize(packet);
  packet >> statistics_json;
  try {
  packet >> object;
  } catch (...) { workaround_bool = true; }
  object.type         = DynamicObject::Character;
  statistics_datatree = DataTree::Factory::StringJSON(statistics_json);
  statistics          = new StatController(statistics_datatree);
  metabolism          = new Metabolism(statistics);
  if (!workaround_bool)
  metabolism->Unserialize(packet);
  inventory->LoadInventory(&object);
}

/*
 * Party
 */
Party::Party(void)
{

}

Party::Party(const string& savepath, const string& name) : name(name)
{
  string        save_party = savepath + "/party-" + name + ".blob";
  ifstream      file(save_party.c_str());

  if (file.is_open())
  {
    Utils::Packet packet(file);

    Unserialize(packet);
  }
}

Party::~Party()
{
  cout << "Deleting party " << GetName() << endl;
  for_each(members.begin(), members.end(), [](Member* member)
  {
    delete member;
    cout << "Member deleted" << endl;
  });
  cout << "All members deleted" << endl;
}

void Party::Join(ObjectCharacter* character)
{
  if (!(IsInParty(character)))
  {
    Member* member = new Member(character);

    members.push_back(member);
    Updated.Emit();
  }
}

void Party::Join(const std::string& name, const std::string& object_name)
{
  Member* member = new Member(name, object_name);
  
  members.push_back(member);
  Updated.Emit();
}

void Party::Leave(ObjectCharacter* character)
{
  auto it_member = find_if(members.begin(), members.end(), [character](Member* member)
  { return (member->GetStatController() == character->GetStatController()); });
  
  if (it_member != members.end())
  {
    Member* member = *it_member;

    member->UnlinkCharacter(character);
    delete member;
    members.erase(it_member);
    Updated.Emit();
  }
}

bool Party::IsInParty(ObjectCharacter* compare_to) const
{
  if (compare_to)
  {
    auto member = find_if(members.begin(), members.end(), [compare_to](Member* member)
    { return (member->GetStatController() == compare_to->GetStatController()); });
    
    return (member != members.end());
  }
  return (false);
}

Party::Member* Party::GetMember(const std::string& name) const
{
  auto member = find_if(members.begin(), members.end(), [name](Member* member)
  { return (member->GetDynamicObject().name == name); });

  if (member != members.end())
    return (*member);
  return (0);
}

bool Party::Save(const string& savepath)
{
  string        path = savepath + "/party-" + name + ".blob";
  ofstream      file;
  Utils::Packet packet;

  file.open(path.c_str(), ios::binary);
  if (file.is_open())
  {
    Serialize(packet);
    file.write(packet.raw(), packet.size());
    file.close();
    return (true);
  }
  return (false);
}

void Party::Serialize(Utils::Packet& packet)
{
  unsigned int member_count = members.size();
  
  packet << member_count;
  for_each(members.begin(), members.end(), [&packet](Member* member)
  {
    member->Serialize(packet);
  });
}

void Party::Unserialize(Utils::Packet& packet)
{
  unsigned int member_count;
  
  packet >> member_count;
  for (unsigned int i = 0 ; i < member_count ; ++i)
  {
    Member* member = new Member;
    
    member->Unserialize(packet);
    members.push_back(member);
  }
}

void Party::RemoveDeadMembers(void)
{
  auto it  = members.begin();
  auto end = members.end();
  
  for (; it != end ; ++it)
  {
    StatController* statistics = (*it)->GetStatController();
    int             hit_points = statistics->GetData()["Variables"]["Hit Points"];

    if (hit_points <= 0)
    {
      delete *it;
      members.erase(it);
    }
    else
      ++it;
  }
}

Party* Party::NewPartyFromData(Data data)
{
  Party* party = new Party;
  
  party->name = data["name"].Value();
  for_each(data["members"].begin(), data["members"].end(), [party](Data member_data)
  {
    unsigned int count = member_data.Or(1);
    unsigned int clones = 0;

    for (; clones < count ; ++clones)
    {
      stringstream unique_name;

      unique_name << "member-" << party->name << "-" << count;
      party->Join(member_data.Key(), unique_name.str());
    }
  });
  return (party);
}
