#ifndef COLOR_H
#define COLOR_H

#include "include/tuple.h"

class Color
{
public:
   Color();
   Color(double red, double green, double blue);

  bool operator==(const Color& rhs) const;
  Color operator+(const Color& rhs) const;
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

  Tuple  _color;
};

#endif // COLOR_H