#ifndef COLOR_H
#define COLOR_H

#include "include/tuple.h"

class Color
{
public:
   Color();
   Color(float red, float green, float blue);

  bool operator==(const Color& rhs) const;
  Color operator+(const Color& rhs) const;
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

  Tuple  _color;
};

#endif // COLOR_H