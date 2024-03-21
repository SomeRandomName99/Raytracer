#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include <variant>

#include "Color.hpp"
#include "Matrix.hpp"
#include "StripePattern.hpp"
#include "GradientPattern.hpp"

namespace raytracer {
namespace material {

/* This class serves as a wrapper around a variant of all the available patterns we have. It serves the purpose of 
   enabling compile time polymorphism by visiting all the pattern interface functions. */
class Pattern {
public:
    using PatternVariant = std::variant<StripePattern, GradientPattern>;

    Pattern(PatternVariant pattern) noexcept: pattern_{std::move(pattern)} {}
    template <typename PatternType>
    Pattern(const PatternType& pattern) noexcept: pattern_{PatternVariant{pattern}} {}
    
    void setTransform(const utility::Matrix<4,4> &transformation) noexcept {
        std::visit([transformation](auto &p) { p.setTransform(transformation); }, pattern_);
    }
    
    const utility::Matrix<4,4>& transformation() const noexcept {
        return std::visit([&](auto &p) -> auto const &
                        { return p.transformation(); },
                pattern_);
    }
    
    const utility::Matrix<4,4>& inverseTransform() const noexcept {
        return std::visit([&](auto &p) -> auto const &
                        { return p.inverseTransform(); },
                pattern_);
    } 

    PatternVariant pattern() const noexcept {
        return pattern_;
    }

    utility::Color pattern_at_object(const utility::Matrix<4,4>& objectInverseTransformation, 
                                    const utility::Tuple& worldPoint) const noexcept {
        return std::visit([objectInverseTransformation, worldPoint](auto &p) 
                            { return p.pattern_at_object(objectInverseTransformation, worldPoint); }, 
                            pattern_);
    }
private:
    PatternVariant pattern_;
};

bool operator==(const Pattern& lhs, const Pattern& rhs) noexcept;

} // namespace material
} // namespace raytracer

#endif // PATTERNS_HPP