#ifndef TRANSFORMATION_H 
#define TRANSFORMATION_H

#include "Matrix.hpp"

namespace raytracer {
namespace utility {

namespace transformations{
  Matrix<4,4> translation(float x, float y, float z);
  Matrix<4,4> scaling(float x, float y, float z);

  Matrix<4,4> rotation_x(float rads);
  Matrix<4,4> rotation_y(float rads);
  Matrix<4,4> rotation_z(float rads);

  Matrix<4,4> shearing(float x_y, float x_z, 
                      float y_x, float y_z,
                      float z_x, float z_y);

  Matrix<4,4> view_transform(const Tuple& from, const Tuple& to, const Tuple& up);
}

} // namespace utility
} // namespace raytracer

#endif // TRANSFORMATION_H