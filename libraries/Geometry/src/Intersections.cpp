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

} // namespace geometry
} // namespace raytracer