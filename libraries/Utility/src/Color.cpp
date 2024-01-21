#include "Color.hpp"

namespace raytracer {
namespace utility {

Color::Color() : _color{Tuple(0.0f, 0.0f, 0.0f, 0.0f)}
                 {}
Color::Color(double red_, double green_, double blue_) : _color{Tuple(red_, green_, blue_, 0.0f)}
                                                      {}

bool Color::operator==(const Color& rhs) const{
  return this->_color==rhs._color;
}

Color Color::operator+(const Color& rhs) const{
  const auto newTuple = this->_color + rhs._color;
  return Color(newTuple.x(), newTuple.y(), newTuple.z()); 
}

Color Color::operator-(const Color& rhs) const{
  const auto newTuple = this->_color - rhs._color;
  return Color(newTuple.x(), newTuple.y(), newTuple.z()); 
}

Color Color::operator*(const double& rhs) const{
  const auto newTuple = this->_color * rhs;
  return Color(newTuple.x(), newTuple.y(), newTuple.z());
}

Color Color::operator*(const Color& rhs) const{ // Hadamarad Product
  return Color(this->red()*rhs.red(), this->green()*rhs.green(), this->blue()*rhs.blue());
}

// Color setters and getters
const double& Color::red() const{
  return _color.x();
}
void Color::red(const double& val) {
  _color.x() = val;
}

const double& Color::green() const{
  return _color.y();
}
void Color::green(const double& val) {
  _color.y() = val;
}

const double& Color::blue() const{
  return _color.z();
}
void Color::blue(const double& val) {
  _color.z() = val;
}

} // namespace utility
} // namespace raytracer