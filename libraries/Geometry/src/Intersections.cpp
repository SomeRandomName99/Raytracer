#include <algorithm>
#include <ranges>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Intersections.hpp"
#include "FloatUtils.hpp"
#include "Shape.hpp"
#include "Transformations.hpp"

namespace raytracer {
namespace geometry {

using utility::Arena;
using utility::MB;

bool operator==(Intersection const& lhs, Intersection const& rhs) noexcept{
  return utility::floatNearlyEqual(lhs.dist, rhs.dist) && lhs.object == rhs.object;
}

double schlick(const Computations& comps) noexcept{
  auto cos = comps.eyeVector.dot(comps.normalVector);
  if(comps.n1 > comps.n2){
    auto n = comps.n1 / comps.n2;
    auto sin2_t = n * n * (1 - cos * cos);
    if(sin2_t > 1) // total internal reflection
      return 1.0;

    auto cos_t = std::sqrt(1.0 - sin2_t);
    cos = cos_t;
  }

  auto r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
  return r0 + (1 - r0) * std::pow(1 - cos, 5);
}

} // namespace geometry
} // namespace raytracer