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
  Cone() noexcept;
  Cone(float minimum, float maximum, bool closed = false) noexcept;

  utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept;
  std::vector<Intersection> localIntersect(const utility::Ray &transformedRay) const noexcept;

  float minimum_{-std::numeric_limits<float>::infinity()};
  float maximum_{std::numeric_limits<float>::infinity()};
  bool closed_{false};
};

std::shared_ptr<Cone> makeCone() noexcept;

} // namespace geometry
} // namespace raytracer

#endif // CONE_HPP