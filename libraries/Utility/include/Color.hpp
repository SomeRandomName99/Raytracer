#ifndef COLOR_H
#define COLOR_H

#include "Tuple.hpp"

namespace raytracer {
namespace utility {

class Color
{
public:
   Color();
   Color(float red, float green, float blue);

  bool operator==(const Color& rhs) const;
  Color operator+(const Color& rhs) const;
  Color operator+=(const Color& rhs);
  Color operator-(const Color& rhs) const;
  Color operator*(const float& rhs) const;
  Color operator*(const Color& rhs) const; // Hadamarad Product

  // Color setters and getters
  const float& red() const;
  void red(const float& val);

  const float& green() const;
  void green(const float& val);

  const float& blue() const;
  void blue(const float& val);

  utility::Tuple  _color;
};

} // namespace utility
} // namespace raytracer  

#endif // COLOR_H