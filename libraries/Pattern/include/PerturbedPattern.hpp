#ifndef PERTURBEDPATTERN_HPP
#define PERTURBEDPATTERN_HPP

#include <memory>

#include "Color.hpp"
#include "PatternT.hpp"

namespace raytracer {
namespace material {

class Pattern;

template <typename T>
concept NotSharedPattern = !std::is_same_v<std::shared_ptr<Pattern>, T>;


class PerturbedPattern: public PatternT<PerturbedPattern>{
public:
  PerturbedPattern(std::shared_ptr<Pattern> pattern) : pattern_{pattern}{}

  template <NotSharedPattern T>
  PerturbedPattern(const T& pattern) : pattern_{std::make_shared<Pattern>(pattern)} {}

  utility::Color localPattern_at(const utility::Tuple& point) const noexcept; 

  std::shared_ptr<Pattern> pattern_;
};

bool operator==(const PerturbedPattern& lhs, const PerturbedPattern& rhs) noexcept;

} // namespace material
} // namespace raytracer

#endif //PERTURBEDPATTERN_HPP