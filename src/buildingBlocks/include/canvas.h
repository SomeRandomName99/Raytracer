#ifndef CANVAS_H
#define CANVAS_H

#include <vector>

#include "include/color.h"

class Canvas{
public:
  Canvas(size_t width_, size_t height_);

  size_t pixelIndex(size_t x, size_t y) const; // helper to turn 2d index into 1d
  const Color& pixelAt(size_t x, size_t y) const;
  void pixelWrite(const Color& color, size_t x, size_t y);

  size_t width;
  size_t height;
  std::vector<Color> pixels;
};

#endif // CANVAS_H