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
  World() {}

  /**
   * Calculates the intersections between a ray and a world.
   *
   * @param world The scene::World object representing the world.
   * @param ray The utility::Ray object representing the ray.
   * @return A std::vector<Intersection> containing the intersections between the ray and the objects in the world.
   */
  std::vector<geometry::Intersection> intersect(const utility::Ray& ray) const;
  
  /**
   * Calculates the color of a shaded hit based on the given computation.
   *
   * @param computation The computation containing information about the hit.
   * @return The color of the shaded hit.
   */
  utility::Color shadeHit(const geometry::Computations& computation) const;

  utility::Color colorAt(const utility::Ray& ray) const;

  std::vector<PointLight> lights_;
  std::vector<std::shared_ptr<geometry::Sphere>> objects_;
};

World defaultWorld();

} // namespace scene
} // namespace raytracer

#endif // WORLD_HPP