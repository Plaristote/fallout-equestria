#ifndef  OBJECT_OUTLINE_HPP
# define OBJECT_OUTLINE_HPP

# include "globals.hpp"
# include <panda3d/lvector4.h>
# include <panda3d/nodePath.h>
# include <list>
# include <vector>

class InstanceDynamicObject;
class ObjectCharacter;

class TargetOutliner
{
  class Outline
  {
    friend class TargetOutliner;
  public:
    Outline(InstanceDynamicObject* object);

    void     Initialize(void);
    void     Finalize(void);
    void     Show(void);
    void     Hide(void);
    void     SetColor(LVector4f);
    
  private:
    NodePath target;
    NodePath outline;
  };

public:
  ~TargetOutliner()
  {
    DisableOutline();
  }
  
  void UsePerspectiveOfCharacter(ObjectCharacter* subject);
  void EnableOutline(void);
  void DisableOutline(void);

private:
  void MakeOutlinesForListWithColor(const std::vector<ObjectCharacter*>& targets, LVector4f color);

  ObjectCharacter*   subject;
  std::list<Outline> outlines;
};

#endif
