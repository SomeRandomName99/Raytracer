#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Color.hpp"
#include "Tuple.hpp"

class PointLight{
public:
  PointLight(Color intensity, Tuple position): intensity_{intensity}, position_{position} {}

  Color intensity_;
  Tuple position_;
};

#endif // LIGHT_HPP