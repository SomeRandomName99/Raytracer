#include <cmath>

#include "include/transformations.h"


Matrix<4,4> translation(double x, double y, double z){
  auto translationMatrix = Matrix<4,4>::identity();
  
  translationMatrix.at(0,3) = x;
  translationMatrix.at(1,3) = y;
  translationMatrix.at(2,3) = z;
  return translationMatrix;
}

Matrix<4,4> scaling(double x, double y, double z){
  auto translationMatrix = Matrix<4,4>::identity();
  
  translationMatrix.at(0,0) = x;
  translationMatrix.at(1,1) = y;
  translationMatrix.at(2,2) = z;
  return translationMatrix;
}

Matrix<4,4> rotation_x(double rads){
  auto rotationMatrix = Matrix<4,4>::identity();

  rotationMatrix.at(1,1) =  std::cos(rads);
  rotationMatrix.at(1,2) = -std::sin(rads);
  rotationMatrix.at(2,1) =  std::sin(rads);
  rotationMatrix.at(2,2) =  std::cos(rads);
  return rotationMatrix;
}
Matrix<4,4> rotation_y(double rads){
  auto rotationMatrix = Matrix<4,4>::identity();

  rotationMatrix.at(0,0) =  std::cos(rads);
  rotationMatrix.at(0,2) =  std::sin(rads);
  rotationMatrix.at(2,0) = -std::sin(rads);
  rotationMatrix.at(2,2) =  std::cos(rads);
  return rotationMatrix;
}
Matrix<4,4> rotation_z(double rads){
  auto rotationMatrix = Matrix<4,4>::identity();

  rotationMatrix.at(0,0) =  std::cos(rads);
  rotationMatrix.at(0,1) = -std::sin(rads);
  rotationMatrix.at(1,0) =  std::sin(rads);
  rotationMatrix.at(1,1) =  std::cos(rads);
  return rotationMatrix;
}