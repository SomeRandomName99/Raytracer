#ifndef PATTERNT_HPP
#define PATTERNT_HPP

#include <memory>

#include "Matrix.hpp"
#include "Color.hpp"

namespace raytracer {
namespace material {

namespace details {
class PatternConcept {
public:
  virtual ~PatternConcept() {}
  virtual utility::Color drawPatternAt(const utility::Matrix<4,4>& objectInverseTransformation, const utility::Tuple& worldPoint) const noexcept = 0;
  virtual void setTransform(const utility::Matrix<4,4> &transformation) noexcept = 0;
  virtual const utility::Matrix<4,4>& transform() const noexcept = 0;
  virtual const utility::Matrix<4,4>& inverseTransform() const noexcept = 0;
  virtual std::unique_ptr<PatternConcept> clone() const = 0;
  virtual bool isEquals(const PatternConcept& other) const noexcept = 0;
};

template <typename PatternT>
class OwningPatternModel : public PatternConcept {
public:
  OwningPatternModel(PatternT pattern) noexcept : pattern_{std::move(pattern)} {}
  OwningPatternModel(PatternT pattern, const utility::Matrix<4,4>& transformation)
    : transformation_{transformation},
      inverseTransformation_{inverse(transformation)},
      pattern_{pattern} {}
      
  std::unique_ptr<PatternConcept> clone() const override {
    return std::make_unique<OwningPatternModel<PatternT>>(pattern_, transformation_);
  }

  utility::Color drawPatternAt(const utility::Matrix<4,4>& objectInverseTransformation, const utility::Tuple& worldPoint) const noexcept override {
    const auto objectPoint  = objectInverseTransformation * worldPoint;
    const auto patternPoint = this->inverseTransform() * objectPoint;
    return pattern_.drawPatternAt(patternPoint);
  }

  void swap(OwningPatternModel& other) noexcept {
    std::swap(pattern_, other.pattern_);
  }

  bool isEquals(const PatternConcept& other) const noexcept override {
    using Model = OwningPatternModel<PatternT>;
    const auto * model = dynamic_cast<const Model*>(&other);
    return (model && pattern_ == model->pattern_);
  }

  void setTransform(const utility::Matrix<4,4> &transformation) noexcept override {
    transformation_ = transformation;
    inverseTransformation_ = inverse(transformation_);
  }

  const utility::Matrix<4,4>& transform() const noexcept override {
    return transformation_;
  }

  const utility::Matrix<4,4>& inverseTransform() const noexcept override {
    return inverseTransformation_;
  }

private:
  utility::Matrix<4,4> transformation_{utility::Matrix<4,4>::identity()};
  utility::Matrix<4,4> inverseTransformation_{utility::Matrix<4,4>::identity()};
  PatternT pattern_;
};

} // namespace details

using namespace details;
class Pattern {
public:
  template <typename PatternT>
  Pattern(PatternT pattern) noexcept {
    pimpl_ = std::make_unique<OwningPatternModel<PatternT>>(std::move(pattern));
  }

  Pattern(const Pattern& other) noexcept: pimpl_{other.pimpl_->clone()} {}

  Pattern& operator=(const Pattern& other) noexcept {
    Pattern tmp{other};
    pimpl_.swap(tmp.pimpl_);
    return *this;
  }

  utility::Color drawPatternAt(const utility::Matrix<4,4>& objectInverseTransformation, const utility::Tuple& worldPoint) const noexcept {
    return pimpl_->drawPatternAt(objectInverseTransformation, worldPoint);
  }

  void setTransform(const utility::Matrix<4,4> &transformation) noexcept {
    pimpl_->setTransform(transformation);
  }

  const utility::Matrix<4,4>& transform() const noexcept {
    return pimpl_->transform();
  }

  const utility::Matrix<4,4>& inverseTransform() const noexcept {
    return pimpl_->inverseTransform();
  }

  ~Pattern() = default;
  Pattern(Pattern&&) = default;
  Pattern& operator=(Pattern&&) = default;

private:
  friend bool operator==(const Pattern& lhs, const Pattern& rhs) noexcept{
    return lhs.pimpl_->isEquals(*rhs.pimpl_);
  }
  friend bool operator!=(const Pattern& lhs, const Pattern& rhs) noexcept{
    return !(lhs == rhs);
  }

  std::unique_ptr<PatternConcept> pimpl_;
};

} // namespace material
} // namespace raytracer

#endif // PATTERNT_HPP