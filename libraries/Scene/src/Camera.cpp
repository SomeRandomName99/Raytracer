#include <algorithm>
#include <execution>
#include <numeric>
#include <vector>

#include "Camera.hpp"

namespace raytracer {
namespace scene {

utility::Ray Camera::rayForPixel(const unsigned int x, const unsigned int y) const{
  const auto xOffsetToPixelCenter = (x + 0.5) * this->pixelSize_;
  const auto yOffsetToPixelCenter = (y + 0.5) * this->pixelSize_;

  const auto worldX = this->halfWidth_  - xOffsetToPixelCenter;
  const auto worldY = this->halfHeight_ - yOffsetToPixelCenter;
  // z-coord is -1 because the canvas is always 1 unit away from the camera
  const auto pixel     = inverse(this->transform_) * utility::Point(worldX, worldY, -1); 
  const auto origin    = inverse(this->transform_) * utility::Point(0, 0, 0);
  const auto direction = (pixel - origin).normalize();

  return utility::Ray{origin, direction};
}

Canvas Camera::render(const World& world){
  auto image = Canvas(this->numHorPixels_, this->numVerPixels_);

  std::vector<size_t> pixelIndices(this->numHorPixels_ * this->numVerPixels_);
  std::iota(pixelIndices.begin(), pixelIndices.end(), 0);
  for_each(std::execution::par_unseq, pixelIndices.begin(), pixelIndices.end(),
           [this, &image, &world](const auto index) {
             const auto x = index % this->numHorPixels_;
             const auto y = index / this->numHorPixels_;
             const auto ray = this->rayForPixel(x, y);
             const auto color = world.colorAt(ray);
             image.pixelWrite(color, x, y);
           });

  return image;
}

} // namespace scene
} // namespace raytracer