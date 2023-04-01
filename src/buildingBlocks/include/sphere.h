#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

#include "include/intersections.h"
#include "include/tuple.h"
#include "include/ray.h"

class Sphere
{
private:

public:
  /**
  * @brief Return the t values correspondig to points at which the ray intersects the sphere's surface
  *
  * This function will also calculate intersection behind the origin's start point, as they will be used for calculating
  * reflectionsn and other lighting effects later on.
  * 
  * @param ray a Ray object contaning basic info such as direction and starting point.
  */
  std::vector<Intersection> intersect(const Ray::Ray& ray) const;
  bool operator==(Sphere const& other) const;
};

#endif // SPHERE_H