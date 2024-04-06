#ifndef PERTURBEDPATTERN_HPP
#define PERTURBEDPATTERN_HPP

#include <memory>

#include "Color.hpp"
#include "Matrix.hpp"
#include "Pattern.hpp"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

namespace raytracer {
namespace material {

template <typename PatternT>
class Perturb  {
public:
  Perturb(PatternT pattern) noexcept : pattern_{std::move(pattern)} {}

  utility::Color drawPatternAt(const utility::Tuple& point) const noexcept{
    auto noise = stb_perlin_noise3(point.x(), point.y(), point.z(), 0, 0, 0);
    auto perturbedPoint = point + noise;
    return pattern_.drawPatternAt(perturbedPoint);
  }

private:
friend bool operator==(const Perturb<PatternT>& lhs, const Perturb<PatternT>& rhs) noexcept {
  return lhs.pattern_ == rhs.pattern_;
}
PatternT pattern_;
};

} // namespace material
} // namespace raytracer

#endif //PERTURBEDPATTERN_HPP