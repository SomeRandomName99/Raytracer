#include "include/color.h"
#include "include/tuple.h"

Color::Color(float red_, float green_, float blue_) : _color{Tuple(red_, green_, blue_, 0.0f)},
                                                      red{_color.x}, green{_color.y}, blue{_color.z}
                                                      {}

bool Color::operator==(const Color& rhs) const{
  return this->_color==rhs._color;
}

Color Color::operator+(const Color& rhs) const{
  const auto newTuple = this->_color + rhs._color;
  return Color(newTuple.x, newTuple.y, newTuple.z); 
}
Color Color::operator-(const Color& rhs) const{
  const auto newTuple = this->_color - rhs._color;
  return Color(newTuple.x, newTuple.y, newTuple.z); 
}
Color Color::operator*(const float& rhs) const{
  const auto newTuple = this->_color * rhs;
  return Color(newTuple.x, newTuple.y, newTuple.z);
}
Color Color::operator*(const Color& rhs) const{ // Hadamarad Product
  return Color(this->red*rhs.red, this->green*rhs.green, this->blue*rhs.blue);
}