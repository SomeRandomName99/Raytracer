#include <memory>

#include "include/color.h"
#include "include/canvas.h"

Canvas::Canvas(size_t width_, size_t height_): width{width_}, 
                                               height{height_} {
  this->pixels.resize(width_*height_);
}

// helper to turn 2d index into 1d
size_t Canvas::pixelIndex(size_t x, size_t y) const{
  return x+y*this->width;
}
const Color& Canvas::pixelAt(size_t x, size_t y) const{
  return this->pixels.at(pixelIndex(x,y));
}
void Canvas::pixelWrite(const Color& color, size_t x, size_t y){
  this->pixels.at(pixelIndex(x,y)) = color;
}