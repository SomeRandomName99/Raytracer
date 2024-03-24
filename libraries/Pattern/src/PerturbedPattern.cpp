#include "PerturbedPattern.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "Pattern.hpp"

namespace raytracer {
namespace material {

struct localPatternAtVisitor {
    const utility::Tuple& point;

    template<typename T>
    utility::Color operator()(const T& pattern) const noexcept {
        return pattern.localPattern_at(point);
    }
};

utility::Color PerturbedPattern::localPattern_at(const utility::Tuple& point) const noexcept {
    auto noise = stb_perlin_noise3(point.x(), point.y(), point.z(), 0, 0, 0);
    auto perturbedPoint = point + noise;
    return std::visit(localPatternAtVisitor{perturbedPoint}, pattern_->pattern());
}

bool operator==(const PerturbedPattern& lhs, const PerturbedPattern& rhs) noexcept {
    return lhs.pattern_ == rhs.pattern_;
}

} // namespace material
} // namespace raytracer