#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <string>
#include <sstream>

#include "Color.hpp"

namespace raytracer {

class Canvas{
public:
  Canvas(size_t width_, size_t height_) noexcept;

  size_t pixelIndex(size_t x, size_t y) const noexcept; // helper to turn 2d index into 1d
  const utility::Color& pixelAt(size_t x, size_t y) const noexcept;
  void pixelWrite(const utility::Color& color, size_t x, size_t y) noexcept;

  void canvasToPPM(std::ostream& outputStream) const noexcept;
private:
  void PPMHeader(std::ostream& outputStream) const noexcept;
  void PPMData(std::ostream& outputStream) const noexcept;
  inline unsigned int convertColor(const float& colorComponent) const noexcept;
  size_t ColorComponentToPPM(const float& colorComponent, std::ostream& outputStream, size_t rowLineLen) const noexcept;
  size_t pixelToPPM(utility::Color const& pixel, std::ostream& outputStream, size_t rowLineLen) const noexcept;
  std::string rowToPPM(size_t rowIdx) const noexcept;

public:
  size_t width;
  size_t height;
  std::vector<utility::Color> pixels;
};

} // namespace raytracer

#endif // CANVAS_H