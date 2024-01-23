#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>
#include <optional>
#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"

namespace raytracer {
namespace geometry {

class Sphere;

/**
 * @brief A class that stores relevant info regarding an intersecion
 * 
 * The class include store the t value which is used in calculating the distance from the ray's origin, as well as
 * the object that was intersected with. For now it is a hardcoded sphere but will be generalized later. 
 */
class Intersection
{
public:
  Intersection(double t, std::shared_ptr<Sphere> object) : t_{t}, object_{object} {}

  double t_;
  std::shared_ptr<Sphere> object_;
};

bool operator==(Intersection const& lhs, Intersection const& rhs);

/**
 * @brief Creates a vector of intersections from the given arguments.
 * 
 * @param arg The arguments to be converted to intersections.
 * @return std::vector<Intersection> The vector of intersections.
 */
template<std::same_as<Intersection> ... Arg> 
auto intersections(const Arg&... arg){
  return std::vector<Intersection>{arg...};
}

/**
 * @brief A function that returns the intersection with the least distance.
 * 
 * This is used to find the first object that is intersection in order to display it first.
 * 
 * @param intersections a utility::Vector containing information about all the intersected objects
 * @return const Intersection* a pointer to the struct containing info the first object that the ray intersected with
 */
std::optional<Intersection> hit(const std::vector<Intersection>& intersections);

} // namespace geometry
} // namespace raytracer

#endif // INTERSECTIONS_H