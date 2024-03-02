#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "Color.hpp"

namespace raytracer {
namespace material {

struct StripePattern{
    StripePattern(utility::Color a, utility::Color b) : a{a}, b{b} {}
    utility::Color a;
    utility::Color b;
};

utility::Color stripe_at(const StripePattern& pattern, const utility::Tuple& point) noexcept;

} // namespace material
} // namespace raytracer

#endif // PATTERNS_HPP