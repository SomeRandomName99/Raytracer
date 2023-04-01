#include <vector>

#include "include/tuple.h"
#include "include/ray.h"

class Sphere
{
private:

public:
  Sphere() = default;
  ~Sphere() = default;

  /*
  * \brief Return the t values correspondig to points at which the ray intersects the sphere's surface
  *
  * This function will also calculate intersection behind the origin's start point, as they will be used for calculating
  * reflectionsn and other lighting effects later on.
  * 
  * \param ray a Ray object contaning basic info such as direction and starting point.
  */
  std::vector<double> intersect(const Ray::Ray& ray) const;
};
