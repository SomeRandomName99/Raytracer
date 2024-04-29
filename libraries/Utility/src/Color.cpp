#include <cstdint>

#include "Color.hpp"


namespace raytracer {
namespace utility {

Color::Color() noexcept : _color{Tuple(0.0, 0.0, 0.0, 0.0)}
                 {}
Color::Color(float red_, float green_, float blue_) noexcept : _color{Tuple(red_, green_, blue_, 0.0)}
                                                      {}

bool Color::operator==(const Color& rhs) const noexcept{
  return this->_color==rhs._color;
}

Color Color::operator+(const Color& rhs) const noexcept{
  const auto newTuple = this->_color + rhs._color;
  return Color(newTuple.x(), newTuple.y(), newTuple.z()); 
}

Color Color::operator+=(const Color& rhs) noexcept {
  this->_color = this->_color + rhs._color;
  return *this; 
}

Color Color::operator-(const Color& rhs) const noexcept{
  const auto newTuple = this->_color - rhs._color;
  return Color(newTuple.x(), newTuple.y(), newTuple.z()); 
}

Color Color::operator*(const float& rhs) const noexcept{
  const auto newTuple = this->_color * rhs;
  return Color(newTuple.x(), newTuple.y(), newTuple.z());
}

Color Color::operator*(const Color& rhs) const noexcept{ // Hadamarad Product
  return Color(this->red()*rhs.red(), this->green()*rhs.green(), this->blue()*rhs.blue());
}

// Color setters and getters
const float& Color::red() const noexcept{
  return _color.x();
}
void Color::red(const float& val) noexcept {
  _color.x() = val;
}

const float& Color::green() const noexcept{
  return _color.y();
}
void Color::green(const float& val) noexcept {
  _color.y() = val;
}

const float& Color::blue() const noexcept{
  return _color.z();
}
void Color::blue(const float& val) noexcept {
  _color.z() = val;
}

Color hexColor(unsigned int hex) noexcept{
 const auto red = (hex >> 16) & 0xFF;
  const auto green = (hex >> 8) & 0xFF;
  const auto blue = hex & 0xFF;
  return Color(red/255.0, green/255.0, blue/255.0);
} 

} // namespace utility
} // namespace raytracer