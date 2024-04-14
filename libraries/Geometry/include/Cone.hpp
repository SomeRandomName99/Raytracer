#ifndef CONE_HPP
#define CONE_HPP

#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Shape.hpp"

namespace raytracer {
namespace geometry {

class Cone final : public ShapeBase
{
public:
  Cone() noexcept {}
  Cone(double minimum, double maximum, bool closed = false) noexcept : minimum_{minimum}, maximum_{maximum}, 
                                                                       closed_{closed} {}

  utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept;
  std::vector<Intersection> localIntersect(const utility::Ray &transformedRay) const noexcept;

  double minimum_{-std::numeric_limits<double>::infinity()};
  double maximum_{std::numeric_limits<double>::infinity()};
  bool closed_{false};
};

std::shared_ptr<Cone> normalCone() noexcept;

} // namespace geometry
} // namespace raytracer

#endif // CONE_HPP