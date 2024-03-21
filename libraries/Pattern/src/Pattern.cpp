#include <cmath>

#include "Pattern.hpp"

namespace raytracer {
namespace material {

struct patternEqualityVisitor {
  template<typename T>
  bool operator()(const T& lhs, const T& rhs) const noexcept {
    return lhs == rhs;
  }

  template<typename T, typename U>
  bool operator()(const T&, const U&) const noexcept {
    return false;
  }
};

bool operator==(const Pattern& lhs, const Pattern& rhs) noexcept{
    return lhs.transformation() == rhs.transformation() &&
           std::visit(patternEqualityVisitor{}, lhs.pattern(), rhs.pattern());
}

} // namespace material
} // namespace raytracer