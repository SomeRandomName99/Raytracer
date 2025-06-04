#ifndef AABB_H
#define AABB_H

#include "Tuple.hpp"
#include "Intersections.hpp"

namespace raytracer {
namespace utility {

using namespace raytracer::geometry;

struct AABB {
  Tuple min{};
  Tuple max{};

  AABB() noexcept = default;
  AABB(const Tuple &p1, const Tuple &p2) noexcept : min{componentWiseMin(p1, p2)}, max{componentWiseMax(p1,p2)} {}
  AABB(const Tuple &p) noexcept : min{p}, max{p} {}

  bool intersect(const Ray &ray) const noexcept {
    double tx1 = (min.x() - ray.origin_.x()) / ray.direction_.x();
    double tx2 = (max.x() - ray.origin_.x()) / ray.direction_.x();

    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (min.y() - ray.origin_.y()) / ray.direction_.y();
    double ty2 = (max.y() - ray.origin_.y()) / ray.direction_.y();

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (min.z() - ray.origin_.z()) / ray.direction_.z();
    double tz2 = (max.z() - ray.origin_.z()) / ray.direction_.z();

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax < tmin;
  }

  void expandToInclude(const AABB &other) noexcept {
    min = componentWiseMin(min, other.min);
    max = componentWiseMax(max, other.max);
  }

  void expandToInclude(const Tuple &p) noexcept {
    min = componentWiseMin(min, p);
    max = componentWiseMax(max, p);
  }

  AABB transform(const Matrix<4, 4> &matrix) const noexcept {
    AABB transformedAABB;

    transformedAABB.expandToInclude(matrix*Point(min.x(), min.y(), min.z()));
    transformedAABB.expandToInclude(matrix*Point(min.x(), min.y(), max.z()));
    transformedAABB.expandToInclude(matrix*Point(min.x(), max.y(), min.z()));
    transformedAABB.expandToInclude(matrix*Point(min.x(), max.y(), max.z()));
    transformedAABB.expandToInclude(matrix*Point(max.x(), min.y(), min.z()));
    transformedAABB.expandToInclude(matrix*Point(max.x(), min.y(), max.z()));
    transformedAABB.expandToInclude(matrix*Point(max.x(), max.y(), min.z()));
    transformedAABB.expandToInclude(matrix*Point(max.x(), max.y(), max.z()));

    return transformedAABB;
  }
};

} // namespace utility
} // namespace raytracer

#endif // AABB_H