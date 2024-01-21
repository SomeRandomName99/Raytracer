#include <fstream>
#include <iostream>
#include <array>
#include <numbers>
#include <algorithm>

#include "Tuple.hpp"
#include "Canvas.hpp"
#include "Transformations.hpp"


const auto orange = raytracer::utility::Color(1.0f, 0.65f, 0.0f);

void drawOnCanvas(raytracer::Canvas& canvas, raytracer::utility::Tuple& point){
  auto x_axis = canvas.width/2 + point.x()*canvas.width/2*0.75;
  auto y_axis = canvas.height/2 + point.y()*canvas.height/2*0.75;

  std::cout << "x: " << x_axis << " y: " << y_axis << '\n';
  canvas.pixelWrite(orange, x_axis, y_axis);
}

int main(void){
  std::array<raytracer::utility::Tuple, 12> points;
  auto canvas = raytracer::Canvas(500, 500);

  for (size_t index = 0; index < points.size(); index++){
    points.at(index) = raytracer::utility::transformations::rotation_z((std::numbers::pi/6)*index)*raytracer::utility::Point(0,1,0);
    
    std::cout << points.at(index) << '\n';
    drawOnCanvas(canvas, points.at(index));
  }

  std::ofstream image{"clock.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
}