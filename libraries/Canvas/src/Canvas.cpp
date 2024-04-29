#include <cmath>
#include <ranges>
#include <iostream>

#include "Canvas.hpp"

namespace raytracer {

Canvas::Canvas(size_t width_, size_t height_) noexcept: width{width_}, 
                                               height{height_} {
  this->pixels.resize(width_*height_);
}

// helper to turn 2d index into 1d
size_t Canvas::pixelIndex(size_t x, size_t y) const noexcept{
  return x+y*this->width;
}

const utility::Color& Canvas::pixelAt(size_t x, size_t y) const noexcept{
  return this->pixels.at(pixelIndex(x,y));
}

void Canvas::pixelWrite(const utility::Color& color, size_t x, size_t y) noexcept{
  this->pixels.at(pixelIndex(x,y)) = color;
}

void Canvas::canvasToPPM(std::ostream& outputStream) const noexcept{
  PPMHeader(outputStream);
  PPMData(outputStream);
}

void Canvas::PPMHeader(std::ostream& outputStream) const noexcept{
  outputStream << "P3\n" << this->width << " " << this->height << '\n'
               << 255 << '\n';
}

void Canvas::PPMData(std::ostream& outputStream) const noexcept{
  for(size_t&& i : std::views::iota(0u, this->height)){
    outputStream << rowToPPM(i);
  }
}

inline unsigned int Canvas::convertColor(const float& ColorComponent) const noexcept{
  // map the range 0.0-1.0 to 0-255 which is used for the PPM format
  if(std::isgreater(ColorComponent, 1.0)){return 255;}
  else if(std::isless(ColorComponent, 0.0)){return 0;}
  else{return std::ceil(255*ColorComponent);}
}

size_t Canvas::ColorComponentToPPM(const float& ColorComponent, std::ostream& outputStream, size_t rowLineLen) const noexcept{
  const auto maxLineLen = 70u;
  std::string ComponentString{std::to_string(convertColor(ColorComponent))};

  if((rowLineLen + ComponentString.size()) >= maxLineLen){
    outputStream << '\n';
    rowLineLen = 0;
  }

  if(rowLineLen){outputStream << ' '; rowLineLen++;}
  
  outputStream << ComponentString;
  return rowLineLen+ComponentString.size();
}

size_t Canvas::pixelToPPM(utility::Color const& pixel, std::ostream& outputStream, size_t rowLineLen) const noexcept{
  rowLineLen = ColorComponentToPPM(pixel.red(), outputStream, rowLineLen);
  rowLineLen = ColorComponentToPPM(pixel.green(), outputStream, rowLineLen);
  rowLineLen = ColorComponentToPPM(pixel.blue(), outputStream, rowLineLen);
  return rowLineLen;
}

std::string Canvas::rowToPPM(size_t rowIdx) const noexcept{
  std::ostringstream stream;
  const size_t offset{this->width*rowIdx};
  size_t rowLineLen{};
  for(auto&& pixel : std::views::counted(this->pixels.begin()+offset, this->width))
      rowLineLen = pixelToPPM(pixel, stream, rowLineLen);
  stream << "\n";
  return stream.str();
}

} // namespace raytracer