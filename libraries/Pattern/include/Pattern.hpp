#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "Color.hpp"
#include "Matrix.hpp"

namespace raytracer {
namespace material {

class Shape;

class StripePattern{
public:
    StripePattern(utility::Color a, utility::Color b) : a{a}, b{b}, transformation{utility::Matrix<4,4>::identity()},
                                                        inverseTransformation{utility::Matrix<4,4>::identity()}{}
    const utility::Matrix<4, 4>& transform() const noexcept { return transformation; }
    const utility::Matrix<4, 4>& inverseTransform() const noexcept { return inverseTransformation; }
    void setTransform(const utility::Matrix<4, 4>& transform) noexcept {
        transformation = transform;
        inverseTransformation = inverse(transformation);
    }

    utility::Color a;
    utility::Color b;

private:
    utility::Matrix<4, 4> transformation;
    utility::Matrix<4, 4> inverseTransformation;
};

utility::Color stripe_at_object(const StripePattern& pattern, const Shape& object, const utility::Tuple& worldPoint) noexcept;
utility::Color stripe_at(const StripePattern& pattern, const utility::Tuple& point) noexcept;

} // namespace material
} // namespace raytracer

#endif // PATTERNS_HPP