#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <atomic>

#include "include/tuple.h"
#include "include/ray.h"


class Sphere
{
public:
  /**
  * @brief Return the t values correspondig to points at which the ray intersects the sphere's surface
  *
  * This function will also calculate intersection behind the origin's start point, as they will be used for calculating
  * reflectionsn and other lighting effects later on.
  * 
  * @param ray a Ray object contaning basic info such as direction and starting point.
  */
  Sphere(): id{ID++}{}
  bool operator==(Sphere const& other) const;

static std::atomic<size_t> ID;

std::size_t id;
unsigned long int radius;
Tuple origin;
};


#endif // SPHERE_H