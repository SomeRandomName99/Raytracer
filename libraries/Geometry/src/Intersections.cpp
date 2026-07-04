#include <algorithm>
#include <ranges>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <memory>

#include "libraries/Geometry/include/Intersections.hpp"
#include "libraries/Utility/include/FloatUtils.hpp"
#include "libraries/Geometry/include/Shape.hpp"
#include "libraries/Utility/include/Transformations.hpp"

namespace raytracer {
namespace geometry {

using utility::Arena;
using utility::MB;

bool operator==(Intersection const& lhs, Intersection const& rhs) noexcept{
  return utility::floatNearlyEqual(lhs.dist, rhs.dist) && lhs.object == rhs.object;
}

} // namespace geometry
} // namespace raytracer