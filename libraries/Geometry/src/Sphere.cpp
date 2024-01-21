#include "Sphere.hpp"
#include "FloatAlmostEquals.hpp"

namespace raytracer {
namespace geometry {

std::atomic<size_t> Sphere::ID = 0;

bool Sphere::operator==(const Sphere& other) const{
  return this->id == other.id;
}

void Sphere::setTransform(const utility::Matrix<4,4> &transformation){
  this->transformation = transformation;
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