#include "Tuple.hpp"
#include "Ray.hpp"
#include "Transformations.hpp"
#include "Matrix.hpp"
#include "Material.hpp"
#include "Intersections.hpp"

namespace raytracer {
namespace geometry {

template <typename concreteShapeT>
class Shape {
public:
  Shape() noexcept: id_{ID++}, transformation_{utility::Matrix<4,4>::identity()},
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

  /**
   * @brief Function to calculate the intersections between a ray and a sphere.
   */
  std::vector<Intersection> intersect(const utility::Ray& ray) noexcept {
    return as_underlying().localIntersect(utility::transform(ray, inverseTransform()));
  }

  /**
   * @brief Calculates the normal vector at a given point on the shape.
   */
  utility::Tuple normalAt(const utility::Tuple &point) noexcept {
    const auto localPoint  = inverseTransform() * point;
    const auto localNormal = as_underlying().localNormalAt(localPoint);
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
};

class TestShape : public Shape<TestShape> {
public:
  std::vector<Intersection> localIntersect(const utility::Ray& ray) noexcept {
    saved_ray = ray;
    return std::vector<Intersection>{};
  }

  utility::Tuple localNormalAt(const utility::Tuple &point) noexcept {
    return point;
  }

  utility::Ray saved_ray{};
};

} // namespace geometry
} // namespace raytracer