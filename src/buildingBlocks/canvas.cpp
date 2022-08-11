#include <cmath>
#include <ranges>
#include <iostream>

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

void Canvas::canvasToPPM(std::ostream& outputStream) const{
  PPMHeader(outputStream);
  PPMData(outputStream);
}

void Canvas::PPMHeader(std::ostream& outputStream) const{
  outputStream << "P3\n" << this->width << " " << this->height << '\n'
               << 255 << '\n';
}

void Canvas::PPMData(std::ostream& outputStream) const{
  for(size_t&& i : std::views::iota(0u, this->height)){
    outputStream << rowToPPM(i);
  }
}

inline unsigned int Canvas::convertColor(const float& colorComponent) const{
  // map the range 0.0-1.0 to 0-255 which is used for the PPM format
  if(std::isgreater(colorComponent, 1.0f)){return 255;}
  else if(std::isless(colorComponent, 0.0f)){return 0;}
  else{return std::ceil(255*colorComponent);}
}

size_t Canvas::ColorComponentToPPM(const float& colorComponent, std::ostream& outputStream, size_t rowLineLen) const{
  const auto maxLineLen = 70u;
  std::string ComponentString{std::to_string(convertColor(colorComponent))};

  if((rowLineLen + ComponentString.size()) >= maxLineLen){
    outputStream << '\n';
    rowLineLen = 0;
  }

  if(rowLineLen){outputStream << ' '; rowLineLen++;}
  
  outputStream << ComponentString;
  return rowLineLen+ComponentString.size();
}

size_t Canvas::pixelToPPM(Color const& pixel, std::ostream& outputStream, size_t rowLineLen) const{
  rowLineLen = ColorComponentToPPM(pixel.red(), outputStream, rowLineLen);
  rowLineLen = ColorComponentToPPM(pixel.green(), outputStream, rowLineLen);
  rowLineLen = ColorComponentToPPM(pixel.blue(), outputStream, rowLineLen);
  return rowLineLen;
}

// Todo: Need to check whether to add a new line per color component
std::string Canvas::rowToPPM(size_t rowIdx) const{
  std::ostringstream stream;
  const size_t offset{this->width*rowIdx};
  size_t rowLineLen{};
  for(auto&& pixel : std::views::counted(this->pixels.begin()+offset, this->width))
      rowLineLen = pixelToPPM(pixel, stream, rowLineLen);
  stream << "\n";
  return stream.str();
}
