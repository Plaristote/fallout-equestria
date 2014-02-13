#ifndef  EXECUTOR_HPP
# define EXECUTOR_HPP

# include "globals.hpp"
# include <queue>
# include <functional>

class Executor
{
  typedef std::queue<std::function<void (void)> > Lambdas;
public:
  static void ExecuteLater(std::function<void (void)> lambda);
  static void Run(void);

private:
  static Lambdas _lambdas;
};

#endif
