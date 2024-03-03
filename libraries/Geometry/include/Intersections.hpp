#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>
#include <optional>
#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"

constexpr double SHADOW_OFFSET = 1e-6;

namespace raytracer {
namespace geometry {

class Material;


/**
 * \class Intersection
 * \brief Save information about an intersection between a ray and a geometry object.
 */
struct Intersection
{
  utility::Tuple normalVector; 
  const Material* material;
  double dist; ///< Distance from the ray origin to the intersection point.
};

bool operator==(Intersection const& lhs, Intersection const& rhs) noexcept;

struct Computations{
  Intersection intersection;
  utility::Tuple point;
  utility::Tuple overPoint; // used for shadows
  utility::Tuple eyeVector;
  bool inside;
};


Computations prepareComputations(Intersection intersection, const utility::Ray& ray);

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