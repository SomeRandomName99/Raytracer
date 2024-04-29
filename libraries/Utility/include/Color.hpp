#ifndef COLOR_H
#define COLOR_H

#include "Tuple.hpp"

namespace raytracer {
namespace utility {

class Color
{
public:
   Color() noexcept;
   Color(float red, float green, float blue) noexcept;

  bool operator==(const Color& rhs) const noexcept;
  Color operator+(const Color& rhs) const noexcept;
  Color operator+=(const Color& rhs) noexcept;
  Color operator-(const Color& rhs) const noexcept;
  Color operator*(const float& rhs) const noexcept;
  Color operator*(const Color& rhs) const noexcept; // Hadamarad Product

  // Color setters and getters
  const float& red() const noexcept;
  void red(const float& val) noexcept;

  const float& green() const noexcept;
  void green(const float& val) noexcept;

  const float& blue() const noexcept;
  void blue(const float& val) noexcept;

  utility::Tuple  _color;
};

utility::Color hexColor(unsigned int hex) noexcept;

} // namespace utility
} // namespace raytracer  

#endif // COLOR_H