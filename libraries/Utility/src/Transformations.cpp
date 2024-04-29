#include <cmath>

#include "Transformations.hpp"

namespace raytracer {
namespace utility {
namespace transformations{
  Matrix<4,4> translation(const float x, const float y, const float z) noexcept{
    auto translationMatrix = Matrix<4,4>::identity();
    
    translationMatrix.at(0,3) = x;
    translationMatrix.at(1,3) = y;
    translationMatrix.at(2,3) = z;
    return translationMatrix;
  }

  Matrix<4,4> scaling(const float x, const float y, const float z) noexcept{
    auto translationMatrix = Matrix<4,4>::identity();
    
    translationMatrix.at(0,0) = x;
    translationMatrix.at(1,1) = y;
    translationMatrix.at(2,2) = z;
    return translationMatrix;
  }

  Matrix<4,4> rotation_x(const float rads) noexcept{
    auto rotationMatrix = Matrix<4,4>::identity();

    rotationMatrix.at(1,1) =  std::cos(rads);
    rotationMatrix.at(1,2) = -std::sin(rads);
    rotationMatrix.at(2,1) =  std::sin(rads);
    rotationMatrix.at(2,2) =  std::cos(rads);
    return rotationMatrix;
  }
  Matrix<4,4> rotation_y(const float rads) noexcept{
    auto rotationMatrix = Matrix<4,4>::identity();

    rotationMatrix.at(0,0) =  std::cos(rads);
    rotationMatrix.at(0,2) =  std::sin(rads);
    rotationMatrix.at(2,0) = -std::sin(rads);
    rotationMatrix.at(2,2) =  std::cos(rads);
    return rotationMatrix;
  }
  Matrix<4,4> rotation_z(const float rads) noexcept{
    auto rotationMatrix = Matrix<4,4>::identity();

    rotationMatrix.at(0,0) =  std::cos(rads);
    rotationMatrix.at(0,1) = -std::sin(rads);
    rotationMatrix.at(1,0) =  std::sin(rads);
    rotationMatrix.at(1,1) =  std::cos(rads);
    return rotationMatrix;
  }

  Matrix<4,4> shearing(const float x_y, const float x_z, 
                       const float y_x, const float y_z,
                       const float z_x, const float z_y) noexcept{
    auto shearingMatrix = Matrix<4,4>::identity();

    shearingMatrix.at(0,1) =  x_y;
    shearingMatrix.at(0,2) =  x_z;
    shearingMatrix.at(1,0) =  y_x;
    shearingMatrix.at(1,2) =  y_z;
    shearingMatrix.at(2,0) =  z_x;
    shearingMatrix.at(2,1) =  z_y;

    return shearingMatrix;
  }

  Matrix<4,4> view_transform(const Tuple& from, const Tuple& to, const Tuple& up) noexcept{
    // https://learnopengl.com/Getting-started/Camera has a nice explanation of the code

    const auto forwardVector = (to - from).normalize();
    const auto leftVector    = forwardVector.cross(up.normalize());
    const auto trueUpVector  = leftVector.cross(forwardVector);
    
    const auto orientation = Matrix<4,4>{
      leftVector.x(),      leftVector.y(),     leftVector.z(),    0.0f,
      trueUpVector.x(),    trueUpVector.y(),   trueUpVector.z(),  0.0f,
      -forwardVector.x(), -forwardVector.y(), -forwardVector.z(), 0.0f,
      0.0f,                0.0f,               0.0f,              1.0
    };

    return orientation * translation(-from.x(), -from.y(), -from.z());
  }
}

} // namespace utility
} // namespace raytracer