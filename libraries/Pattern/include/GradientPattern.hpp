#ifndef GRADIENTPATTERNS_HPP
#define GRADIENTPATTERNS_HPP

#include <cmath>

#include "PatternT.hpp"
#include "Color.hpp"

namespace raytracer {
namespace material {

class GradientPattern: public PatternT<GradientPattern>{
public:
    GradientPattern(utility::Color a, utility::Color b) : a{a}, b{b}{}

    utility::Color localPattern_at(const utility::Tuple& point) const noexcept {
      auto distance = b - a;
      auto fraction = point.x() - std::floor(point.x());
      return a + distance * fraction;
    }
    utility::Color a;
    utility::Color b;
};

bool operator==(const GradientPattern& lhs, const GradientPattern& rhs) noexcept;

} // namespace material
} // namespace raytracer

#endif //GRADIENTPATTERNS_HPP