#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <atomic>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Transformations.hpp"
#include "Matrix.hpp"

namespace raytracer {
namespace geometry {

struct Sphere
{
Sphere(): id{ID++}, transformation{utility::Matrix<4,4>::identity()}{}
bool operator==(Sphere const& other) const;
void setTransform(const utility::Matrix<4,4> &transformation);
utility::Tuple normalAt(const utility::Tuple &point) const;

std::size_t id;
unsigned long int radius;
utility::Tuple origin;
utility::Matrix<4,4> transformation;

private: 
static std::atomic<size_t> ID;
};

} // namespace geometry
} // namespace raytracer

#endif // SPHERE_H