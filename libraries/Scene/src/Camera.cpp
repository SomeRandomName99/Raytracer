#include <algorithm>
#include <execution>
#include <numeric>
#include <vector>

#include "Camera.hpp"
#include "Intersections.hpp"
#include "Arena.hpp"

namespace raytracer {
namespace scene {

using namespace utility;

Ray Camera::rayForPixel(const unsigned int x, const unsigned int y) const noexcept{
  const auto xOffsetToPixelCenter = (x + 0.5) * this->pixelSize_;
  const auto yOffsetToPixelCenter = (y + 0.5) * this->pixelSize_;

  const auto worldX = this->halfWidth_  - xOffsetToPixelCenter;
  const auto worldY = this->halfHeight_ - yOffsetToPixelCenter;
  // z-coord is -1 because the canvas is always 1 unit away from the camera
  const auto pixel     = inverse(this->transform_) * Point(worldX, worldY, -1); 
  const auto origin    = inverse(this->transform_) * Point(0, 0, 0);
  const auto direction = (pixel - origin).normalize();

  return Ray{origin, direction};
}

Canvas Camera::render(const World& world) noexcept{
  auto image = Canvas(this->numHorPixels_, this->numVerPixels_);

  std::vector<size_t> pixelIndices(this->numHorPixels_ * this->numVerPixels_);
  std::iota(pixelIndices.begin(), pixelIndices.end(), 0);
  thread_local Arena<geometry::Intersection> intersectionsArena(MB(1));
  for_each(std::execution::par_unseq, pixelIndices.begin(), pixelIndices.end(),
           [this, &image, &world, &intersectionsArena](const auto index) {
             const auto x = index % this->numHorPixels_;
             const auto y = index / this->numHorPixels_;
             const auto ray = this->rayForPixel(x, y);
             intersectionsArena.clear();
             const auto color = world.colorAt(ray, intersectionsArena);
             image.pixelWrite(color, x, y);
           });

  return image;
}

} // namespace scene
} // namespace raytracer