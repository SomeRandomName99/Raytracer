#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <atomic>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "ShapeT.hpp"

namespace raytracer {
namespace geometry {

struct Intersection;

class Sphere : public ShapeT<Sphere>
{
public:
Sphere() noexcept {}
bool operator==(Sphere const& other) const noexcept;

/**
 * Calculates the normal vector at a given point on the sphere.
 */
utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept;

/**
 * Function to calculate the intersections between a ray and a sphere.
 */
std::vector<Intersection> localIntersect(const utility::Ray &transformedRay) const noexcept;

double radius_;
utility::Tuple origin_;
};

} // namespace geometry
} // namespace raytracer

#endif // SPHERE_H