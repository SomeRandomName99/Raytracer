#include <fstream>
#include <iostream>
#include <array>
#include <numbers>
#include <algorithm>

#include "Tuple.hpp"
#include "Canvas.hpp"
#include "Transformations.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Intersections.hpp"

const auto shadow    = Color(0.627f, 0.125f, 0.941f);
const auto wallColor = Color(0.0f, 0.0f, 0.0f);

Tuple pixelToWorldCoords(unsigned int widthPixel, unsigned int heightPixel, unsigned int canvasSize, float canvasToWallConversionRatio){
  float x_axis = (widthPixel  - canvasSize/2.0) / canvasToWallConversionRatio;
  float y_axis = (heightPixel - canvasSize/2.0) / canvasToWallConversionRatio;

  return Point(x_axis, y_axis, 0);
}

int main(void){
  const auto rayOrigin = Point(0, 0, -3);
  auto sphere    = Sphere();
  sphere.setTransform(transformations::shearing(1,0,0,0,0,0)*transformations::scaling(1,0.5,1));

  // Assuming square wall and canvas
  const int wallZOffset    = 10;
  const int wallSize       = 10;
  const int canvasSize     = 500;
  const float canvasToWallConversionRatio = canvasSize/wallSize;
  auto canvas = Canvas(canvasSize, canvasSize);

  
  for (size_t x = 0; x < canvas.width; x++){
    for(size_t y = 0; y < canvas.height; y++){
      Tuple pointOnWall = pixelToWorldCoords(x, y, canvasSize, canvasToWallConversionRatio);
      pointOnWall.z() = wallZOffset;

      Ray::Ray firedRay{rayOrigin, (pointOnWall - rayOrigin).normalize()};
      auto intersections = intersect(sphere, firedRay);

      if(intersections.size() > 0){
        canvas.pixelWrite(shadow, x, y);
      } else {
        canvas.pixelWrite(wallColor, x, y);
      }
    }
  }

  std::ofstream image{"BallProjection.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
}