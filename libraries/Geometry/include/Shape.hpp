#ifndef SHAPET_HPP
#define SHAPET_HPP

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

class ShapeBase {
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
    const auto localPoint  = inverseTransform() * point;
    const auto localNormal = localNormalAt(localPoint);
    // This is needed because the normal utility::Vector is a linear form, see below:
    // https://computergraphics.stackexchange.com/questions/1502/why-is-the-transposed-inverse-of-the-model-view-matrix-used-to-transform-the-nor
    auto worldNormal  = inverseTransform().transpose() * localNormal;
    worldNormal.w() = 0;

    return worldNormal.normalize();
  }

protected:
  virtual std::vector<Intersection> localIntersect(const utility::Ray& transformedRay) const noexcept = 0;
  virtual utility::Tuple localNormalAt(const utility::Tuple &objectPoint) const noexcept = 0;
private:
  utility::Matrix<4,4> transformation_;
  utility::Matrix<4,4> inverseTransformation_;
  material::Material material_;
  std::size_t id_;
  bool hasShadow_;

  static inline std::atomic<size_t> ID{};
};

} // namespace geometry
} // namespace raytracer

#endif // SHAPET_HPP