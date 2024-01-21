#ifndef TRANSFORMATION_H 
#define TRANSFORMATION_H

#include "Matrix.hpp"

namespace raytracer {
namespace utility {

namespace transformations{
  Matrix<4,4> translation(double x, double y, double z);
  Matrix<4,4> scaling(double x, double y, double z);

  Matrix<4,4> rotation_x(double rads);
  Matrix<4,4> rotation_y(double rads);
  Matrix<4,4> rotation_z(double rads);

  Matrix<4,4> shearing(double x_y, double x_z, 
                      double y_x, double y_z,
                      double z_x, double z_y);
}

} // namespace utility
} // namespace raytracer

#endif // TRANSFORMATION_H