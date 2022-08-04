#include <cmath>

#include "include/tuple.h"
#include "include/maths.h"


Tuple::Tuple(float x_, float y_, float z_, float w_): x{x_}, 
                                                      y{y_}, 
                                                      z{z_}, 
                                                      w{w_} 
                                                      {}

bool Tuple::isVector() const {
  return floatEqual(this->w, 0.0f);
}
bool Tuple::isPoint() const {
  return floatEqual(this->w, 1.0f);
}

bool Tuple::operator==(const Tuple& rhs) const {
  bool xEqual = floatEqual(this->x, rhs.x);
  bool yEqual = floatEqual(this->y, rhs.y);
  bool zEqual = floatEqual(this->z, rhs.z);
  bool wEqual = floatEqual(this->w, rhs.w);

  bool allEqual = xEqual&&yEqual&&zEqual&&wEqual;
  return allEqual;
}

Tuple Tuple::operator+(const Tuple& rhs) const {
  return Tuple(this->x+rhs.x, this->y+rhs.y, 
               this->z+rhs.z, this->w+rhs.w);
}

Tuple Tuple::operator-(const Tuple& rhs)  const{
  return Tuple(this->x-rhs.x, this->y-rhs.y, 
               this->z-rhs.z, this->w-rhs.w);
}
Tuple Tuple::operator*(const double& rhs) const{
  return Tuple(this->x*rhs, this->y*rhs, 
               this->z*rhs, this->w*rhs);
}
Tuple Tuple::operator/(const double& rhs) const{
  return Tuple(this->x/rhs, this->y/rhs, 
               this->z/rhs, this->w/rhs);
}

double Tuple::magnitude() const{
  auto sumSquared = this->x*this->x + this->y*this->y +
                    this->z*this->z + this->w*this->w;
  return std::sqrt(sumSquared);
}
Tuple Tuple::nomrmalize() const{
  return *this/this->magnitude();
}

double Tuple::dot(const Tuple& rhs) const{
  auto multAndACum = this->x*rhs.x + this->y*rhs.y +
                     this->z*rhs.z + this->w*rhs.w;
  return multAndACum;
}
Tuple Tuple::cross(const Tuple& rhs) const{
  return Vector(this->y*rhs.z - this->z*rhs.y,
                this->z*rhs.x - this->x*rhs.z,
                this->x*rhs.y - this->y*rhs.x);
}

Tuple Point(float x,float y, float z){
  return Tuple(x,y,z,1);
}
Tuple Vector(float x,float y, float z){
  return Tuple(x,y,z,0);
}