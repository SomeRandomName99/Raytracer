#ifndef COLOR_H
#define COLOR_H

#include "include/tuple.h"

class Color
{
public:
   Color();
   Color(float red, float green, float blue);
  ~Color() = default;

  bool operator==(const Color& rhs) const;
  Color operator+(const Color& rhs) const;
  Color operator-(const Color& rhs) const;
  Color operator*(const float& rhs) const;
  Color operator*(const Color& rhs) const; // Hadamarad Product

  Tuple  _color;
  float& red;
  float& green;
  float& blue;
};

#endif // COLOR_H