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