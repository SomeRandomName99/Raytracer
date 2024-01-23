#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

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
  std::vector<geometry::Intersection> intersect(const utility::Ray& ray);

  std::vector<PointLight> lights_;
  std::vector<geometry::Sphere> objects_;
};

} // namespace scene
} // namespace raytracer

#endif // WORLD_HPP