#include <Boots/functorthread.hpp>

namespace Sync
{
  template<>
  void FunctorThread<void>::RunFunctor(void)
  {
    functor();
    Done.Emit();
  }
}
