#ifndef TRANSFORMATION_H 
#define TRANSFORMATION_H

#include "Matrix.hpp"

namespace raytracer {
namespace utility {

namespace transformations{
  Matrix<4,4> translation(const double x, const double y, const double z) noexcept;
  Matrix<4,4> scaling(const double x, const double y, const double z) noexcept;

  Matrix<4,4> rotation_x(const double rads) noexcept;
  Matrix<4,4> rotation_y(const double rads) noexcept;
  Matrix<4,4> rotation_z(const double rads) noexcept;

  Matrix<4,4> shearing(const double x_y, const double x_z, 
                       const double y_x, const double y_z,
                       const double z_x, const double z_y) noexcept;

  Matrix<4,4> view_transform(const Tuple& from, const Tuple& to, const Tuple& up) noexcept;
}

} // namespace utility
} // namespace raytracer

#endif // TRANSFORMATION_H