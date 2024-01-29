#include "Color.hpp"

namespace raytracer {
namespace utility {

Color::Color() : _color{Tuple(0.0f, 0.0f, 0.0f, 0.0f)}
                 {}
Color::Color(float red_, float green_, float blue_) : _color{Tuple(red_, green_, blue_, 0.0f)}
                                                      {}

bool Color::operator==(const Color& rhs) const{
  return this->_color==rhs._color;
}

Color Color::operator+(const Color& rhs) const{
  const auto newTuple = this->_color + rhs._color;
  return Color(newTuple.x(), newTuple.y(), newTuple.z()); 
}

Color Color::operator+=(const Color& rhs) {
  this->_color = this->_color + rhs._color;
  return *this; 
}

Color Color::operator-(const Color& rhs) const{
  const auto newTuple = this->_color - rhs._color;
  return Color(newTuple.x(), newTuple.y(), newTuple.z()); 
}

Color Color::operator*(const float& rhs) const{
  const auto newTuple = this->_color * rhs;
  return Color(newTuple.x(), newTuple.y(), newTuple.z());
}

Color Color::operator*(const Color& rhs) const{ // Hadamarad Product
  return Color(this->red()*rhs.red(), this->green()*rhs.green(), this->blue()*rhs.blue());
}

// Color setters and getters
const float& Color::red() const{
  return _color.x();
}
void Color::red(const float& val) {
  _color.x() = val;
}

const float& Color::green() const{
  return _color.y();
}
void Color::green(const float& val) {
  _color.y() = val;
}

const float& Color::blue() const{
  return _color.z();
}
void Color::blue(const float& val) {
  _color.z() = val;
}

} // namespace utility
} // namespace raytracer