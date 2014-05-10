#ifndef  OBJECT_NODE_HPP
# define OBJECT_NODE_HPP

# include "globals.hpp"
# include <panda3d/pandaFramework.h>
# include <panda3d/pandaSystem.h>
# include <panda3d/texturePool.h>
# include <panda3d/animControlCollection.h>
# include "datatree.hpp"
# include "observatory.hpp"
# include "world/world.h"
# include "level/pathfinding/collider.hpp"
# include "level/pathfinding/path.hpp"
# include "level/inventory.hpp"
# include "animatedobject.hpp"
# include "level/skill_target.hpp"
# include "level/tasks/task_set.hpp"
# include "level/objects/object_types.hpp"
# include "level/interactions/target.hpp"

//HAIL MICROSOFT
#ifdef WIN32
static inline double round(double val)
{
    return floor(val + 0.5);
}
#endif

class Level;
class ObjectCharacter;
class ISampleInstance;

class InstanceDynamicObject : public virtual Pathfinding::Collider, public AnimatedObject, public Interactions::Target
{
  friend class SkillTarget;
public:
  InstanceDynamicObject(Level* level, DynamicObject* object);
  virtual ~InstanceDynamicObject();

  virtual void              Unserialize(Utils::Packet&);
  virtual void              Serialize(Utils::Packet&);
  virtual void              Run(float elapsedTime)                    { TaskAnimation();                               }
  bool                      operator==(NodePath np)             const { return (_object->nodePath.is_ancestor_of(np)); }
  bool                      operator==(const std::string& name) const { return (GetName() == name);                    }
  virtual std::string       GetName(void)                       const { return (_object->name);                        }
  Level*                    GetLevel(void)                      const { return (_level);                               }
  NodePath                  GetNodePath(void)                   const { return (_object->nodePath);                    }
  LPoint3                   GetSize(void)                       const { return (idle_size);                            }
  const std::string&        GetDialog(void)                     const { return (_object->dialog);                      }
  DynamicObject*            GetDynamicObject(void)                    { return (_object);                              }
  const DynamicObject*      GetDynamicObject(void)              const { return (_object);                              }
  TaskSet&                  GetTaskSet(void)                          { return (tasks);                                }
  Data                      GetDataStore(void)                  const { return (data_store);                           }

  void                      AddTextBox(const std::string& text, unsigned short r, unsigned short g, unsigned short b, float timeout = 5.f);
  ISampleInstance*          PlaySound(const std::string& name);
  
  template<class C>
  C*                        Get(void)
  {
    if (ObjectType2Code<C>::Type == _type)
      return (reinterpret_cast<C*>(this));
    return (0);
  }

private:
  void                      SerializeDataStore(Utils::Packet&);
  void                      UnserializeDataStore(Utils::Packet&);

protected:
  Level*                   _level;
  unsigned char            _type;
  DynamicObject*           _object;
  TaskSet                  tasks;
  LPoint3                  idle_size;
  DataTree*                data_store;
};


#endif
