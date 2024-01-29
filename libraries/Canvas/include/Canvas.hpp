#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <string>
#include <sstream>

#include "Color.hpp"

namespace raytracer {

class Canvas{
public:
  Canvas(size_t width_, size_t height_);

  size_t pixelIndex(size_t x, size_t y) const; // helper to turn 2d index into 1d
  const utility::Color& pixelAt(size_t x, size_t y) const;
  void pixelWrite(const utility::Color& color, size_t x, size_t y);

  void canvasToPPM(std::ostream& outputStream) const;
private:
  void PPMHeader(std::ostream& outputStream) const;
  void PPMData(std::ostream& outputStream) const;
  inline unsigned int convertColor(const double& colorComponent) const;
  size_t ColorComponentToPPM(const double& colorComponent, std::ostream& outputStream, size_t rowLineLen) const;
  size_t pixelToPPM(utility::Color const& pixel, std::ostream& outputStream, size_t rowLineLen) const;
  std::string rowToPPM(size_t rowIdx) const;

public:
  size_t width;
  size_t height;
  std::vector<utility::Color> pixels;
};

} // namespace raytracer

#endif // CANVAS_H