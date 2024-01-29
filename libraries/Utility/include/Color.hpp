#ifndef COLOR_H
#define COLOR_H

#include "Tuple.hpp"

namespace raytracer {
namespace utility {

class Color
{
public:
   Color();
   Color(double red, double green, double blue);

  bool operator==(const Color& rhs) const;
  Color operator+(const Color& rhs) const;
  Color operator+=(const Color& rhs);
  Color operator-(const Color& rhs) const;
  Color operator*(const double& rhs) const;
  Color operator*(const Color& rhs) const; // Hadamarad Product

  // Color setters and getters
  const double& red() const;
  void red(const double& val);

  const double& green() const;
  void green(const double& val);

  const double& blue() const;
  void blue(const double& val);

  utility::Tuple  _color;
};

} // namespace utility
} // namespace raytracer  

#endif // COLOR_H