#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <atomic>

#include "include/tuple.h"
#include "include/ray.h"
#include "transformations.h"
#include "matrix.h"


struct Sphere
{
Sphere(): id{ID++}, transformation{Matrix<4,4>::identity()}{}
bool operator==(Sphere const& other) const;
void setTransform(const Matrix<4,4> &transformation);
Tuple normalAt(const Tuple &point) const;

std::size_t id;
unsigned long int radius;
Tuple origin;
Matrix<4,4> transformation;

private: 
static std::atomic<size_t> ID;
};


#endif // SPHERE_H