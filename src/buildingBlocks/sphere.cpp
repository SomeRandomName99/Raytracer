#include "include/sphere.h"
#include "include/maths.h"
#include "include/matrix.h"

std::atomic<size_t> Sphere::ID = 0;

bool Sphere::operator==(const Sphere& other) const{
  return this->id == other.id;
}

void Sphere::setTransform(const Matrix<4,4> &transformation){
  this->transformation = transformation;
}

Tuple Sphere::normalAt(const Tuple &worldPoint) const{
  Tuple objectPoint  = inverse(this->transformation) * worldPoint;
  Tuple objectNormal = objectPoint - Point(0, 0, 0);
  // This is needed because the normal vector is a linear form, see below:
  // https://computergraphics.stackexchange.com/questions/1502/why-is-the-transposed-inverse-of-the-model-view-matrix-used-to-transform-the-nor
  Tuple worldNormal  = inverse(this->transformation).transpose() * objectNormal;
  worldNormal.w() = 0;

  return worldNormal.normalize();
}