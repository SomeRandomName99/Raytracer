#if !defined(INTERSECTIONS_H)
#define INTERSECTIONS_H

#include <vector>
#include <concepts>

class Sphere;

/**
 * @brief A class that stores relevant info regarding an intersecion
 * 
 * The class include store the t value which is used in calculating the distance from the ray's origin, as well as
 * the object that was intersected with. For now it is a hardcoded sphere but will be generalized later. 
 */
struct Intersection
{
  double t;
  const Sphere& object;
};

template<std::same_as<Intersection> ... Arg> 
auto intersections(const Arg&... arg){
  return std::vector<Intersection>{arg...};
}


#endif // INTERSECTIONS_H