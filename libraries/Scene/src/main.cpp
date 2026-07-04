#include <vector>

#include "libraries/Geometry/include/Shape.hpp"
#include "libraries/Material/include/Material.hpp"
#include "libraries/Material/include/Pattern.hpp"

namespace raytracer::scene {

using namespace material;
using namespace geometry;

constexpr size_t MAX_INTERSECTIONS = 5;

struct PointLight{
  utility::Color intensity_;
  utility::Tuple position_;
};

struct World {
  std::vector<PointLight> lights_;
  std::vector<Material> materials;
  std::vector<Pattern> patterns;
  std::vector<WorldObject> sceneNodes;
  std::vector<GroupData> groupData;
  std::vector<CircularSolidData> circularSolidData;
};

struct Camera {
Camera(unsigned int numHorPixels, unsigned int numVerPixels, double fov) noexcept
    : numHorPixels_{numHorPixels}, numVerPixels_{numVerPixels}, fov_{fov}, 
      transform_{utility::Matrix<4,4>::identity()}, 
      inverseTransform_{utility::Matrix<4,4>::identity()},
      cameraOrigin_{utility::Point(0, 0, 0)} {
      // Imagine a triangle made from the camera to the canvas(1 unit away), the angle of which it the fov.
      // We calculate the half width because we can make a right angle triangle with adjacent = 1 and angle = fov/2.
      const auto halfView = std::tan(fov / 2);
      const auto aspect = static_cast<double>(numHorPixels) / static_cast<double>(numVerPixels);

      if(aspect >= 1){
        halfWidth_ = halfView;
        halfHeight_ = halfView / aspect;
      } else {
        // To think of this correctly, imagine the fov as the angle with which we cover the whole screen
        // So if we have more vertical pixels than horizintal, then our angle needs to on the vertical axis
        halfWidth_ = halfView * aspect;
        halfHeight_ = halfView;
      }

      pixelSize_ = halfWidth_ / static_cast<double>(numHorPixels_) * 2; // fullWidth/NumofHorizontalPixels
}

/**
 * \brief Calculates the ray corresponding to a pixel in the camera's view.
 *
 * \param x The x-coordinate of the pixel.
 * \param y The y-coordinate of the pixel.
 * \return The ray corresponding to the given pixel.
 */
Ray Camera::rayForPixel(const unsigned int x, const unsigned int y) const noexcept{
  const auto xOffsetToPixelCenter = (x + 0.5) * this->pixelSize_;
  const auto yOffsetToPixelCenter = (y + 0.5) * this->pixelSize_;

  const auto worldX = this->halfWidth_  - xOffsetToPixelCenter;
  const auto worldY = this->halfHeight_ - yOffsetToPixelCenter;
  // z-coord is -1 because the canvas is always 1 unit away from the camera
  const auto pixel     = this->inverseTransform_ * Point(worldX, worldY, -1); 
  const auto direction = (pixel - this->cameraOrigin_).normalize();

  return Ray{this->cameraOrigin_, direction};
}

/**
 * Renders the scene using the specified camera and world.
 *
 * @param world The world containing the objects and lights in the scene.
 * @return The rendered image as a Canvas object.
 */
Canvas render(const World& world) noexcept;

void setTransform(const utility::Matrix<4,4>& transform) noexcept {
  transform_ = transform;
  inverseTransform_ = inverse(transform_);
  cameraOrigin_ = inverseTransform_ * utility::Point(0, 0, 0);  // Precompute origin
}

unsigned int numHorPixels_;
unsigned int numVerPixels_;
double fov_;
utility::Matrix<4,4> transform_;
utility::Matrix<4,4> inverseTransform_;  // Precomputed inverse
utility::Tuple cameraOrigin_;            // Precomputed camera origin in world space
double halfWidth_;
double halfHeight_;
double pixelSize_;
};


} // namespace raytracer::scene