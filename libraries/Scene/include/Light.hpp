#ifndef LIGHT_HPP
#define LIGHT_HPP

namespace raytracer::scene{

struct PointLight{
  utility::Color intensity;
  utility::Tuple position;
};

} // namespace raytracer::scene

#endif // LIGHT_HPP