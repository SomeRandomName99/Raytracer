#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <memory>

#include "Sphere.hpp"
#include "Plane.hpp"
#include "Intersections.hpp"
#include "Shape.hpp"
#include "Light.hpp"
#include "Arena.hpp"

namespace raytracer {

namespace scene {

constexpr size_t MAX_INTERSECTIONS = 5;
class World {
public:
  World() noexcept {}

  /**
   * Calculates the intersections between a ray and a world.
   *
   * @param ray The utility::Ray object representing the ray.
   * @param intersections The vector to store the intersections in.
   */
  void intersect(const utility::Ray& ray, utility::Arena<geometry::Intersection>& intersections) const noexcept;

  /**
   * @brief Checks if a ray intersects with any objects in the scene between a point and a light source.
   *
   * This function is typically used to determine if a point is in shadow or not. If the ray does intersect with an object,
   * that means the point is in shadow. If the ray does not intersect with any objects, that means the point is lit by the light source.
   */
  bool intersectShadow(const utility::Ray& ray, double distanceToLight, utility::Arena<geometry::Intersection>& intersections) const noexcept;

  /**
   * Calculates the color of a shaded hit based on the given computation.
   */
  utility::Color shadeHit(const geometry::Computations& computation, utility::Arena<geometry::Intersection>& intersections, size_t recursionLimit = MAX_INTERSECTIONS) const noexcept;

  utility::Color reflectedColor(const geometry::Computations& computation, utility::Arena<geometry::Intersection>& intersections, size_t recursionLimit = MAX_INTERSECTIONS) const noexcept;
  utility::Color refractedColor(const geometry::Computations& computation, utility::Arena<geometry::Intersection>& intersections, size_t recursionLimit = MAX_INTERSECTIONS) const noexcept;

  /**
   * Calculates the color at a given ray intersection point in the world.
   *
   * @param ray The ray to calculate the color for.
   * @return The color at the ray intersection point.
   */
  utility::Color colorAt(const utility::Ray& ray, utility::Arena<geometry::Intersection>& intersections, size_t recursionLimit = 5) const noexcept;

  /**
   * Checks if a given point in the world is shadowed by a light source.
   *
   * @param light The light source to check against.
   * @param point The point to check for shadows.
   * @return True if the point is shadowed, false otherwise.
   */
  bool isShadowed(const PointLight& light, const utility::Tuple& point, utility::Arena<geometry::Intersection>& intersections) const noexcept;

  std::vector<PointLight> lights_;
  std::vector<std::shared_ptr<geometry::ShapeBase>> objects_;
};

World defaultWorld() noexcept;

} // namespace scene
} // namespace raytracer

#endif // WORLD_HPP