#include <level/objects/locker.hpp>
#include <level/level.hpp>

using namespace std;

ObjectLocker::ObjectLocker(Level* level, DynamicObject* object) : ObjectShelf(level, object), Lockable(object)
{
  _type   = ObjectTypes::Locker;
  _closed = true;
  InstanceDynamicObject::_object = object;

  string anims[] = { "open", "close" };
  for (unsigned int i = 0 ; i < GET_ARRAY_SIZE(anims) ; ++i)
    LoadAnimation(anims[i]);
}

ObjectLocker::~ObjectLocker()
{
}

// Callback for AnimationEnded after CallbackActionUse has been called...
// Apparently you can't call 'super' in a lambda with MSVC2010. Nice...
void ObjectLocker::FuckYouMSVC(InstanceDynamicObject* object)
{
  AnimationEndForObject.DisconnectAll();
  ObjectShelf::ActionUse(object);
}

void ObjectLocker::ActionUse(InstanceDynamicObject* object)
{
  if (_closed == false)
    ObjectShelf::ActionUse(object);
  else if (IsLocked())
    _level->GetLevelUi().GetMainBar().AppendToConsole(i18n::T("It's locked"));
  else
  {
    AnimationEndForObject.DisconnectAll();
    AnimationEndForObject.Connect([this, object](AnimatedObject*)
    {
      FuckYouMSVC(object);
    });
    PlayAnimation("open");
  }
}
