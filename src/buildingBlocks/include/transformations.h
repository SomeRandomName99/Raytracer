#ifndef TRANSFORMATION_H 
#define TRANSFORMATION_H

#include "include/matrix.h"

Matrix<4,4> translation(double x, double y, double z);
Matrix<4,4> scaling(double x, double y, double z);


#endif