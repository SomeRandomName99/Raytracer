#include "Sphere.hpp"
#include "FloatAlmostEquals.hpp"
#include "Intersections.hpp"

namespace raytracer {
namespace geometry {

std::atomic<size_t> Sphere::ID = 0;

bool Sphere::operator==(const Sphere& other) const{
  return this->id == other.id;
}

void Sphere::setTransform(const utility::Matrix<4,4> &transformation){
  this->transformation = transformation;
}

std::vector<Intersection> Sphere::intersect(const utility::Ray& ray) const {
  // For now we assume that the sphere is always at the origin
  const auto transformedRay    = utility::transform(ray, inverse(this->transformation));
  const auto SphereCenterToRay = transformedRay.origin_ - utility::Point(0, 0, 0);

  const auto a = transformedRay.direction_.dot(transformedRay.direction_);
  const auto b = 2 * transformedRay.direction_.dot(SphereCenterToRay);
  const auto c = SphereCenterToRay.dot(SphereCenterToRay) - 1; // where 1 is the radius of the sphere

  const auto discriminant = b*b - 4*a*c;

  if (discriminant < 0.)
    return std::vector<Intersection>{};

  const auto t1 = (-b - std::sqrt(discriminant)) / (2*a);
  const auto t2 = (-b + std::sqrt(discriminant)) / (2*a);
  return std::vector<Intersection>{Intersection{t1, std::make_shared<Sphere>(*this)}, 
                                   Intersection{t2, std::make_shared<Sphere>(*this)}};
}

utility::Tuple Sphere::normalAt(const utility::Tuple &worldPoint) const{
  utility::Tuple objectPoint  = inverse(this->transformation) * worldPoint;
  utility::Tuple objectNormal = objectPoint - utility::Point(0, 0, 0);
  // This is needed because the normal utility::Vector is a linear form, see below:
  // https://computergraphics.stackexchange.com/questions/1502/why-is-the-transposed-inverse-of-the-model-view-matrix-used-to-transform-the-nor
  utility::Tuple worldNormal  = inverse(this->transformation).transpose() * objectNormal;
  worldNormal.w() = 0;

  return worldNormal.normalize();
}

} // namespace geometry
} // namespace raytracer