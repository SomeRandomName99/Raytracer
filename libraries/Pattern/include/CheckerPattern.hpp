#ifndef CHECKERPATTERN_HPP
#define CHECKERPATTERN_HPP

#include <cmath>

#include "Pattern.hpp"
#include "Color.hpp"

namespace raytracer {
namespace material {

// 3D checker pattern
class CheckerPattern {
public :
    CheckerPattern(utility::Color a, utility::Color b) : a{a}, b{b}{}

    utility::Color drawPatternAt(const utility::Tuple& point) const noexcept {
        auto sum = std::floor(point.x()) + std::floor(point.y()) + std::floor(point.z());
        if (std::fmod(sum, 2) == 0) {
            return a;
        } else {
            return b;
        }
    }
    utility::Color a;
    utility::Color b;
};

bool operator==(const CheckerPattern& lhs, const CheckerPattern& rhs) noexcept;

} // namespace material
} // namespace raytracer

#endif //CHECKERPATTERN_HPP