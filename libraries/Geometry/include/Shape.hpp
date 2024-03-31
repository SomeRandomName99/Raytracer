#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <variant>
#include <vector>
#include <concepts>

#include "Ray.hpp"
#include "Tuple.hpp"
#include "Matrix.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

namespace raytracer {
namespace geometry {

struct Intersection;
class Material;

/* This class serves as a wrapper around a variant of all the available shapes we have. It serves the purpose of 
   enabling compile time polymorphism by visiting all the shape interface functions. */
class Shape {
public:
  using ShapeVariant = std::variant<Plane, Sphere>;

  Shape(ShapeVariant shape) noexcept: shape_{std::move(shape)} {}

  void setTransform(const utility::Matrix<4,4> &transformation) noexcept {
    std::visit([transformation](auto &s) { s.setTransform(transformation); }, shape_);
  }

  const utility::Matrix<4,4>& transformation() const noexcept {
    return std::visit([&](auto &s) -> auto const &
                      { return s.transformation(); },
            shape_);
  }

  const utility::Matrix<4,4>& inverseTransform() const noexcept {
    return std::visit([&](auto &s) -> auto const &
                      { return s.inverseTransform(); },
            shape_);
  } 

  const material::Material& material() const noexcept {
    return std::visit([](auto &s) -> auto const &
                      { return s.material(); }, 
            shape_);
  }
  material::Material& material() noexcept {
    return std::visit([](auto &s)  -> auto &
                      { return s.material(); }, 
                      shape_);
  }

  void setMaterial(const material::Material& material) noexcept {
    std::visit([material](auto &s) { s.setMaterial(material); }, shape_);
  }

  std::size_t id() const noexcept {
    return std::visit([](auto &s) { return s.id(); }, shape_);
  }

  std::vector<Intersection> intersect(const utility::Ray& ray) const noexcept {
    return std::visit([ray](auto &s) { return s.intersect(ray); }, shape_);
  }

  utility::Tuple normalAt(const utility::Tuple &point) const noexcept {
    return std::visit([point](auto &s) { return s.normalAt(point); }, shape_);
  }

  ShapeVariant shape_;
};


} // namespace geometry
} // namespace raytracer

#endif // SHAPE_HPP