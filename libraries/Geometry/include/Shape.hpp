#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <variant>
#include <atomic>
#include <memory>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Transformations.hpp"
#include "Matrix.hpp"
#include "Material.hpp"
#include "Intersections.hpp"

namespace raytracer {
namespace geometry {

// helper functions
inline utility::Tuple worldPointToObjectPoint(const ShapeBase* shape, const utility::Tuple& worldPoint) noexcept;
inline utility::Tuple objectNormalToWorldNormal(const ShapeBase* shape, const utility::Tuple& objectNormal) noexcept;

class ShapeBase : public std::enable_shared_from_this<ShapeBase> {
public:
  ShapeBase() noexcept: transformation_{utility::Matrix<4,4>::identity()},
                        inverseTransformation_{utility::Matrix<4,4>::identity()}, 
                        material_{}, id_{ID++}, hasShadow_{true}{}

  virtual ~ShapeBase() {}

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

  std::size_t id() const noexcept {
    return id_;
  }

  const material::Material& material() const noexcept {
    return material_;
  }
  material::Material& material() noexcept {
    return const_cast<material::Material&>(const_cast<const ShapeBase*>(this)->material());
  }

  void setMaterial(material::Material& material) noexcept {
    material_ = std::move(material);
  }
  void setMaterial(material::Material&& material) noexcept {
    material_ = std::move(material);
  }

  bool hasShadow() const noexcept {
    return hasShadow_;
  }
  void setShadow(bool hasShadow) noexcept {
    hasShadow_ = hasShadow;
  }

  const std::shared_ptr<ShapeBase> parent() const noexcept {
    return parent_;
  }
  void setParent(std::shared_ptr<ShapeBase> parent) noexcept {
    parent_ = std::move(parent);
  }

  /**
   * @brief Function to calculate the intersections between a ray and a sphere.
   */
  std::vector<Intersection> intersect(const utility::Ray& ray) const noexcept {
    return localIntersect(utility::transform(ray, inverseTransform()));
  }

  /**
   * @brief Calculates the normal vector at a given point on the shape.
   */
  utility::Tuple normalAt(const utility::Tuple &point) const noexcept {
    const auto localPoint  = worldPointToObjectPoint(this, point);
    const auto localNormal = localNormalAt(localPoint);

    return objectNormalToWorldNormal(this, localNormal);
  }

protected:
  virtual std::vector<Intersection> localIntersect(const utility::Ray& transformedRay) const noexcept = 0;
  virtual utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const = 0;
private:
  utility::Matrix<4,4> transformation_;
  utility::Matrix<4,4> inverseTransformation_;
  material::Material material_;
  std::size_t id_;
  std::shared_ptr<ShapeBase> parent_{};
  bool hasShadow_;

  static inline std::atomic<size_t> ID{};
};

inline utility::Tuple worldPointToObjectPoint(const ShapeBase* shape, const utility::Tuple& worldPoint) noexcept {
  auto point = worldPoint;
  if (shape->parent() != nullptr) {
    point = worldPointToObjectPoint(shape->parent().get(), point);
  }
  return shape->inverseTransform() * point;
}

inline utility::Tuple objectNormalToWorldNormal(const ShapeBase* shape, const utility::Tuple& objectNormal) noexcept {
  // The transpose is needed because the normal utility::Vector is a linear form, see below:
  // https://computergraphics.stackexchange.com/questions/1502/why-is-the-transposed-inverse-of-the-model-view-matrix-used-to-transform-the-nor
  auto normal = shape->inverseTransform().transpose() * objectNormal;

  if (shape->parent() == nullptr) {
    normal.w() = 0;
    normal = normal.normalize();
    return normal;
  }
  return objectNormalToWorldNormal(shape->parent().get(), normal);
}

} // namespace geometry
} // namespace raytracer

#endif // SHAPE_HPP