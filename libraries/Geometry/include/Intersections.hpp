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

struct Computations{
  float dist;
  std::shared_ptr<Sphere> object;
  utility::Tuple point;
  utility::Tuple eyeVector;
  utility::Tuple normalVector;
  bool inside;
};

/**
 * \class Intersection
 * \brief Represents an intersection between a ray and a geometry object.
 *
 * The Intersection class stores information about the intersection point, distance from the ray origin,
 * and the intersected geometry object. It also provides a method to prepare the necessary computations
 * for the intersection, such as the intersection point, normal vector, and material properties.
 */
class Intersection
{
public:
  Intersection(double dist, std::shared_ptr<Sphere> object) : dist_{dist}, object_{object} {}

  /**
   * \brief Prepares the computations for a given ray intersection.
   *
   * This function takes a ray as input and prepares the necessary computations for the intersection
   * between the ray and the geometry. It returns an object of type Computations that contains
   * relevant information such as the intersection point, normal vector, and material properties.
   *
   * \param ray The ray to compute the intersection with.
   * \return An object of type Computations containing the computed intersection information.
   */
  Computations prepareComputations(const utility::Ray& ray) const;

  double dist_; ///< Distance from the ray origin to the intersection point.
  std::shared_ptr<Sphere> object_; ///< The intersected geometry object.
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