#ifndef PATTERN_HPP
#define PATTERN_HPP

#include "Color.hpp"
#include "Matrix.hpp"
#include "Tuple.hpp"

namespace raytracer::material {
  enum class PatternType {
    Gradient,
    Checker,
    Ring,
    Stripe
  };

  struct PatternData {
    utility::Color a;
    utility::Color b;
  };

  struct Pattern {
    PatternType type;
    PatternData data;
    utility::Matrix<4, 4> transform = utility::Matrix<4, 4>::identity();
    utility::Matrix<4, 4> inverseTransform = utility::Matrix<4, 4>::identity();
    bool preturb = false;
  };

  void setPatternTransform(Pattern& pattern, const utility::Matrix<4, 4>& transform);
  utility::Color drawPatternAt(const Pattern& pattern, const utility::Tuple& point);
}
#endif // PATTERN_HPP