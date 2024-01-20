#include <cmath>
#include <ostream>

#include "Tuple.hpp"
#include "FloatAlmostEquals.hpp"


bool Tuple::isVector() const {
  return floatEqual(this->w(), 0.0);
}
bool Tuple::isPoint() const {
  return floatEqual(this->w(), 1.0);
}

bool Tuple::operator==(const Tuple& rhs) const {
  bool xEqual = floatEqual(this->x(), rhs.x());
  bool yEqual = floatEqual(this->y(), rhs.y());
  bool zEqual = floatEqual(this->z(), rhs.z());
  bool wEqual = floatEqual(this->w(), rhs.w());

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
Tuple Tuple::operator*(const double& rhs) const{
  return Tuple(this->x()*rhs, this->y()*rhs, 
               this->z()*rhs, this->w()*rhs);
}
Tuple Tuple::operator/(const double& rhs) const{
  return Tuple(this->x()/rhs, this->y()/rhs, 
               this->z()/rhs, this->w()/rhs);
}
Tuple Tuple::operator-() const{
  return *this*-1;
}

const double& Tuple::x() const{
  return data[0];
}
double& Tuple::x(){
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[0]);
}

const double& Tuple::y() const{
  return data[1];
}
double& Tuple::y(){
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[1]);
}

const double& Tuple::z() const{
  return data[2];
}
double& Tuple::z(){
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[2]);
}

const double& Tuple::w() const{
  return data[3];
}
double& Tuple::w(){
  return const_cast<double&>(const_cast<const Tuple*>(this)->data[3]);
}

double Tuple::magnitude() const{
  auto sumSquared = this->x()*this->x() + this->y()*this->y() +
                    this->z()*this->z() + this->w()*this->w();
  return std::sqrt(sumSquared);
}

Tuple Tuple::normalize() const{
  return *this/this->magnitude();
}

double Tuple::dot(const Tuple& rhs) const{
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

Tuple Point(double x, double y, double z){
  return Tuple(x,y,z,1);
}
Tuple Vector(double x, double y, double z){
  return Tuple(x,y,z,0);
}

std::ostream& operator<<(std::ostream& os, const Tuple& rhs) {
  os << rhs.x() << ' ' << rhs.y() << ' ' << rhs.z() << ' ' << rhs.w() << '\n';

  return os;
}