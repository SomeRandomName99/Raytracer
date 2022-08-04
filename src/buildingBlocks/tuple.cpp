#include "include/tuple.h"
#include "include/maths.h"


Tuple::Tuple(float x_, float y_, float z_, float w_): x{x_}, 
                                                      y{y_}, 
                                                      z{z_}, 
                                                      w{w_} 
                                                      {}

bool Tuple::isVector() const {
  return floatEqual(this->w, 1.0f);
}
bool Tuple::isPoint() const {
  return floatEqual(this->w, 0.0f);
}

bool Tuple::operator==(const Tuple& rhs) const {
  bool xEqual = floatEqual(this->x, rhs.x);
  bool yEqual = floatEqual(this->y, rhs.y);
  bool zEqual = floatEqual(this->z, rhs.z);
  bool wEqual = floatEqual(this->w, rhs.w);

  bool allEqual = xEqual&&yEqual&&zEqual&&wEqual;
  return allEqual;
}

Tuple Point(float x,float y, float z){
  return Tuple(x,y,z,0);
}
Tuple Vector(float x,float y, float z){
  return Tuple(x,y,z,1);
}