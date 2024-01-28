#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <atomic>
#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Transformations.hpp"
#include "Matrix.hpp"
#include "Material.hpp"

namespace raytracer {
namespace geometry {

class Intersection;

class Sphere
{
public:
Sphere(): id_{ID++}, transformation_{utility::Matrix<4,4>::identity()}{}
bool operator==(Sphere const& other) const;
void setTransform(const utility::Matrix<4,4> &transformation);

/**
 * Calculates the normal vector at a given point on the sphere.
 *
 * @param point The point on the sphere.
 * @return The normal vector at the given point.
 */
utility::Tuple normalAt(const utility::Tuple &point) const;

/**
 * @brief Function to calculate the intersections between a ray and a sphere.
 *
 * @param ray The ray to intersect with the sphere.
 * @return A vector of Intersection objects representing the intersections between the ray and the sphere.
 */
std::vector<Intersection> intersect(const utility::Ray& ray) const;

std::size_t id_;
float radius_;
utility::Tuple origin_;
utility::Matrix<4,4> transformation_;
material::Material material_;

private: 
static std::atomic<size_t> ID;
};

} // namespace geometry
} // namespace raytracer

#endif // SPHERE_H