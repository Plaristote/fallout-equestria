#include "executor.hpp"
#include <algorithm>

using namespace std;

Executor::Lambdas Executor::_lambdas;

void Executor::ExecuteLater(std::function<void (void)> lambda)
{
  _lambdas.push(lambda);
}

void Executor::Run(void)
{
  while (_lambdas.size())
  {
    _lambdas.front()();
    _lambdas.pop();
  }
}