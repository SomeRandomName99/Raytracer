#ifndef TRANSFORMATION_H 
#define TRANSFORMATION_H

#include "Matrix.hpp"

namespace raytracer {
namespace utility {

namespace transformations{
  Matrix<4,4> translation(const float x, const float y, const float z) noexcept;
  Matrix<4,4> scaling(const float x, const float y, const float z) noexcept;

  Matrix<4,4> rotation_x(const float rads) noexcept;
  Matrix<4,4> rotation_y(const float rads) noexcept;
  Matrix<4,4> rotation_z(const float rads) noexcept;

  Matrix<4,4> shearing(const float x_y, const float x_z, 
                       const float y_x, const float y_z,
                       const float z_x, const float z_y) noexcept;

  Matrix<4,4> view_transform(const Tuple& from, const Tuple& to, const Tuple& up) noexcept;
}

} // namespace utility
} // namespace raytracer

#endif // TRANSFORMATION_H