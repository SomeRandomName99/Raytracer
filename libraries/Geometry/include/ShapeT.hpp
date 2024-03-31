#ifndef SHAPET_HPP
#define SHAPET_HPP

#include <variant>
#include <atomic>

#include "Tuple.hpp"
#include "Ray.hpp"
#include "Transformations.hpp"
#include "Matrix.hpp"
#include "Material.hpp"
#include "Intersections.hpp"

namespace raytracer {
namespace geometry {

/* This ShapeT class is a base class for all shapes within this ray tracing library. It provides and abstraction from
   the actual type of the shape by implementing CRTP pattern.*/
template <typename concreteShapeT>
class ShapeT {
public:
  ShapeT() noexcept: id_{ID++}, transformation_{utility::Matrix<4,4>::identity()},
                    inverseTransformation_{utility::Matrix<4,4>::identity()}{}

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
    return const_cast<material::Material&>(const_cast<const ShapeT*>(this)->material());
  }

  void setMaterial(const material::Material& material) noexcept {
    material_ = material;
  }

  /**
   * @brief Function to calculate the intersections between a ray and a sphere.
   */
  std::vector<Intersection> intersect(const utility::Ray& ray) const noexcept {
    return as_underlying().localIntersect(utility::transform(ray, inverseTransform()));
  }

  /**
   * @brief Calculates the normal vector at a given point on the shape.
   */
  utility::Tuple normalAt(const utility::Tuple &point) const noexcept {
    const auto localPoint  = inverseTransform() * point;
    const auto localNormal = as_underlying().localNormalAt(localPoint);
    // This is needed because the normal utility::Vector is a linear form, see below:
    // https://computergraphics.stackexchange.com/questions/1502/why-is-the-transposed-inverse-of-the-model-view-matrix-used-to-transform-the-nor
    auto worldNormal  = inverseTransform().transpose() * localNormal;
    worldNormal.w() = 0;

    return worldNormal.normalize();
  }

  utility::Tuple origin_;
  material::Material material_;
private:
  std::size_t id_;
  utility::Matrix<4,4> transformation_;
  utility::Matrix<4,4> inverseTransformation_;

  static inline std::atomic<size_t> ID{};

  inline concreteShapeT& as_underlying() noexcept {
    return static_cast<concreteShapeT&>(*this);
  }
  inline const concreteShapeT& as_underlying() const noexcept {
    return static_cast<const concreteShapeT&>(*this);
  }
};

} // namespace geometry
} // namespace raytracer

#endif // SHAPET_HPP