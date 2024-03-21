#ifndef PATTERNT_HPP
#define PATTERNT_HPP

#include "Matrix.hpp"
#include "Color.hpp"

namespace raytracer {
namespace material {

/* This PatternT class is a base class for all patterns within this ray tracing library. It provides and abstraction from
   the actual type of the pattern by implementing CRTP pattern.*/
template <typename concretePatternT>
class PatternT {
public:
  PatternT() noexcept: transformation_{utility::Matrix<4,4>::identity()},
                       inverseTransformation_{utility::Matrix<4,4>::identity()}{}

  void setTransform(const utility::Matrix<4,4> &transformation) noexcept {
    transformation_ = transformation;
    inverseTransformation_ = inverse(transformation_);
  }

  const utility::Matrix<4,4>& transformation() const noexcept {
    return transformation_;
  }

  const utility::Matrix<4,4>& inverseTransform() const noexcept {
    return inverseTransformation_;
  }

  utility::Color pattern_at_object(const utility::Matrix<4,4>& objectInverseTransformation, 
                                  const utility::Tuple& worldPoint) const noexcept{
    const auto objectPoint  = objectInverseTransformation * worldPoint;
    const auto patternPoint = this->inverseTransform() * objectPoint;
    return as_underlying().localPattern_at(patternPoint);
  }

  bool operator==(const PatternT& other) const noexcept {
    return transformation_ == other.transformation() && as_underlying() == other.as_underlying();
  }

private:
  utility::Matrix<4,4> transformation_;
  utility::Matrix<4,4> inverseTransformation_;

  inline concretePatternT& as_underlying() noexcept{
    return static_cast<concretePatternT&>(*this);
  }
  inline const concretePatternT& as_underlying() const noexcept{
    return static_cast<const concretePatternT&>(*this);
  }
};

} // namespace material
} // namespace raytracer

#endif // PATTERNT_HPP