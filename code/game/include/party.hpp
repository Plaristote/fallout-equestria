#ifndef  PARTY_HPP
# define PARTY_HPP

# include "globals.hpp"
# include "observatory.hpp"
# include "serializer.hpp"
# include "datatree.hpp"
# include "cmap/statcontroller.hpp"
#include "level/tasks/metabolism.hpp"
# include "world/dynamic_object.hpp"

class ObjectCharacter;
class Inventory;

class Party
{
public:
  class Member
  {
  public:
    Member(void);
    Member(const std::string& name, const std::string& object_name = "");
    Member(ObjectCharacter* character);
    ~Member();

    void                 LinkCharacter(ObjectCharacter*);
    void                 UnlinkCharacter(ObjectCharacter*);
    void                 SaveCharacter(ObjectCharacter*);

    StatController*      GetStatController(void) const { return (statistics); }
    Inventory*           GetInventory(void)      const { return (inventory);  }
    DynamicObject&       GetDynamicObject(void)        { return (object);     }
    const DynamicObject& GetDynamicObject(void)  const { return (object);     }

    void                 Serialize(Utils::Packet&);
    void                 Unserialize(Utils::Packet&);

  private:
    DynamicObject        object;
    DataTree*            statistics_datatree;
    StatController*      statistics;
    Inventory*           inventory;
    Metabolism*          metabolism;
    Utils::Packet        task_set;
  };

  typedef std::list<Member*> PartyMembers;

  Party(void);
  Party(const std::string& savepath, const std::string& name);
  virtual ~Party();
  
  Sync::Signal<void>     Updated;

  void                   SetName(const std::string& name)  { this->name = name; }
  const std::string&     GetName(void)               const { return (name);     }
  
  void                   Join(ObjectCharacter*);
  void                   Join(const std::string& name, const std::string& object_name = "");
  void                   Leave(ObjectCharacter*);
  bool                   IsInParty(ObjectCharacter*) const;
  const PartyMembers&    GetPartyMembers(void)       const { return (members); }
  PartyMembers&          GetPartyMembers(void)             { return (members); }
  Member*                GetMember(const std::string& name) const;
  void                   RemoveDeadMembers(void);

  bool                   Save(const std::string& savepath);
  void                   Serialize(Utils::Packet&);
  void                   Unserialize(Utils::Packet&);
  
  static Party*          NewPartyFromData(Data);

private:  
  PartyMembers           members;
  std::string            name;
};

#endif
