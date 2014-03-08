#ifndef  FIELD_OF_VIEW_HPP
# define FIELD_OF_VIEW_HPP

# include "globals.hpp"
# include "scheduled_task.hpp"
# define FLAG_CHARACTER_SNEAK 1
# define FOV_TTL              5

class Level;
class ObjectCharacter;

class FieldOfView : public ScheduledTask
{
  typedef std::vector<ObjectCharacter*> CharacterList;

  struct Entry
  {
    Entry(ObjectCharacter* character) : character(character), time_to_live(FOV_TTL)
    {
    }

    bool             operator==(const ObjectCharacter* comp) const { return (character == comp); }
    ObjectCharacter& operator*(void) const { return (*character); }

    ObjectCharacter* character;
    char             time_to_live;
  };

public:
  FieldOfView(Level& level, ObjectCharacter& character);
  void                 SetIntervalDurationFromStatistics(void);

  bool                 IsDetected(const ObjectCharacter*) const;
  bool                 HasLivingEnemiesInSight(void) const;
  
  void                 SetEnemyDetected(ObjectCharacter& enemy);
  void                 SetCharacterDetected(ObjectCharacter& character);
  CharacterList        GetCharactersInRange(void)  const;
  CharacterList        GetDetectedEnemies(void)    const;
  CharacterList        GetDetectedAllies(void)     const;
  CharacterList        GetDetectedNonHostile(void) const;
  CharacterList        GetDetectedCharacters(void) const;
  float                GetRadius(void)             const;
  
  void                 RunCheck(void);
  void                 MarkForUpdate(void) { needs_update = true; }

protected:
  void                 Run(void);

  void                 LoseTrackOfCharacters(std::list<Entry>&);
  void                 DetectCharacters(void);
  bool                 CheckIfEnemyIsDetected(const ObjectCharacter& enemy)                  const;
  bool                 CheckIfSneakingEnemyIsDetected(const ObjectCharacter& enemy)          const;
  void                 InsertOrUpdateCharacterInList(ObjectCharacter&, std::list<Entry>&);
  bool                 IsCharacterInList(const ObjectCharacter*, const std::list<Entry>&)    const;
  void                 AppendEntriesToCharacterList(const std::list<Entry>&, CharacterList&) const;
  CharacterList        GetDetectedCharactersMatching(std::function<bool (ObjectCharacter*)>) const;
  
private:
  Level&               level;
  ObjectCharacter&     character;
  bool                 needs_update;
  std::list<Entry>     detected_enemies;
  std::list<Entry>     detected_characters;
};

#endif
