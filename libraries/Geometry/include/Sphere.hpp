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
Sphere() noexcept: id_{ID++}, transformation_{utility::Matrix<4,4>::identity()},
                   inverseTransformation_{utility::Matrix<4,4>::identity()}{}
bool operator==(Sphere const& other) const noexcept;

/**
 * Calculates the normal vector at a given point on the sphere.
 *
 * @param point The point on the sphere.
 * @return The normal vector at the given point.
 */
utility::Tuple normalAt(const utility::Tuple &point) const noexcept;

/**
 * @brief Function to calculate the intersections between a ray and a sphere.
 *
 * @param ray The ray to intersect with the sphere.
 * @return A vector of Intersection objects representing the intersections between the ray and the sphere.
 */
std::vector<Intersection> intersect(const utility::Ray& ray) const noexcept;

/**
 * @brief Sets the transformation matrix for the sphere.
 * 
 * @param transformation The transformation matrix to set.
 */
void setTransform(const utility::Matrix<4,4> &transformation) noexcept;

/**
 * @brief Return the transformation matrix for the sphere.
 * 
 * @return The transformation matrix.
 */
const utility::Matrix<4,4>& transformation() const noexcept;

/**
 * @brief Return the inverse transformation matrix for the sphere.
 * 
 * @return The inverse transformation matrix.
 */
const utility::Matrix<4,4>& inverseTransform() const noexcept;

std::size_t id_;
double radius_;
utility::Tuple origin_;
material::Material material_;

private: 
static std::atomic<size_t> ID;
utility::Matrix<4,4> transformation_;
utility::Matrix<4,4> inverseTransformation_;
};

inline void Sphere::setTransform(const utility::Matrix<4,4> &transformation) noexcept{
  transformation_ = std::move(transformation);
  inverseTransformation_ = inverse(transformation_);
}

inline const utility::Matrix<4,4>& Sphere::transformation() const noexcept{
  return transformation_;
}

inline const utility::Matrix<4,4>& Sphere::inverseTransform() const noexcept{
  return inverseTransformation_;
}

} // namespace geometry
} // namespace raytracer

#endif // SPHERE_H