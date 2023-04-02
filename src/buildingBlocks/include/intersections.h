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

/**
 * @brief A function that returns the intersection with the least distance.
 * 
 * This is used to find the first object that is intersection in order to display it first.
 * 
 * @param intersections a vector containing information about all the intersected objects
 * @return Intersection* a pointer to the struct containing info the first object that the ray intersected with
 */
Intersection* hit(const std::vector<Intersection>& intersectionss);


#endif // INTERSECTIONS_H