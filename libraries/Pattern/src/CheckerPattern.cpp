#include "CheckerPattern.hpp"

namespace raytracer {
namespace material {

bool operator==(const CheckerPattern& lhs, const CheckerPattern& rhs) noexcept {
    return lhs.a == rhs.a && lhs.b == rhs.b;
}

} // namespace material
} // namespace raytracer