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

using namespace raytracer;

const auto shadow    = utility::Color(0.627f, 0.125f, 0.941f);
const auto wallcolor = utility::Color(0.0f, 0.0f, 0.0f);

utility::Tuple pixelToWorldCoords(unsigned int widthPixel, unsigned int heightPixel, unsigned int canvasSize, float canvasToWallConversionRatio){
  float x_axis = (widthPixel  - canvasSize/2.0) / canvasToWallConversionRatio;
  float y_axis = (heightPixel - canvasSize/2.0) / canvasToWallConversionRatio;

  return utility::Point(x_axis, y_axis, 0);
}

int main(void){
  const auto rayOrigin = utility::Point(0, 0, -3);
  auto sphere          = geometry::Sphere();
  sphere.setTransform(utility::transformations::shearing(1,0,0,0,0,0)*utility::transformations::scaling(1,0.5,1));

  // Assuming square wall and canvas
  const int wallZOffset    = 10;
  const int wallSize       = 10;
  const int canvasSize     = 500;
  const float canvasToWallConversionRatio = canvasSize/wallSize;
  auto canvas = Canvas(canvasSize, canvasSize);

  
  for (size_t x = 0; x < canvas.width; x++){
    for(size_t y = 0; y < canvas.height; y++){
      utility::Tuple pointOnWall = pixelToWorldCoords(x, y, canvasSize, canvasToWallConversionRatio);
      pointOnWall.z() = wallZOffset;

      utility::Ray::Ray firedRay{rayOrigin, (pointOnWall - rayOrigin).normalize()};
      auto intersections = geometry::intersect(sphere, firedRay);

      if(intersections.size() > 0){
        canvas.pixelWrite(shadow, x, y);
      } else {
        canvas.pixelWrite(wallcolor, x, y);
      }
    }
  }

  std::ofstream image{"BallProjection.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
}