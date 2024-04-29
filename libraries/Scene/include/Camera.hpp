#include "Matrix.hpp"
#include "Ray.hpp"
#include "Canvas.hpp"
#include "World.hpp"

namespace raytracer {
namespace scene {

class Camera {
public:

Camera(unsigned int numHorPixels, unsigned int numVerPixels, float fov) noexcept
    : numHorPixels_{numHorPixels}, numVerPixels_{numVerPixels}, fov_{fov}, transform_{utility::Matrix<4,4>::identity()} {
      // Imagine a triangle made from the camera to the canvas(1 unit away), the angle of which it the fov.
      // We calculate the half width because we can make a right angle triangle with adjacent = 1 and angle = fov/2.
      const auto halfView = std::tan(fov / 2);
      const auto aspect = static_cast<float>(numHorPixels) / static_cast<float>(numVerPixels);

      if(aspect >= 1){
        halfWidth_ = halfView;
        halfHeight_ = halfView / aspect;
      } else {
        // To think of this correctly, imagine the fov as the angle with which we cover the whole screen
        // So if we have more vertical pixels than horizintal, then our angle needs to on the vertical axis
        halfWidth_ = halfView * aspect;
        halfHeight_ = halfView;
      }

      pixelSize_ = halfWidth_ / static_cast<float>(numHorPixels_) * 2; // fullWidth/NumofHorizontalPixels
}

/**
 * \brief Calculates the ray corresponding to a pixel in the camera's view.
 *
 * \param x The x-coordinate of the pixel.
 * \param y The y-coordinate of the pixel.
 * \return The ray corresponding to the given pixel.
 */
utility::Ray rayForPixel(const unsigned int x, const unsigned int y) const noexcept;

/**
 * Renders the scene using the specified camera and world.
 *
 * @param world The world containing the objects and lights in the scene.
 * @return The rendered image as a Canvas object.
 */
Canvas render(const World& world) noexcept;

unsigned int numHorPixels_;
unsigned int numVerPixels_;
float fov_;
utility::Matrix<4,4> transform_;
float halfWidth_;
float halfHeight_;
float pixelSize_;
};

} // namespace raytracer
} // namespace scene