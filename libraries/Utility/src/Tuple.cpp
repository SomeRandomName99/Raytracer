#include <cmath>
#include <ostream>
#include <ranges>
#include <numeric>
#include <execution>

#include "Tuple.hpp"
#include "FloatUtils.hpp"

namespace raytracer {
namespace utility { 

bool Tuple::isVector() const noexcept {
  return floatNearlyEqual(this->w(), 0.0f);
}
bool Tuple::isPoint() const noexcept {
  return floatNearlyEqual(this->w(), 1.0f);
}

bool Tuple::operator==(const Tuple& rhs) const noexcept {
  bool xEqual = floatNearlyEqual(this->x(), rhs.x());
  bool yEqual = floatNearlyEqual(this->y(), rhs.y());
  bool zEqual = floatNearlyEqual(this->z(), rhs.z());
  bool wEqual = floatNearlyEqual(this->w(), rhs.w());

  bool allEqual = xEqual&&yEqual&&zEqual&&wEqual;
  return allEqual;
}

Tuple Tuple::operator+(const Tuple& rhs) const noexcept {
  return Tuple(this->x()+rhs.x(), this->y()+rhs.y(), 
               this->z()+rhs.z(), this->w()+rhs.w());
}
Tuple Tuple::operator+(const double& rhs) const noexcept {
  return Tuple(this->x()+rhs, this->y()+rhs, 
               this->z()+rhs, this->w()+rhs);
}

Tuple Tuple::operator-(const Tuple& rhs) const noexcept{
  return Tuple(this->x()-rhs.x(), this->y()-rhs.y(), 
               this->z()-rhs.z(), this->w()-rhs.w());
}
Tuple Tuple::operator*(const double& rhs) const noexcept{
  return Tuple(this->x()*rhs, this->y()*rhs, 
               this->z()*rhs, this->w()*rhs);
}
Tuple Tuple::operator/(const double& rhs) const noexcept{
  return Tuple(this->x()/rhs, this->y()/rhs, 
               this->z()/rhs, this->w()/rhs);
}
Tuple Tuple::operator-() const noexcept{
  return *this*-1;
}

double Tuple::magnitude() const noexcept{
  auto sumSquared = std::transform_reduce(std::execution::unseq, this->data.cbegin(), this->data.cend(), 
                                          0.0f, std::plus<>(), [](const auto& val){return val*val;});
  return std::sqrt(sumSquared);
}

Tuple Tuple::normalize() const noexcept{
  return *this/this->magnitude();
}

double Tuple::dot(const Tuple& rhs) const noexcept{
  return std::transform_reduce(std::execution::unseq, this->data.cbegin(), this->data.cend(), 
                               rhs.data.cbegin(), 0.0f, std::plus<>(), std::multiplies<>());
}

Tuple Tuple::cross(const Tuple& rhs) const noexcept{
  return Vector(this->y()*rhs.z() - this->z()*rhs.y(),
                this->z()*rhs.x() - this->x()*rhs.z(),
                this->x()*rhs.y() - this->y()*rhs.x());
}

Tuple Tuple::reflect(const Tuple& normal) const noexcept{
  return *this - (normal * 2 * this->dot(normal));
}

Tuple Point(const double x, const double y, const double z) noexcept{
  return Tuple(x,y,z,1);
}
Tuple Vector(const double x, const double y, const double z) noexcept{
  return Tuple(x,y,z,0);
}

std::ostream& operator<<(std::ostream& os, const Tuple& rhs) noexcept {
  os << rhs.x() << ' ' << rhs.y() << ' ' << rhs.z() << ' ' << rhs.w() << '\n';

  return os;
}

Tuple operator*(const Tuple& lhs, const Tuple& rhs) noexcept{
  return Tuple{lhs.x()*rhs.x(), lhs.y()*rhs.y(), 
               lhs.z()*rhs.z(), lhs.w()*rhs.w()};
}

Tuple componentWiseMin(const Tuple& lhs, const Tuple& rhs) noexcept{
  return Tuple{std::min(lhs.x(), rhs.x()), std::min(lhs.y(), rhs.y()), 
               std::min(lhs.z(), rhs.z()), lhs.w()};
}

Tuple componentWiseMax(const Tuple& lhs, const Tuple& rhs) noexcept{
  return Tuple{std::max(lhs.x(), rhs.x()), std::max(lhs.y(), rhs.y()), 
               std::max(lhs.z(), rhs.z()), lhs.w()};
}

} // namespace utility
} // namespace raytracer