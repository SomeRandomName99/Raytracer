#ifndef GROUP_HPP
#define GROUP_HPP

#include <memory>
#include <vector>
#include <algorithm>

#include "Shape.hpp"

namespace raytracer{
namespace geometry{

template <typename T>
struct fail :std::false_type{};

class Group final : public ShapeBase {
public:
  Group() {}

  utility::Tuple localNormalAt([[maybe_unused]]const utility::Tuple &objectPoint) const {
    throw std::logic_error("Group::localNormalAt should not be called");
  }

  std::vector<Intersection> localIntersect(const utility::Ray &transformedRay) const noexcept;

  void addChild(std::shared_ptr<ShapeBase> child) noexcept;
  std::vector<std::shared_ptr<ShapeBase>> const& getChildren() const noexcept;

private:
  std::vector<std::shared_ptr<ShapeBase>> children{};
};

std::shared_ptr<Group> makeGroup() noexcept;

} // namespace geometry
} // namespace raytracer

#endif //GROUP_HPP