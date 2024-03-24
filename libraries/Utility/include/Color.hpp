#ifndef COLOR_H
#define COLOR_H

#include "Tuple.hpp"

namespace raytracer {
namespace utility {

class Color
{
public:
   Color() noexcept;
   Color(double red, double green, double blue) noexcept;

  bool operator==(const Color& rhs) const noexcept;
  Color operator+(const Color& rhs) const noexcept;
  Color operator+=(const Color& rhs) noexcept;
  Color operator-(const Color& rhs) const noexcept;
  Color operator*(const double& rhs) const noexcept;
  Color operator*(const Color& rhs) const noexcept; // Hadamarad Product

  // Color setters and getters
  const double& red() const noexcept;
  void red(const double& val) noexcept;

  const double& green() const noexcept;
  void green(const double& val) noexcept;

  const double& blue() const noexcept;
  void blue(const double& val) noexcept;

  utility::Tuple  _color;
};

utility::Color hexColor(unsigned int hex) noexcept;

} // namespace utility
} // namespace raytracer  

#endif // COLOR_H