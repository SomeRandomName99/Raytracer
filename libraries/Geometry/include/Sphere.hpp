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
Sphere(): id{ID++}, transformation{utility::Matrix<4,4>::identity()}{}
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

std::size_t id;
float radius;
utility::Tuple origin;
utility::Matrix<4,4> transformation;
material::Material material;

private: 
static std::atomic<size_t> ID;
};

} // namespace geometry
} // namespace raytracer

#endif // SPHERE_H