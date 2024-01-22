#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>
#include <optional>
#include <memory>

#include "Tuple.hpp"
#include "Sphere.hpp"
#include "World.hpp"
#include "Ray.hpp"

namespace raytracer {
namespace geometry {

/**
 * @brief A class that stores relevant info regarding an intersecion
 * 
 * The class include store the t value which is used in calculating the distance from the ray's origin, as well as
 * the object that was intersected with. For now it is a hardcoded sphere but will be generalized later. 
 */
struct Intersection
{
  Intersection(double t, std::shared_ptr<Sphere> object) : t_{t}, object_{object} {}

  double t_;
  std::shared_ptr<Sphere> object_;
};

bool operator==(Intersection const& lhs, Intersection const& rhs);

/**
 * @brief Calculate and return \sa{Intersection} between ray and sphere 
 * 
 * @param \sa{Sphere} 
 * @param \sa{Ray} 
 * @return std::vector< \sa{Intersection} > utility::Vector containing all recorded intersections
 */
std::vector<Intersection> intersect(const Sphere& sphere, const utility::Ray& ray);

/**
 * Calculates the intersections between a ray and a world.
 *
 * @param world The scene::World object representing the world.
 * @param ray The utility::Ray object representing the ray.
 * @return A std::vector<Intersection> containing the intersections between the ray and the objects in the world.
 */
std::vector<Intersection> intersect_world(const scene::World& world, const utility::Ray& ray);

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