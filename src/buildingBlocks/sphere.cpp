#include "include/sphere.h"
#include "include/maths.h"

std::atomic<size_t> Sphere::ID = 0;

bool Sphere::operator==(const Sphere& other) const{
  return this->id == other.id;
}