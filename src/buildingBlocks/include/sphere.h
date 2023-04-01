#include <vector>

#include "include/tuple.h"
#include "include/ray.h"

class Sphere
{
private:

public:
  Sphere() = default;
  ~Sphere() = default;

  std::vector<double> intersect(const Ray::Ray& ray) const;
};
