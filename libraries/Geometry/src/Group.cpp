#include <memory>
#include "Group.hpp"
#include "Arena.hpp"
namespace raytracer { 
namespace geometry {

Group::Group() noexcept {
  this->setBoundingBox(utility::AABB(utility::Point(0, 0, 0), utility::Point(0, 0, 0)));
}

void Group::localIntersect(const utility::Ray &transformedRay, utility::Arena<Intersection>& intersections) const noexcept {
    for(const auto &child : children){
        child->intersect(transformedRay, intersections);
    }
}

void Group::addChild(std::shared_ptr<ShapeBase> child) noexcept {
  child->setParent(shared_from_this());
  children.emplace_back(child);

  // Update the bounding box of the group
  auto childBoundingBox = child->getBoundingBox();
  this->getBoundingBox().expandToInclude(childBoundingBox.transform(child->transformation()));
}
std::vector<std::shared_ptr<ShapeBase>> const& Group::getChildren() const noexcept{
  return children;
}

std::shared_ptr<Group> makeGroup() noexcept {
  return std::make_shared<Group>();
}

} // namespace geometry
} // namespace raytracer