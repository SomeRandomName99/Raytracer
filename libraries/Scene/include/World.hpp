#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <memory>

#include "Sphere.hpp"
#include "Intersections.hpp"
#include "Light.hpp"

namespace raytracer {
namespace scene {

class World {
public:
  World() noexcept {}

  /**
   * Calculates the intersections between a ray and a world.
   *
   * @param ray The utility::Ray object representing the ray.
   * @return A std::vector<Intersection> containing the intersections between the ray and the objects in the world.
   */
  std::vector<geometry::Intersection> intersect(const utility::Ray& ray) const noexcept;

  /**
   * Barebones implementation of a shadow intersection check.
   * 
   * @details Because this function is only used for calculating shadows, it will instantly returns if an intersection is found.
   * As there is no need to find all of the intersections between the ray and the world, save them, and then sort them.
   *
   * @param ray The utility::Ray object representing the ray.
   * @param distanceToLight The distance between the ray origin and the light source.
   * @return True: if an intersection was found, False: otherwise.
   */
  bool intersectShadow(const utility::Ray& ray, double distanceToLight) const noexcept;

  
  /**
   * Calculates the color of a shaded hit based on the given computation.
   *
   * @param computation The computation containing information about the hit.
   * @return The color of the shaded hit.
   */
  utility::Color shadeHit(const geometry::Computations& computation) const noexcept;

  /**
   * Calculates the color at a given ray intersection point in the world.
   *
   * @param ray The ray to calculate the color for.
   * @return The color at the ray intersection point.
   */
  utility::Color colorAt(const utility::Ray& ray) const noexcept;

  /**
   * Checks if a given point in the world is shadowed by a light source.
   *
   * @param light The light source to check against.
   * @param point The point to check for shadows.
   * @return True if the point is shadowed, false otherwise.
   */
  bool isShadowed(const PointLight& light, const utility::Tuple& point) const noexcept;

  std::vector<PointLight> lights_;
  std::vector<std::shared_ptr<geometry::Sphere>> objects_;
};

World defaultWorld() noexcept;

} // namespace scene
} // namespace raytracer

#endif // WORLD_HPP