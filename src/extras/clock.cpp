#include <fstream>
#include <iostream>
#include <array>
#include <numbers>
#include <algorithm>

#include "include/tuple.h"
#include "include/canvas.h"
#include "include/transformations.h"

const auto orange = Color(1.0f, 0.65f, 0.0f);

void drawOnCanvas(Canvas& canvas, Tuple& point){
  auto x_axis = canvas.width/2 + point.x()*canvas.width/2*0.75;
  auto y_axis = canvas.height/2 + point.y()*canvas.height/2*0.75;

  std::cout << "x: " << x_axis << " y: " << y_axis << '\n';
  canvas.pixelWrite(orange, x_axis, y_axis);
}

int main(void){
  std::array<Tuple, 12> points;
  auto canvas = Canvas(500, 500);

  for (size_t index = 0; index < points.size(); index++){
    auto p = rotation_z((std::numbers::pi/6)*index)*Point(0,1,0);
    points.at(index) = p;
    
    std::cout << p << '\n';
    drawOnCanvas(canvas, points.at(index));
  }

  std::ofstream image{"clock.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
}