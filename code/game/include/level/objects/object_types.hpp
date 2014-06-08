#ifndef  LEVEL_OBJECT_TYPES_HPP
# define LEVEL_OBJECT_TYPES_HPP

namespace ObjectTypes
{
  enum ObjectType
  {
    Character, Stair, Door, Shelf, Locker, Item, Default, Other
  };
}

template<class C>
struct ObjectType2Code { enum { Type = ObjectTypes::ObjectType::Other }; };

#endif
