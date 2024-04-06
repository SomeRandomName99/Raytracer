#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>
#include <optional>
#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Material.hpp"
#include "Matrix.hpp"

constexpr double SHADOW_OFFSET = 1e-6;

namespace raytracer {
namespace geometry {
class ShapeBase;

/**
 * \class Intersection
 * \brief Save information about an intersection between a ray and a geometry object.
 */
/*
 * TODO: Use a reference wrapper instead of a pointer in order to make sure that the pointer is never null.
*/
struct Intersection
{
  const ShapeBase *object;
  double dist; ///< Distance from the ray origin to the intersection point.
};

bool operator==(Intersection const& lhs, Intersection const& rhs) noexcept;

struct Computations{
  Intersection intersection;
  utility::Tuple normalVector; 
  utility::Tuple point;
  utility::Tuple overPoint; // used for shadows
  utility::Tuple underPoint; // used for refractions
  utility::Tuple eyeVector;
  utility::Tuple reflectVector;
  double n1; // refractive indices
  double n2;
  bool inside;
};


Computations prepareComputations(Intersection intersection, const utility::Ray& ray, 
                                 const std::vector<Intersection>& intersections = {}) noexcept;

/**
 * @brief Creates a vector of intersections from the given arguments.
 */
template<std::same_as<Intersection> ... Arg> 
auto intersections(const Arg&... arg) noexcept{
  return std::vector<Intersection>{arg...};
}

/**
 * @brief A function that returns the intersection with the least distance.
 * 
 * This is used to find the first object that was hit in order to display it first.
 */
std::optional<Intersection> hit(const std::vector<Intersection>& intersections) noexcept;

} // namespace geometry
} // namespace raytracer

#endif // INTERSECTIONS_H