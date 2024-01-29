#include <cmath>
#include <ostream>

#include "Tuple.hpp"
#include "FloatAlmostEquals.hpp"

namespace raytracer {
namespace utility { 

bool Tuple::isVector() const {
  return floatNearlyEqual(this->w(), 0.0f);
}
bool Tuple::isPoint() const {
  return floatNearlyEqual(this->w(), 1.0f);
}

bool Tuple::operator==(const Tuple& rhs) const {
  bool xEqual = floatNearlyEqual(this->x(), rhs.x());
  bool yEqual = floatNearlyEqual(this->y(), rhs.y());
  bool zEqual = floatNearlyEqual(this->z(), rhs.z());
  bool wEqual = floatNearlyEqual(this->w(), rhs.w());

  bool allEqual = xEqual&&yEqual&&zEqual&&wEqual;
  return allEqual;
}

Tuple Tuple::operator+(const Tuple& rhs) const {
  return Tuple(this->x()+rhs.x(), this->y()+rhs.y(), 
               this->z()+rhs.z(), this->w()+rhs.w());
}

Tuple Tuple::operator-(const Tuple& rhs)  const{
  return Tuple(this->x()-rhs.x(), this->y()-rhs.y(), 
               this->z()-rhs.z(), this->w()-rhs.w());
}
Tuple Tuple::operator*(const float& rhs) const{
  return Tuple(this->x()*rhs, this->y()*rhs, 
               this->z()*rhs, this->w()*rhs);
}
Tuple Tuple::operator/(const float& rhs) const{
  return Tuple(this->x()/rhs, this->y()/rhs, 
               this->z()/rhs, this->w()/rhs);
}
Tuple Tuple::operator-() const{
  return *this*-1;
}

const float& Tuple::x() const{
  return data[0];
}
float& Tuple::x(){
  return const_cast<float&>(const_cast<const Tuple*>(this)->data[0]);
}

const float& Tuple::y() const{
  return data[1];
}
float& Tuple::y(){
  return const_cast<float&>(const_cast<const Tuple*>(this)->data[1]);
}

const float& Tuple::z() const{
  return data[2];
}
float& Tuple::z(){
  return const_cast<float&>(const_cast<const Tuple*>(this)->data[2]);
}

const float& Tuple::w() const{
  return data[3];
}
float& Tuple::w(){
  return const_cast<float&>(const_cast<const Tuple*>(this)->data[3]);
}

float Tuple::magnitude() const{
  auto sumSquared = this->x()*this->x() + this->y()*this->y() +
                    this->z()*this->z() + this->w()*this->w();
  return std::sqrt(sumSquared);
}

Tuple Tuple::normalize() const{
  return *this/this->magnitude();
}

float Tuple::dot(const Tuple& rhs) const{
  auto multAndACum = this->x()*rhs.x() + this->y()*rhs.y() +
                     this->z()*rhs.z() + this->w()*rhs.w();
  return multAndACum;
}

Tuple Tuple::cross(const Tuple& rhs) const{
  return Vector(this->y()*rhs.z() - this->z()*rhs.y(),
                this->z()*rhs.x() - this->x()*rhs.z(),
                this->x()*rhs.y() - this->y()*rhs.x());
}

Tuple Tuple::reflect(const Tuple& normal) const{
  return *this - (normal * 2 * this->dot(normal));
}

Tuple Point(float x, float y, float z){
  return Tuple(x,y,z,1);
}
Tuple Vector(float x, float y, float z){
  return Tuple(x,y,z,0);
}

std::ostream& operator<<(std::ostream& os, const Tuple& rhs) {
  os << rhs.x() << ' ' << rhs.y() << ' ' << rhs.z() << ' ' << rhs.w() << '\n';

  return os;
}

} // namespace utility
} // namespace raytracer