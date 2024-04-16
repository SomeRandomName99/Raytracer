#include <memory>

#include "Group.hpp"

namespace raytracer { 
namespace geometry {

std::vector<Intersection> Group::localIntersect(const utility::Ray &transformedRay) const noexcept {
  std::vector<Intersection> xs{};

  for(const auto &child : children){
    auto childXs = child->intersect(transformedRay);
    xs.insert(xs.end(), childXs.begin(), childXs.end());
  }

  std::ranges::sort(xs, std::less<>{}, [](const auto intersection){return intersection.dist;});

  return xs;
}

void Group::addChild(std::shared_ptr<ShapeBase> child) noexcept {
  child->setParent(shared_from_this());
  children.emplace_back(child);
}
std::vector<std::shared_ptr<ShapeBase>> const& Group::getChildren() const noexcept{
  return children;
}

std::shared_ptr<Group> makeGroup() noexcept {
  return std::make_shared<Group>();
}

} // namespace geometry
} // namespace raytracer