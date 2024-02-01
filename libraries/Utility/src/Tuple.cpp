#include <cmath>
#include <ostream>

#include "Tuple.hpp"
#include "FloatAlmostEquals.hpp"

namespace raytracer {
namespace utility { 

bool Tuple::isVector() const noexcept {
  return floatNearlyEqual(this->w(), 0.0);
}
bool Tuple::isPoint() const noexcept {
  return floatNearlyEqual(this->w(), 1.0);
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

const double& Tuple::x() const noexcept{
  return data[0];
}
double& Tuple::x() noexcept{
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[0]);
}

const double& Tuple::y() const noexcept{
  return data[1];
}
double& Tuple::y() noexcept{
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[1]);
}

const double& Tuple::z() const noexcept{
  return data[2];
}
double& Tuple::z() noexcept{
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[2]);
}

const double& Tuple::w() const noexcept{
  return data[3];
}
double& Tuple::w() noexcept{
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[3]);
}

double Tuple::magnitude() const noexcept{
  auto sumSquared = this->x()*this->x() + this->y()*this->y() +
                    this->z()*this->z() + this->w()*this->w();
  return std::sqrt(sumSquared);
}

Tuple Tuple::normalize() const noexcept{
  return *this/this->magnitude();
}

double Tuple::dot(const Tuple& rhs) const noexcept{
  auto multAndACum = this->x()*rhs.x() + this->y()*rhs.y() +
                     this->z()*rhs.z() + this->w()*rhs.w();
  return multAndACum;
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

} // namespace utility
} // namespace raytracer