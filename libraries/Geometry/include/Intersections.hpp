#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>
#include <optional>
#include <memory>
#include <span>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "Arena.hpp"

constexpr double SHADOW_OFFSET = 1e-4;

namespace raytracer {
namespace geometry {
class ShapeBase;

/**
 * \class Intersection
 * \brief Save information about an intersection between a ray and a geometry object.
 */
struct Intersection
{
  const ShapeBase* object;
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
                                 const utility::Arena<Intersection>& intersections) noexcept;

/**
 * @brief A function that returns the intersection with the least distance.
 * 
 * This is used to find the first object that was hit in order to display it first.
 */
std::optional<Intersection> hit(const utility::Arena<Intersection>& intersections) noexcept;

/**
 * @brief Computes the Schlick approximation for the Fresnel effect.
 *
 * The Schlick approximation is used to model the amount of reflection and refraction 
 * at the surface of a transparent material based on the angle of incidence. It is 
 * a parameter in the calculation of the Fresnel effect, which describes how the 
 * amount of reflection and refraction varies depending on the angle at which light 
 * hits the surface.
 *
 * @param comps The pre-computed state of a ray-object intersection.
 * @return The reflectance factor, a value between 0 and 1.
 */
double schlick(const Computations& comps) noexcept;

} // namespace geometry
} // namespace raytracer

#endif // INTERSECTIONS_H