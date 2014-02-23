#ifndef  CIRCULAR_VALUE_SET_HPP 
# define CIRCULAR_VALUE_SET_HPP

struct CircularValueSet
{
  CircularValueSet(float set_size) : set_size(set_size) {}

  float AdditionDistance    (int goal, int value) const
  {
    float result = 0;

    if (value > goal)
      result = (set_size - value) + goal;
    else if (value < goal)
      result = goal - value;
    return (result);
  }

  float SubstractionDistance(int goal, int value) const
  {
    float result = 0;

    if (value > goal)
      result = value - goal;
    else if (value < goal)
      result = (set_size - goal) + value;
    return (result);
  }

  float set_size;
};

#endif
