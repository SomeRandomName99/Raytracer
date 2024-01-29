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
#include "Light.hpp"

using namespace raytracer;

const auto shadow    = utility::Color(0.627f, 0.125f, 0.941f);
const auto wallcolor = utility::Color(0.0f, 0.0f, 0.0f);

utility::Tuple pixelToWorldCoords(unsigned int widthPixel, unsigned int heightPixel, unsigned int canvasSize, float canvasToWallConversionRatio){
  float x_axis = (widthPixel  - canvasSize/2.0f) / canvasToWallConversionRatio;
  float y_axis = (heightPixel - canvasSize/2.0f) / canvasToWallConversionRatio;

  return utility::Point(x_axis, y_axis, 0);
}

int main(void){
  const auto rayOrigin = utility::Point(0, 0, -3);
  auto sphere          = geometry::Sphere();
  sphere.material_.surfaceColor_ = utility::Color(1,0.2f,1);
  sphere.setTransform(utility::transformations::shearing(1,0,0,0,0,0)*utility::transformations::scaling(1,0.5f,1));

  auto lightSource = scene::PointLight(utility::Color(1,1,1), utility::Point(-10,10,-10));

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

      utility::Ray firedRay{rayOrigin, (pointOnWall - rayOrigin).normalize()};
      auto intersections = sphere.intersect(firedRay);

      if(intersections.size() > 0){
        auto hit               = geometry::hit(intersections);
        auto intersectionPoint = firedRay.position(hit->dist_);
        auto normal            = hit->object_->normalAt(intersectionPoint);
        auto eyeVector         = -firedRay.direction_;
        auto color             = scene::lighting(hit->object_->material_, lightSource, intersectionPoint, eyeVector, normal);   
        canvas.pixelWrite(color, x, y);
      } else {
        canvas.pixelWrite(wallcolor, x, y);
      }
    }
  }

  std::ofstream image{"BallProjection.ppm", std::ios::out | std::ios::trunc};
  canvas.canvasToPPM(image);
}