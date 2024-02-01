#include "Color.hpp"

namespace raytracer {
namespace utility {

Color::Color() noexcept : _color{Tuple(0.0, 0.0, 0.0, 0.0)}
                 {}
Color::Color(double red_, double green_, double blue_) noexcept : _color{Tuple(red_, green_, blue_, 0.0)}
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

Color Color::operator*(const double& rhs) const noexcept{
  const auto newTuple = this->_color * rhs;
  return Color(newTuple.x(), newTuple.y(), newTuple.z());
}

Color Color::operator*(const Color& rhs) const noexcept{ // Hadamarad Product
  return Color(this->red()*rhs.red(), this->green()*rhs.green(), this->blue()*rhs.blue());
}

// Color setters and getters
const double& Color::red() const noexcept{
  return _color.x();
}
void Color::red(const double& val) noexcept {
  _color.x() = val;
}

const double& Color::green() const noexcept{
  return _color.y();
}
void Color::green(const double& val) noexcept {
  _color.y() = val;
}

const double& Color::blue() const noexcept{
  return _color.z();
}
void Color::blue(const double& val) noexcept {
  _color.z() = val;
}

} // namespace utility
} // namespace raytracer