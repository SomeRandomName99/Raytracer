#ifndef RINGPATTERN_HPP
#define RINGPATTERN_HPP

#include <cmath>

#include "PatternT.hpp"
#include "Color.hpp"

namespace raytracer {
namespace material {

class RingPattern: public PatternT<RingPattern>{
public:
    RingPattern(utility::Color a, utility::Color b) : a{a}, b{b}{}

    utility::Color localPattern_at(const utility::Tuple& point) const noexcept {
      if(static_cast<int>(std::floor(std::sqrt(point.x()*point.x() + point.z()*point.z()))) % 2 == 0){
        return a;
      } else {
        return b;
      }
    }
    utility::Color a;
    utility::Color b;
};

bool operator==(const RingPattern& lhs, const RingPattern& rhs) noexcept;

} // namespace material
} // namespace raytracer

#endif //RINGPATTERN_HPP