
#ifndef STRIPEPATTERN_HPP
#define STRIPEPATTERN_HPP

#include <cmath>

#include "Pattern.hpp"
#include "Color.hpp"

namespace raytracer {
namespace material {

class StripePattern  {
public:
    StripePattern(utility::Color a, utility::Color b) : a{a}, b{b}{}

    utility::Color drawPatternAt(const utility::Tuple& point) const noexcept {
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

#endif //STRIPEPATTERN_HPP