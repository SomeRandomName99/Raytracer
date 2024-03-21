
#ifndef STRIPEPATTERNS_HPP
#define STRIPEPATTERNS_HPP

#include <cmath>

#include "PatternT.hpp"
#include "Color.hpp"

namespace raytracer {
namespace material {

class StripePattern: public PatternT<StripePattern>{
public:
    StripePattern(utility::Color a, utility::Color b) : a{a}, b{b}{}

    utility::Color localPattern_at(const utility::Tuple& point) const noexcept {
      if(static_cast<int>(std::floor(point.x())) % 2 == 0){
        return a;
      } else {
        return b;
      }
    }
    utility::Color a;
    utility::Color b;
};

bool operator==(const StripePattern& lhs, const StripePattern& rhs) noexcept;

} // namespace material
} // namespace raytracer

#endif //STRIPEPATTERNS_HPP